#include "mainwindow.h"
#include <mrglobalapplication.h>
//#include <QApplication>
#include <QHotkey>
#include <QDebug>
#include <QTranslator>
#include <signal.h>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include "tipsdialog.h"
#include "splashwindow.h"

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


void setAutoStart(bool enable) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);
    QString applicationName = QApplication::applicationName();
    QString applicationPath = QDir::toNativeSeparators(QApplication::applicationFilePath());

    if (enable) {
        qDebug() << "applicationName:" << applicationName << "applicationPath:" << applicationPath;
        settings.setValue(applicationName, applicationPath);
    } else {
        settings.remove(applicationName);
    }
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
    // TipsDialog dialog(&w);


    // 创建背景图像
    // QPixmap splashPixmap(400, 300);
    // splashPixmap.fill(Qt::white); // 背景颜色为白色
    QPixmap splashPixmap;
    splashPixmap.load(":/res/backwnd.svg");


    // 使用默认屏幕创建 SplashWindow
    QScreen *screen = QGuiApplication::primaryScreen();
    SplashWindow splash(screen, splashPixmap);
    splash.setGeometry(QGuiApplication::primaryScreen()->geometry().center().x() - 200,
                       QGuiApplication::primaryScreen()->geometry().center().y() - 150,
                       400, 300); // 居中显示
    splash.show();

    // 创建主窗口
    //QMainWindow mainWindow;
    //mainWindow.setWindowTitle("Main Window");
    //mainWindow.resize(800, 600);

    // 连接信号槽，初始化完成时启动主窗口
    QObject::connect(&splash, &SplashWindow::splashClosed, [&w]() {
        w.show();
    });

    // 模拟初始化过程
    QTimer::singleShot(3000, [&splash]() {
        // 初始化完成，发送关闭信号
        splash.closeSplash();
    });




//    TipsDialog dialog;
    QHotkey hotkey(QKeySequence("Ctrl+Alt+B"), true, &app); //The hotkey will be automatically registered
    if(hotkey.isRegistered()){
        QString str = "hotkey regist ok";
        qDebug() << str;
        //dialog.setText(str);
        //dialog.show();
    }else{
        QString str = "hotkey regist failed";
        qDebug() << str;
        //dialog.setText(str);
        //dialog.show();
    }

    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Hotkey Activated - the application will quit now";

        QMessageBox msgBox;
        msgBox.setText("Error! HotKey is already register=> Ctrl + Alt + B");
        msgBox.show();
        qApp->quit();

    });
    
    setAutoStart(true);

    //w.show();
    signal(SIGINT, CtrlC);
    atexit(Release);
    return app.exec();
}
