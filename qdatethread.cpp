#include "qdatethread.h"
#include <QDate>
#include <QDebug>

QDateThread::QDateThread(QObject *parent) : QThread(parent)
{

}

void QDateThread::run()
{
    QDate lastDate(2023, 5, 10);
    while(!isInterruptionRequested())
    {
        QDate date = QDate::currentDate();
        QDate nextDate(date.year(), date.month(), date.day()+1);
        if(!date.isValid()){
            nextDate = QDate(date.year(), date.month()+1, 1);
            if(!date.isValid()){
                nextDate = QDate(date.year()+1, 1, 1);
            }
        }

        QDateTime dateTime = QDateTime::currentDateTime();
        QDateTime lastDateTime(nextDate, QTime(0,0));
//        lastDateTime.setDate(nextDate);

        /// 86400
        /// 149605
//        qDebug() << "secs since epoch:" << dateTime.toSecsSinceEpoch();
//        qDebug() << "msecs since epoch:" << dateTime.toMSecsSinceEpoch();
//        qDebug() << "" << dateTime.msecsTo(lastDateTime);
//        qDebug() << "date:" << date.toString("yyyy-MM-dd") << " next:" << nextDate.toString("yyyy-MM-dd");
//        qDebug() << "time:" << QTime(0,0,0).addMSecs(dateTime.msecsTo(lastDateTime)).toString("HH:mm:ss:zzz");

//        emit sendDate(QString::number(date.daysTo(lastDate)), QString::number(dateTime.msecsTo(lastDateTime)));
        emit sendDate(QString::number(date.daysTo(lastDate)),
                      QTime(0,0,0).addMSecs(dateTime.msecsTo(lastDateTime)).toString("HH:mm:ss:zzz"));
        QThread::msleep(10);
    }
}


//void addDate()
//{
//    QDateThread *dateThread = new QDateThread();
//    connect(dateThread, &QDateThread::sendDate, this, &MainWindow::getDate);
//    dateThread->start();
//}
