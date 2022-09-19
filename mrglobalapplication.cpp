#include "mrglobalapplication.h"
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include <QKeyEvent>

bool MrGlobalApplication::notify(QObject * obj, QEvent * event)
{
    if (!obj)
    {
        return false;
    }
    switch (event->type())
    {
    case QEvent::Quit:
         qDebug() << __FUNCTION__ << " quit";
        break;
    case QEvent::Close:
        qDebug() << __FUNCTION__ << " close";
        break;
    case QEvent::KeyPress:
//        qDebug() << __FUNCTION__ << " press:" << static_cast<QKeyEvent*>(event)->key();
        break;
    default:
        break;
    }

    return QApplication::notify(obj, event);
}


MrGlobalApplication::MrGlobalApplication(int &argc, char **argv, int flag)
    :QApplication(argc, argv, flag)
{

}
