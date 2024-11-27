#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTime>
#include <QDate>
#include <QDebug>

namespace feiker{
    class Config : public QObject{
         Q_OBJECT
    public:
        static Config &Instance(){
            return _instance;
        }

        Config(QObject* parent = nullptr):QObject(parent)
        {

        }

        virtual ~Config(){
            qDebug() << "release config";
        }

        /**
         * @brief LoadConfig
         * @return
         */
        int loadConfig(const QString& path){
            if(checkExist(path))
            {
                qDebug() << "config.json exist";
                QFile file(path);
                bool ret;
                if((ret = file.open(QIODevice::ReadOnly)) == true)
                {
                    qDebug() << "open fileok";
                    QByteArray bytes = file.readAll();
                    qDebug() << "file read pass";
                    QJsonDocument doc;
                    doc = QJsonDocument::fromJson(bytes);
                    qDebug() << "to json pass";
                    _obj = doc.object();
                    qDebug() << "to obj pass";
                    qDebug() << "bytes len:" << bytes.length() << " " << doc.toJson();
                             //<< " data:" << bytes.toStdString().c_str();// << " " << doc.toJson().toStdString().c_str();
                    file.close();
                    return 0;
                }
            }else{
                qDebug() << "config.json not exist";
            }
            return 1;
        }

        int saveConfig(const QString& path){
            QFile file(path);
            bool ret;
            if((ret = file.open(QIODevice::WriteOnly)) == true)
            {
                QByteArray bytes;
                if(!_obj.contains("exector"))
                    _obj.insert("exector", "monitor");
                QJsonDocument doc(_obj);
                bytes = doc.toJson();
                file.write(bytes);
                file.close();
                return 0;
            }
            return 1;
        }

        bool checkExist(const QString& path){
            QFile file(path);
            if(file.exists())return true;
            else return false;
        }

        void updateTodayKeyBoardValue(long value){
              _kvalue = value;
            QDate date = QDate::currentDate();
            QString dateStr = date.toString("yyyy-MM-dd");
            // _innerObj.insert("keyboard", QJsonValue::fromVariant(QVariant::fromValue(_kvalue)));
            _innerObj.insert("keyboard", QJsonValue(static_cast<qint64>(_kvalue)));
            _obj.insert(dateStr, _innerObj);
        }

        void updateTodayMouseValue(long value){
            _mvalue = value;
            QDate date = QDate::currentDate();
            QString dateStr = date.toString("yyyy-MM-dd");
            // _innerObj.insert("mouse", QJsonValue::fromVariant(QVariant::fromValue(_mvalue)));
            _innerObj.insert("mouse", QJsonValue(static_cast<qint64>(_mvalue)));
            _obj.insert(dateStr, _innerObj);
        }

        long getTodayKeyBoardValue(){
            QDate date = QDate::currentDate();
            QString dateStr = date.toString("yyyy-MM-dd");
            if(_obj.contains(dateStr)){
                _innerObj = _obj[dateStr].toObject();
                if(_innerObj.contains("keyboard"))
                {
                    // return _innerObj.value("keyboard").toString().toLong();
                    return _innerObj.value("keyboard").toInt();
                }
            }

            return 0;
        }

        long getTodayMouseValue(){
            QDate date = QDate::currentDate();
            QString dateStr = date.toString("yyyy-MM-dd");
            if(_obj.contains(dateStr)){
                _innerObj = _obj[dateStr].toObject();
                if(_innerObj.contains("mouse"))
                {
                    return _innerObj.value("mouse").toInt();
                }
            }

            return 0;
        }

        /**
         * @brief GetAllData
         * @return <data<keyboard, mouse>>
         */
        QVector<QMap<QString, QMap<QString, QString>>> GetAllData(){
            QDate date = QDate::currentDate();
            QDate oldDate(2022, 1, 1);

            return QVector<QMap<QString, QMap<QString, QString>>>();
        }

    private:
        QJsonObject _obj;
        QJsonObject _innerObj;
        QString     _path;
        long        _kvalue;
        long        _mvalue;

        static Config  _instance;
    };
}

#endif // CONFIG_H
