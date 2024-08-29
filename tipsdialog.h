#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QCoreApplication>
#include <QTimer>
#include <QEventLoop>

//#define SEL_DEF

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class TipsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TipsDialog(QWidget *parent = nullptr);

    void setText(QString& text);

protected:
    bool event(QEvent *event);

signals:

private:
    //类似于sleep不是想要的效果
    void Delay(int msec)
    {
#ifdef SEL_DEF
        QTime delTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < delTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
#elif defined(OFFICIAL)
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<msec);
#else
        QEventLoop loop;
        QTimer::singleShot(msec, &loop, SLOT(quit()));
        loop.exec();
#endif
    }


private:
    Ui::Dialog *ui;
};

#endif // TIPSDIALOG_H
