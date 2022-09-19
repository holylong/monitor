#ifndef KEYBOARDHISTORY_H
#define KEYBOARDHISTORY_H

#include <QObject>
#include <QThread>

class KeyboardHistory : public QObject
{
    Q_OBJECT
public:
    KeyboardHistory(QObject* parent = nullptr);

public:
    virtual ~KeyboardHistory(){}
   static KeyboardHistory *&instance()
   {
       static KeyboardHistory *s = nullptr;
       if (s == nullptr)
       {
           s = new KeyboardHistory();
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

#endif // KEYBOARDHISTORY_H
