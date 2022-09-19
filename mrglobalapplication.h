#ifndef MRGLOBALAPPLICATION_H
#define MRGLOBALAPPLICATION_H

#include <QApplication>
#include <QObject>
#include <QEvent>

class MrGlobalApplication : public QApplication
{
public:
    MrGlobalApplication(int &argc, char **argv, int flag = ApplicationFlags);
    bool notify(QObject *, QEvent *);
};

#endif // MRGLOBALAPPLICATION_H
