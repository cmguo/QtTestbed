#include "qobjectproxy.h"

QObjectProxy::QObjectProxy(QObject *parent)
    : QObject(parent)
    , stub_(nullptr)
{
}

QObjectProxy::QObjectProxy(QObject *stub, QObject *parent)
    : QObject(parent)
    , stub_(stub)
{
}

QObjectProxy::~QObjectProxy()
{
}

const QMetaObject *QObjectProxy::metaObject() const
{
    return stub_->metaObject();
}

void *QObjectProxy::qt_metacast(const char *cname)
{
    return stub_->qt_metacast(cname);
}

int QObjectProxy::qt_metacall(QMetaObject::Call call, int id, void **v)
{
    return stub_->qt_metacall(call, id, v);
}

