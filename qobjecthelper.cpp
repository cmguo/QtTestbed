#include "mousedevice.h"
#include "qobjecthelper.h"
#include "qobjectproxy.h"

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QWindow>
#include <QQuickWidget>
#include <QQuickItem>
#include <QQmlContext>

QObject * QObjectHelper::root = nullptr;

static QList<QObject*> globalObjects;
static QMap<QMetaObject const *, QList<QObject*>(*)(QObject*)> getChildren;

static QList<QObject*> defaultChildren(QObject* obj)
{
    return obj->children();
}

static QList<QObject*> rootChildren(QObject*)
{
    if (qApp->activeWindow())
        globalObjects[1] = qApp->activeWindow();
    return globalObjects;
}

static QList<QObject*> quickWidgetChildren(QObject* obj)
{
    QQuickWidget * quickWidget = qobject_cast<QQuickWidget*>(obj);
    QList<QObject*> children{quickWidget->windowHandle(),
                quickWidget->rootContext(),
                quickWidget->rootObject()};
    children.removeAll(nullptr);
    return children;
}


void QObjectHelper::init()
{
    root = new RootObject;
    getChildren[&RootObject::staticMetaObject] = rootChildren;
    getChildren[&QQuickWidget::staticMetaObject] = quickWidgetChildren;
    addGlobalObject(new MouseDevice);
    addGlobalObject(new QWidget); // active window
}

void QObjectHelper::addGlobalObject(QObject *obj)
{
    assert(obj->parent() == nullptr);
    globalObjects.append(obj);
}

QByteArray QObjectHelper::objectName(QObject *obj)
{
    if (!obj->objectName().isEmpty()) {
        return obj->objectName().toUtf8();
    } else {
        return obj->metaObject()->className() +
                (":" + QByteArray::number(reinterpret_cast<qulonglong>(obj), 16));
    }
}

QByteArray QObjectHelper::objectPath(QObject *obj)
{
    QByteArray p;
    while (obj) {
        p = '/' + objectName(obj) + p;
        obj = obj->parent();
    }
    return p;
}

QObject *QObjectHelper::getObject(const QByteArray &path)
{
    QList<QByteArray> segs = path.split('/');
    segs.removeAll("");
    QObject * obj = root;
    for (QByteArray & seg : segs) {
        obj = findObject(obj, seg);
        if (obj == nullptr)
            break;
    }
    return obj;
}

QList<QObject *> QObjectHelper::childObjects(QObject *parent)
{
    QMetaObject const * meta = parent->metaObject();
    if (auto children = getChildren.value(meta))
        return children(parent);
    meta = meta->superClass();
    while (meta) {
        if (auto children = getChildren.value(meta)) {
            getChildren[parent->metaObject()] = children;
            return children(parent);
        }
        meta = meta->superClass();
    }
    getChildren[parent->metaObject()] = defaultChildren;
    return defaultChildren(parent);
}

QObject * QObjectHelper::findObject(QObject * parent, QByteArray const & name)
{
    QObject * objc = nullptr;
    if (name.startsWith(".")) {
        objc = parent->property(name.mid(1)).value<QObject*>();
    } else if (name.startsWith(":")) {
        objc = findObject(parent, name.mid(1));
        if (objc == nullptr) {
            for (QObject * c : childObjects(parent)) {
                objc = findObject(c, name);
                if (objc)
                    break;
            }
        }
    } else if (name.endsWith(":")) {
        for (QObject * c : childObjects(parent)) {
            QByteArray cname = name.left(name.length() - 1);
            if (c->metaObject()->className() == cname) {
                objc = c;
                break;
            }
        }
    } else {
        for (QObject * c : childObjects(parent)) {
            if (objectName(c) == name) {
                objc = c;
                break;
            }
        }
    }
    return objc;
}

