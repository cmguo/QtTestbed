#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include <QObject>

class KeyboardDevice : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardDevice(QObject *parent = nullptr);

public slots:
    void key(QObject * target, int key, int modifiers);

    void text(QObject * target, QString const & text);

public slots:
    void press(QObject * target, int key, int modifiers);

    void release(QObject * target, int key, int modifiers);

private:
    static QObject * toWindow(QObject * target);
};

#endif // KEYBOARDDEVICE_H
