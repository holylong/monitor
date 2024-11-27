#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QSplashScreen>
#include <QPainter>
#include <QTimer>
#include <QLinearGradient>
#include <QScreen>
#include <QMainWindow>
#include <QPainterPath>

// 自定义 SplashWindow 类
class SplashWindow : public QSplashScreen {
    Q_OBJECT
public:
    explicit SplashWindow(QScreen *screen = nullptr, const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = Qt::WindowFlags())
        : QSplashScreen(screen, pixmap, f) {}

signals:
    void splashClosed(); // 定义信号，用于通知主界面启动

protected:
    void paintEvent(QPaintEvent *event) override {
        QSplashScreen::paintEvent(event);

        // QPainter painter(this);
        // painter.setRenderHint(QPainter::Antialiasing, true);

        // int width = this->width();
        // int height = this->height();

        // 绘制渐变背景
        // QLinearGradient gradient(0, height - 100, 0, height); // 渐变从下部开始
        // gradient.setColorAt(0.0, QColor(41, 121, 255));  // 深蓝
        // gradient.setColorAt(1.0, QColor(208, 233, 255)); // 浅蓝
        // painter.fillRect(rect(), gradient);

        // 绘制贴近底部的曲线
        // QPainterPath path;
        // path.moveTo(0, height); // 起点在左下角
        // path.cubicTo(width * 0.25, height - 50, width * 0.75, height + 50, width, height); // 贝塞尔曲线
        // path.lineTo(width, height); // 连线到右下角
        // path.lineTo(0, height);     // 连线回到左下角
        // path.closeSubpath();

        // QLinearGradient curveGradient(0, height - 100, 0, height);
        // curveGradient.setColorAt(0.0, QColor(66, 165, 245));  // 深蓝
        // curveGradient.setColorAt(1.0, QColor(144, 202, 249)); // 浅蓝

        // painter.setBrush(curveGradient);
        // painter.setPen(Qt::NoPen);
        // painter.drawPath(path);
    }

public slots:
    void closeSplash() {
        this->close();       // 关闭 Splash 窗口
        emit splashClosed(); // 发出信号通知主窗口启动
    }
};

#endif // SPLASHWINDOW_H
