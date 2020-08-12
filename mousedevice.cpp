#include "mousedevice.h"

#include <QApplication>
#include <QMouseEvent>
#include <QWidget>

MouseDevice::MouseDevice(QObject * parent)
    : QObject(parent)
{
}

void MouseDevice::click(QObject *target)
{

}

void MouseDevice::dragBegin(QObject *target)
{

}

void MouseDevice::dragEnd(QObject *target)
{

}

void MouseDevice::dropUrls(QObject *target, const QList<QUrl> &urls)
{

}

void MouseDevice::press(QObject * target, const QPointF &point)
{
    QMouseEvent e(QEvent::MouseButtonPress, point, Qt::LeftButton, {}, {});
    QApplication::sendEvent(qApp->activeWindow(), &e);
}

void MouseDevice::move(QObject * target, const QPointF &point)
{
    QMouseEvent e(QEvent::MouseMove, point, Qt::LeftButton, {}, {});
    QApplication::sendEvent(qApp->activeWindow(), &e);
}

void MouseDevice::release(QObject * target, const QPointF &point)
{
    QMouseEvent e(QEvent::MouseButtonRelease, {}, Qt::LeftButton, {}, {});
    QApplication::sendEvent(qApp->activeWindow(), &e);
}

void MouseDevice::wheel(QObject *target, const QPointF &point, int delta)
{
    QWheelEvent e(point, delta, {}, {});
    QApplication::sendEvent(qApp->activeWindow(), &e);
}

QObject *MouseDevice::mapToWindow(QObject *target, const QPointF &point, QPointF &windowPoint)
{
    return nullptr;
}

