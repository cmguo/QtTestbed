#ifndef QOBJECTPROXY_H
#define QOBJECTPROXY_H

#include <QObject>

class QObjectProxy : public QObject
{
    Q_OBJECT_FAKE
public:

    explicit QObjectProxy(QObject *parent = nullptr);
    explicit QObjectProxy(QObject * stub, QObject *parent = nullptr);
    ~QObjectProxy() override;

private:
    QObject * stub_;
};


template <> inline QObjectProxy *qobject_cast<QObjectProxy*>(const QObject *o)
{
    void *result = o ? const_cast<QObject *>(o)->qt_metacast("QObjectProxy") : nullptr;
    return reinterpret_cast<QObjectProxy*>(result);
}

template <> inline QObjectProxy *qobject_cast<QObjectProxy*>(QObject *o)
{
    void *result = o ? o->qt_metacast("QObjectProxy") : nullptr;
    return reinterpret_cast<QObjectProxy*>(result);
}

#endif // QOBJECTPROXY_H
