#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <keyboardhistory.h>
#include <mousehistory.h>
#include <QDebug>
#include <hookutil.h>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPainter>

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
}

void MainWindow::UpdateValue()
{   
    _keyboard->setText("keyboard:" + QString::number(_keynum));
    _config->updateTodayKeyBoardValue(_keynum);

    _mouse->setText("mouse:" + QString::number(_mousenum));
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

    _keyboard = new QLabel("keyboard:" + QString::number(_keynum));
    _mouse = new QLabel("mouse:" + QString::number(_mousenum));

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_keyboard);
    mainLayout->addWidget(_mouse);
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

