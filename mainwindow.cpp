#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chartwidget.h"

#include <keyboardhistory.h>
#include <mousehistory.h>
#include <QDebug>
#include <hookutil.h>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPainter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <networker.h>
#include <QMenu>
#include <QStandardPaths>
#include <QSystemTrayIcon>

#include "frameplayer.h"
#include "qdatethread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _dateThread(new QDateThread())
{
    ui->setupUi(this);
    _dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    connect(KeyboardHistory::instance(), &KeyboardHistory::getKey, [=](int key)
            {
                  if(_dateStr != QDate::currentDate().toString("yyyy-MM-dd")){
                      _dateStr = QDate::currentDate().toString("yyyy-MM-dd");
                      _keynum = 0;
                      _mousenum = 0;
                  }
                  ++_keynum;
                  updateValue();
                  trySave();
            });

    connect(MouseHistory::instance(), &MouseHistory::getKey, [=](int key){
        if(_dateStr != QDate::currentDate().toString("yyyy-MM-dd")){
            _dateStr = QDate::currentDate().toString("yyyy-MM-dd");
            _mousenum = 0;
            _keynum = 0;
        }
        ++_mousenum;
        updateValue();
    });

    startKeyBoardHook();
    startMouseHook();

    initLayout();

    initContextMenu();
    initSystemTrayIcon();

    _netWorker = new Networker();

    QObject::connect(_netWorker, SIGNAL(reportNetworker(const QString&, const QString&)),
    this, SLOT(onUpdateNetworker(const QString&, const QString&)));

    QObject::connect(_netWorker, SIGNAL(reportNetworker(int, int)),
    this, SLOT(onUpdateSpeedColor(int, int)));


    QObject::connect(_netWorker, SIGNAL(reportCpuMemory(double, const QString&)),
    this, SLOT(onUpdateCpuMemory(double, const QString&)));

    QObject::connect(this, SIGNAL(reportCpuUsage(double)),  _catWidget, SLOT(onUpdateCpuUsage(double)));

    _netWorker->start();

    QObject::connect(_dateThread, &QDateThread::sendDate, [=](QString d, QString dt){
        _labelDays->setText(tr("Days") + ":" + d);
        _labelSeconds->setText(tr("Seconds") + ":" + dt);
    });
    _dateThread->start();
}

void MainWindow::onUpdateNetworker(const QString& in, const QString& out)
{
    _labelUpload->setText(out);
    _labelDownload->setText(in);
}

void MainWindow::setUploadSpeedColor(int speed){
    speed /= 2;
    if(speed < 1024){
        _labelUpload->setStyleSheet("color: #456700; background-color:transparent;");  //1K
    }else if(speed < 1024*1024){
        _labelUpload->setStyleSheet("color: #808000; background-color:transparent;");  //1M
    }else if(speed < 1024*1024*3){
        _labelUpload->setStyleSheet("color: red; background-color:transparent;");      //3M
    }else{
        _labelUpload->setStyleSheet("color: purple; background-color:transparent;");   //3M>
    }
}

void MainWindow::setDownloadSpeedColor(int speed){
    speed /= 2;
    if(speed < 1024){
        _labelDownload->setStyleSheet("color: #456700; background-color:transparent;");
    }else if(speed < 1024*1024){
        _labelDownload->setStyleSheet("color: #808000; background-color:transparent;");
    }else if(speed < 1024*1024*3){
        _labelDownload->setStyleSheet("color: red; background-color:transparent;");
    }else{
        _labelDownload->setStyleSheet("color: purple; background-color:transparent;");
    }
}

void MainWindow::onUpdateSpeedColor(int in, int out){
    setDownloadSpeedColor(in);
    setUploadSpeedColor(out);
}

void MainWindow::onUpdateCpuMemory(double cpu, const QString& memo)
{
    // qDebug() << "cpu:" << cpu << "memo:" << memo;
    emit reportCpuUsage(cpu);
    _labelCpu->setText(tr("CPU:") + QString::number(cpu, 10, 2) + "%");
    _labelMemory->setText(tr("Memory:") + memo + "%");
    ui->centralwidget->repaint();
}

void MainWindow::initSystemTrayIcon()
{
    _trayIcon = new QSystemTrayIcon();
    _trayMenu = new QMenu();

//    QAction *actionExit = new QAction();
//    QObject::connect(actionExit, &QAction::triggered, []{
//        qDebug() << "app wait quit";
//    });
//    actionExit->setText("Quit");
//    actionExit->setIcon(QIcon(":/res/quit.png"));
    _trayMenu->addAction(_quitAction);

    _trayIcon->setContextMenu(_trayMenu);
    _trayIcon->setIcon(QIcon(":/res/monitor.png"));
    _trayIcon->show();
}

void MainWindow::initContextMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, [=]{
        _ctxMenu->exec(QCursor::pos());
    });

    _ctxMenu = new QMenu;
    _quitAction = new QAction(tr("Quit"));
    _quitAction->setIcon(QIcon(":/res/quit.png"));
    _moreInfoAction = new QAction(tr("ShowMore"));
    _moreInfoAction->setCheckable(true);
    _hideInfoAction = new QAction(tr("HideMore"));
    _hideInfoAction->setCheckable(true);
    _hideInfoAction->setChecked(true);

    _chartAction = new QAction(tr("ShowChart"));

    _ctxMenu->addAction(_moreInfoAction);
    _ctxMenu->addAction(_hideInfoAction);
    _ctxMenu->addAction(_chartAction);

    _hmMenu = new QMenu(tr("MouseKeyboardTrack"));
    _openHmAction = new QAction(tr("Open"));
    _openHmAction->setCheckable(true);
    _openHmAction->setChecked(true);
    _closeHmAction = new QAction(tr("Close"));
    _closeHmAction->setCheckable(true);
    _restartHmAction = new QAction(tr("Restart"));


    _hmMenu->addAction(_openHmAction);
    _hmMenu->addAction(_closeHmAction);
    _hmMenu->addAction(_restartHmAction);
    _ctxMenu->addMenu(_hmMenu);

    _selectDate = new QAction(tr("SelectDate"));
    _ctxMenu->addAction(_selectDate);
    _ctxMenu->addAction(_quitAction);


    connect(_moreInfoAction, SIGNAL(triggered()), this, SLOT(onMoreInfoCallback()));
    connect(_hideInfoAction, SIGNAL(triggered()), this, SLOT(onHideInfoCallback()));
    connect(_quitAction, SIGNAL(triggered()), this, SLOT(onAppQuit()));

    _chartWidget = new ChartWidget();

    QObject::connect(_chartAction, &QAction::triggered, [&]{
        _chartWidget->show();
    });
    QObject::connect(_openHmAction, &QAction::triggered, [&]{
        _closeHmAction->setChecked(false);
        startKeyBoardHook();
        startMouseHook();
    });
    QObject::connect(_closeHmAction, &QAction::triggered, [&]{
        _openHmAction->setChecked(false);
        stopKeyBoardHook();
        stopMouseHook();
    });

    QObject::connect(_restartHmAction, &QAction::triggered, [&]{
        stopKeyBoardHook();
        stopMouseHook();
        startKeyBoardHook();
        startMouseHook();
    });

}

void MainWindow::onSelectDate()
{

}

void MainWindow::onMoreInfoCallback()
{
    _hideInfoAction->setChecked(false);
    setFixedSize(QSize(328, 110));
    _labelCpu->show();
    _labelMemory->show();
    _labelDays->show();
    _labelSeconds->show();
}

void MainWindow::onHideInfoCallback()
{
    _moreInfoAction->setChecked(false);
    setFixedSize(QSize(328,58));
    _labelCpu->hide();
    _labelMemory->hide();
    _labelDays->hide();
    _labelSeconds->hide();
}


void MainWindow::onAppQuit()
{
   qApp->quit();
}

void MainWindow::updateValue()
{   
    _labelKeyboard->setText("键盘次数:" + QString::number(_keynum));
//    _config->updateTodayKeyBoardValue(_keynum);
    feiker::Config::Instance().updateTodayKeyBoardValue(_keynum);

    _labelMouse->setText("鼠标次数:" + QString::number(_mousenum));
//    _config->updateTodayMouseValue(_mousenum);
    feiker::Config::Instance().updateTodayMouseValue(_mousenum);
}

void MainWindow::trySave()
{
    _step++;
    if(_step == 20){
        _step = 0;
//        _config->saveConfig(_configPath);
        feiker::Config::Instance().saveConfig(_configPath);
    }
}

void MainWindow::initLayout()
{
    _configPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    _configPath += "/config.json";
#if 0
    _config = new feiker::Config;
    _config->loadConfig(_configPath);
    _keynum = _config->getTodayKeyBoardValue();
    _mousenum = _config->getTodayMouseValue();
#else
    feiker::Config::Instance().loadConfig(_configPath);
    _keynum = feiker::Config::Instance().getTodayKeyBoardValue();
    _mousenum = feiker::Config::Instance().getTodayMouseValue();
#endif

    setFixedSize(QSize(320, 58));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);
//    setWindowFlags(| Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QWidget#centralwidget{border-image:url(:/res/background.bmp); border-radius:5px;}");

    // font
    QFont ft;
    ft.setPointSize(11);
    ft.setWeight(QFont::Weight(4));

    _labelKeyboard = new QLabel("键盘次数:" + QString::number(_keynum));
    _labelKeyboard->setStyleSheet("color:black; font:10pt;");
//    _labelKeyboard->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelKeyboard->setFont(ft);

    _labelMouse = new QLabel("鼠标次数:" + QString::number(_mousenum));
    _labelMouse->setStyleSheet("color:black; font:10pt;");
//    _labelMouse->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelMouse->setFont(ft);

    _labelUpload = new QLabel;
    _labelUpload->setText("上传:100M/s");
    _labelUpload->setStyleSheet("color:black; font:10pt;");
//    _labelUpload->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelUpload->setFont(ft);

    _labelDownload = new QLabel;
    _labelDownload->setText("下载:10M/s");
    _labelDownload->setStyleSheet("color:black; font:10pt;");
//    _labelDownload->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelDownload->setFont(ft);

    _labelCpu = new QLabel;
    _labelCpu->setText("CPU:10%");
    _labelCpu->setStyleSheet("color:black; font:10pt;");
//    _labelCpu->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelCpu->setFont(ft);
    _labelCpu->hide();

    _labelMemory = new QLabel;
    _labelMemory->setText("内存:10%");
    _labelMemory->setStyleSheet("color:black; font:10pt;");
//    _labelMemory->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelMemory->setFont(ft);
    _labelMemory->hide();

    /// days
    _labelDays = new QLabel(tr("Days"));
    _labelDays->hide();

    /// seconds
    _labelSeconds = new QLabel(tr("Seconds"));
    _labelSeconds->hide();

#if 0
    QHBoxLayout *networkLayout = new QHBoxLayout();
    networkLayout->addWidget(_labelUpload);
    networkLayout->addWidget(_labelDownload);

    QHBoxLayout *monitorLayout = new QHBoxLayout();
    monitorLayout->addWidget(_labelKeyboard);
    monitorLayout->addWidget(_labelMouse);

    QHBoxLayout *memoryLayout = new QHBoxLayout();
    memoryLayout->addWidget(_labelCpu);
    memoryLayout->addWidget(_labelMemory);

    QHBoxLayout *clockLayout = new QHBoxLayout();
    clockLayout->addWidget(_labelDays);
    clockLayout->addWidget(_labelSeconds);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(networkLayout);
    mainLayout->addLayout(monitorLayout);
    mainLayout->addLayout(memoryLayout);
    mainLayout->addLayout(clockLayout);
    ui->centralwidget->setLayout(mainLayout);
#else
    QGridLayout *statusLayout = new QGridLayout();
    statusLayout->setAlignment(Qt::AlignLeft);

    statusLayout->addWidget(_labelUpload, 0, 0);
    statusLayout->addWidget(_labelDownload, 0, 1);
    statusLayout->addWidget(_labelKeyboard, 1, 0);
    statusLayout->addWidget(_labelMouse, 1, 1);

    statusLayout->addWidget(_labelCpu, 2, 0);
    statusLayout->addWidget(_labelMemory, 2, 1);
    statusLayout->addWidget(_labelDays, 3, 0);
    statusLayout->addWidget(_labelSeconds, 3, 1);

    _catWidget = new FramePlayerWidget(this);
    _catWidget->SetFrameCount(5);
    _catWidget->SetFrameSize(65, 45);
    _catWidget->LoadFrame(":/res/frames");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(_catWidget);
    layout->addLayout(statusLayout);

    ui->centralwidget->setLayout(layout);
#endif
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    _pressPos = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->globalX() - _pressPos.x();
    int dy = event->globalY() - _pressPos.y();

    _pressPos = event->globalPos();
    move(x()+dx, y()+dy);
}

MainWindow::~MainWindow()
{
    qDebug() << "release window";

    while(_dateThread->isRunning())_dateThread->requestInterruption();

    stopKeyBoardHook();
    stopMouseHook();
    _catWidget->Stop();
//    _config->saveConfig(_configPath);
//    delete _config;

    feiker::Config::Instance().saveConfig(_configPath);
    delete ui;

}

