#ifndef QDATETHREAD_H
#define QDATETHREAD_H

#include <QObject>
#include <QThread>

class QDateThread : public QThread
{
    Q_OBJECT
public:
    explicit QDateThread(QObject *parent = nullptr);


protected:
    void run() override;

signals:
    void sendDate(QString date, QString datetime);

private:

};

#endif // QDATETHREAD_H
