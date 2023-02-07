#include "mainwindow.h"
#include <mrglobalapplication.h>
//#include <QApplication>
#include <QHotkey>
#include <QDebug>
#include <signal.h>

#include "tipsdialog.h"

#ifdef BUILD_RELEASE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
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
    MainWindow w;
    TipsDialog dialog(&w);
//    TipsDialog dialog;
    QHotkey hotkey(QKeySequence("Ctrl+Alt+Q"), true, &app); //The hotkey will be automatically registered
    if(hotkey.isRegistered()){
        QString str = "hotkey regist ok";
        qDebug() << str;
        dialog.setText(str);
        dialog.show();
    }else{
        QString str = "hotkey regist failed";
        qDebug() << str;
        dialog.setText(str);
        dialog.show();
    }

    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Hotkey Activated - the application will quit now";

        qApp->quit();
    });
    w.show();
    signal(SIGINT, CtrlC);
    atexit(Release);
    return app.exec();
}
