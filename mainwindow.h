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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void InitLayout();

    void TrySave();
    /**
     * @brief 更新记录值
     * 
     * @param name 
     * @param type 0 keyboard 1 mouse 
     */
    void UpdateValue();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    QLabel         *_keyboard;
    long           _keynum{0};
    QLabel         *_mouse;
    long           _mousenum{0};
    QPixmap        _pixmap;

    feiker::Config        *_config;
    QString        _configPath;
    QString        _dateStr;

    //间隔所少次保存一次文件
    long           _step{0};

    //mouse move
    QPoint         _pressPos;
};
#endif // MAINWINDOW_H
