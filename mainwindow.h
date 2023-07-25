#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <config.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum KeyType{
    KtUnknown,
    KtKeyboard,
    KtMouse
};

class Networker;
class ChartWidget;
class QDateThread;
class FramePlayerWidget;
class QSystemTrayIcon;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void initLayout();

    void trySave();
    /**
     * @brief 更新记录值
     * 
     * @param name 
     * @param type 0 keyboard 1 mouse 
     */
    void updateValue();

    void initContextMenu();

    void initSystemTrayIcon();

private slots:
    void onUpdateNetworker(const QString& in, const QString& out);
    void onUpdateCpuMemory(double cpu, const QString& memo);
    void onMoreInfoCallback();
    void onAppQuit();
    void onHideInfoCallback();
    void onSelectDate();

signals:
    void reportCpuUsage(double cpu);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    ChartWidget    *_chartWidget;
    // cat player
    FramePlayerWidget* _catWidget;

    QLabel         *_labelKeyboard;
    long           _keynum{0};

    QLabel         *_labelMouse;
    long           _mousenum{0};
    QPixmap        _pixmap;

    QLabel         *_labelUpload;
    QLabel         *_labelDownload;
    QLabel         *_labelCpu;
    QLabel         *_labelMemory;
    QLabel         *_labelDays;
    QLabel         *_labelSeconds;

//    feiker::Config        *_config;
    QString        _configPath;
    QString        _dateStr;

    //间隔所少次保存一次文件
    long           _step{0};

    //mouse move
    QPoint         _pressPos;

    Networker      *_netWorker;
    QDateThread    *_dateThread;

    QMenu          *_ctxMenu;
    QMenu          *_hmMenu;
    QMenu          *_trayMenu;
    QSystemTrayIcon*_trayIcon;

    QAction        *_moreInfoAction;
    QAction        *_hideInfoAction;
    QAction        *_openHmAction;
    QAction        *_closeHmAction;
    QAction        *_restartHmAction;
    QAction        *_chartAction;
    QAction        *_quitAction;
    QAction        *_selectDate;
};
#endif // MAINWINDOW_H
