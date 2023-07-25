#include "mainwindow.h"
#include <mrglobalapplication.h>
//#include <QApplication>
#include <QHotkey>
#include <QDebug>
#include <QTranslator>
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

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "monitor_" + QLocale(locale).name();
        qDebug() << baseName;
        if (translator.load(":/res/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

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
