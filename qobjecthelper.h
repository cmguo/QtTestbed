#ifndef QOBJECTHELPER_H
#define QOBJECTHELPER_H

#include <QByteArray>
#include <QObject>

class QObjectHelper
{
public:
    static QObject * root;

    static void init();

    static void addGlobalObject(QObject * obj);

    static QByteArray objectName(QObject * obj);

    static QByteArray objectPath(QObject * obj);

    static QObject * getObject(QByteArray const & path);

    static QList<QObject*> childObjects(QObject * parent);

private:
    static QObject * findObject(QObject * parent, QByteArray const & name);
};

class RootObject : public QObject
{
    Q_OBJECT
public:
    RootObject() {}
};

#endif // QOBJECTHELPER_H
