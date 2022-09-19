#include "mainwindow.h"
#include <mrglobalapplication.h>
//#include <QApplication>
#include <QHotkey>
#include <QDebug>
#include <signal.h>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

void CtrlC(int){
    qDebug() << "close program";
    exit(0);
}

void Release(){
    qDebug() << "release something after";
}

int main(int argc, char *argv[])
{
    MrGlobalApplication app(argc, argv);

    QHotkey hotkey(QKeySequence("Ctrl+Alt+Q"), true, &app); //The hotkey will be automatically registered
        qDebug() << "Is segistered:" << hotkey.isRegistered();

        QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
            qDebug() << "Hotkey Activated - the application will quit now";
            qApp->quit();
        });
    MainWindow w;
    w.show();
    signal(SIGINT, CtrlC);
    atexit(Release);
    return app.exec();
}
