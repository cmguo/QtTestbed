#include "mousedevice.h"

#include <QApplication>
#include <QMimeData>
#include <QMouseEvent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QWidget>

MouseDevice::MouseDevice(QObject * parent)
    : QObject(parent)
{
}

void MouseDevice::click(QObject *target)
{
    QPointF pos = posOf(target);
    press(target, pos);
    release(target, pos);
}

void MouseDevice::dragBegin(QObject *target)
{
    QPointF pos = posOf(target);
    press(target, pos);
    move(target, pos);
}

void MouseDevice::dragEnd(QObject *target)
{
    QPointF pos = posOf(target);
    move(target, pos);
    release(target, pos);
}

void MouseDevice::dropUrls(QObject *target, const QList<QUrl> &urls)
{
    QPointF pos = posOf(target);
    QPointF localPoint, windowPoint, screenPoint;
    QObject * w = mapToWindow(target, pos, localPoint, windowPoint, screenPoint);
    auto mimeData = new QMimeData();
    mimeData->setUrls(urls);
    QDragEnterEvent e1(windowPoint.toPoint(), Qt::CopyAction | Qt::MoveAction | Qt::LinkAction,
                      mimeData, Qt::MouseButton::NoButton, nullptr);
    QApplication::sendEvent(w, &e1);
    QDragMoveEvent e2(windowPoint.toPoint(), Qt::CopyAction | Qt::MoveAction | Qt::LinkAction,
                      mimeData, Qt::MouseButton::NoButton, nullptr);
    QApplication::sendEvent(w, &e2);
    QDropEvent e3(windowPoint.toPoint(), Qt::CopyAction | Qt::MoveAction | Qt::LinkAction,
                      mimeData, Qt::MouseButton::NoButton, nullptr);
    QApplication::sendEvent(w, &e3);
}

void MouseDevice::press(QObject * target, const QPointF &point)
{
    QPointF localPoint, windowPoint, screenPoint;
    QObject * w = mapToWindow(target, point, localPoint, windowPoint, screenPoint);
    QMouseEvent e(QEvent::MouseButtonPress, localPoint, windowPoint, screenPoint, Qt::LeftButton, {}, {});
    QApplication::sendEvent(w, &e);
}

void MouseDevice::move(QObject * target, const QPointF &point)
{
    QPointF localPoint, windowPoint, screenPoint;
    QObject * w = mapToWindow(target, point, localPoint, windowPoint, screenPoint);
    QMouseEvent e(QEvent::MouseMove, localPoint, windowPoint, screenPoint, Qt::LeftButton, {}, {});
    QApplication::sendEvent(w, &e);
}

void MouseDevice::release(QObject * target, const QPointF &point)
{
    QPointF localPoint, windowPoint, screenPoint;
    QObject * w = mapToWindow(target, point, localPoint, windowPoint, screenPoint);
    QMouseEvent e(QEvent::MouseButtonRelease, localPoint, windowPoint, screenPoint, Qt::LeftButton, {}, {});
    QApplication::sendEvent(w, &e);
}

void MouseDevice::wheel(QObject *target, const QPointF &point, int delta)
{
    QPointF localPoint, windowPoint, screenPoint;
    QObject * w = mapToWindow(target, point, localPoint, windowPoint, screenPoint);
    QWheelEvent e(localPoint, screenPoint, delta, {}, {});
    QApplication::sendEvent(w, &e);
}

QPointF MouseDevice::posOf(QObject *target)
{
    if (QQuickItem* qi = qobject_cast<QQuickItem*>(target)) {
        return {qi->width() * 0.5, qi->height() * 0.5};
    } else if (QWidget * wd = qobject_cast<QWidget *>(target)) {
        return {wd->width() * 0.5, wd->height() * 0.5};
    }
    return {};
}

QObject *MouseDevice::mapToWindow(QObject *target, const QPointF &point,
                                  QPointF & localPoint, QPointF &windowPoint, QPointF & screenPoint)
{
    if (QQuickItem* qi = qobject_cast<QQuickItem*>(target)) {
        QQuickWindow * w = qi->window();
        localPoint = windowPoint = qi->mapToScene(point);
        screenPoint = w->mapToGlobal(windowPoint.toPoint());
        return w;
    } else if (QWidget * wd = qobject_cast<QWidget *>(target)) {
        localPoint = point;
        screenPoint = wd->mapToGlobal(windowPoint.toPoint());
        windowPoint = screenPoint - wd->window()->pos();
        return wd;
    }
    return nullptr;
}

