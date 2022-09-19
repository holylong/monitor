#ifndef MOUSEHISTORY_H
#define MOUSEHISTORY_H

#include <QObject>

class MouseHistory : public QObject
{
    Q_OBJECT
public:
    MouseHistory(QObject* parent = nullptr):QObject(parent){}
    virtual ~MouseHistory(){}
       static MouseHistory *&instance()
       {
           static MouseHistory *s = nullptr;
           if (s == nullptr)
           {
               s = new MouseHistory();
           }
           return s;
       }

     public:
       void setkeyValue(int key)
       {
           emit getKey(key);
       }

     signals:
       void getKey(int);
};

#endif // MOUSEHISTORY_H
