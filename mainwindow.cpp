#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <keyboardhistory.h>
#include <mousehistory.h>
#include <QDebug>
#include <hookutil.h>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPainter>
#include <QHBoxLayout>
#include <networker.h>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
                  UpdateValue();
                  TrySave();
            });

    connect(MouseHistory::instance(), &MouseHistory::getKey, [=](int key){
        if(_dateStr != QDate::currentDate().toString("yyyy-MM-dd")){
            _dateStr = QDate::currentDate().toString("yyyy-MM-dd");
            _mousenum = 0;
            _keynum = 0;
        }
        ++_mousenum;
        UpdateValue();
    });

    startKeyBoardHook();
    startMouseHook();

    InitLayout();

    InitContextMenu();

    _netWorker = new Networker();
    QObject::connect(_netWorker, SIGNAL(ReportNetworker(const QString&, const QString&)), 
    this, SLOT(UpdateNetworker(const QString&, const QString&)));
    _netWorker->start();
}

void MainWindow::UpdateNetworker(const QString& in, const QString& out)
{
    _labelUpload->setText(out);
    _labelDownload->setText(in);
}

void MainWindow::InitContextMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, [=]{
        _ctxMenu->exec(QCursor::pos());
    });

    _ctxMenu = new QMenu;
    _moreInfoAction = new QAction("显示更多信息");
    _hideInfoAction = new QAction("隐藏更多信息");
    _ctxMenu->addAction(_moreInfoAction);
    _ctxMenu->addAction(_hideInfoAction);

    connect(_moreInfoAction, SIGNAL(triggered()), this, SLOT(OnMoreInfoCallback()));
    connect(_hideInfoAction, SIGNAL(triggered()), this, SLOT(OnHideInfoCallback()));
}

void MainWindow::OnMoreInfoCallback()
{
    setFixedSize(QSize(212,75));
    _labelCpu->show();
    _labelMemory->show();
}

void MainWindow::OnHideInfoCallback()
{
    setFixedSize(QSize(212,50));
    _labelCpu->hide();
    _labelMemory->hide();
}

void MainWindow::UpdateValue()
{   
    _labelKeyboard->setText("键盘次数:" + QString::number(_keynum));
    _config->updateTodayKeyBoardValue(_keynum);

    _labelMouse->setText("鼠标次数:" + QString::number(_mousenum));
    _config->updateTodayMouseValue(_mousenum);    
}

void MainWindow::TrySave()
{
    _step++;
    if(_step == 20){
        _step = 0;
        _config->saveConfig(_configPath);
    }
}

void MainWindow::InitLayout()
{
    _config = new feiker::Config;
    _configPath = QDir::currentPath();
    _configPath += "/config.json";
    _config->loadConfig(_configPath);

    _keynum = _config->getTodayKeyBoardValue();
    _mousenum = _config->getTodayMouseValue();

    setFixedSize(QSize(212,50));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);
//    setWindowFlags(| Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QWidget#centralwidget{border-image:url(:/res/background.bmp); border-radius:5px;}");

    // font
    QFont ft;
    ft.setPointSize(11);

    _labelKeyboard = new QLabel("键盘次数:" + QString::number(_keynum));
    _labelKeyboard->setStyleSheet("color:black; font:10pt;");
    _labelKeyboard->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelKeyboard->setFont(ft);

    _labelMouse = new QLabel("鼠标次数:" + QString::number(_mousenum));
    _labelMouse->setStyleSheet("color:black; font:10pt;");
    _labelMouse->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelMouse->setFont(ft);

    _labelUpload = new QLabel;
    _labelUpload->setText("上传:100M/s");
    _labelUpload->setStyleSheet("color:black; font:10pt;");
    _labelUpload->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelUpload->setFont(ft);

    _labelDownload = new QLabel;
    _labelDownload->setText("下载:10M/s");
    _labelDownload->setStyleSheet("color:black; font:10pt;");
    _labelDownload->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelDownload->setFont(ft);

    _labelCpu = new QLabel;
    _labelCpu->setText("CPU:10%");
    _labelCpu->setStyleSheet("color:black; font:10pt;");
    _labelCpu->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelCpu->setFont(ft);
    _labelCpu->hide();

    _labelMemory = new QLabel;
    _labelMemory->setText("内存:10%");
    _labelMemory->setStyleSheet("color:black; font:10pt;");
    _labelMemory->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    _labelMemory->setFont(ft);
    _labelMemory->hide();

    QHBoxLayout *networkLayout = new QHBoxLayout();
    networkLayout->addWidget(_labelUpload);
    networkLayout->addWidget(_labelDownload);

    QHBoxLayout *monitorLayout = new QHBoxLayout();
    monitorLayout->addWidget(_labelKeyboard);
    monitorLayout->addWidget(_labelMouse);

    QHBoxLayout *memoryLayout = new QHBoxLayout();
    memoryLayout->addWidget(_labelCpu);
    memoryLayout->addWidget(_labelMemory);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(networkLayout);
    mainLayout->addLayout(monitorLayout);
    mainLayout->addLayout(memoryLayout);
    ui->centralwidget->setLayout(mainLayout);
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
    stopKeyBoardHook();
    stopMouseHook();
    _config->saveConfig(_configPath);
    delete _config;
    delete ui;

}

