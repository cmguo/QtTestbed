#include "keyboarddevice.h"

#include <QApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <QWidget>

KeyboardDevice::KeyboardDevice(QObject *parent) : QObject(parent)
{

}

void KeyboardDevice::key(QObject *target, int key, int modifiers)
{
    press(target, key, modifiers);
    release(target, key, modifiers);
}

void KeyboardDevice::text(QObject *target, const QString &text)
{
    QKeyEvent e(QEvent::KeyPress, 0, 0, text);
    QApplication::sendEvent(toWindow(target), &e);
}

void KeyboardDevice::press(QObject *target, int key, int modifiers)
{
    QKeyEvent e(QEvent::KeyPress, key, {});
    QApplication::sendEvent(toWindow(target), &e);
}

void KeyboardDevice::release(QObject *target, int key, int modifiers)
{
    QKeyEvent e(QEvent::KeyRelease, key, {});
    QApplication::sendEvent(toWindow(target), &e);
}

QObject *KeyboardDevice::toWindow(QObject *target)
{
    if (QQuickItem* qi = qobject_cast<QQuickItem*>(target)) {
        return qi->window();
    } else if (QWidget * wd = qobject_cast<QWidget *>(target)) {
        return wd;
    }
    return nullptr;
}
