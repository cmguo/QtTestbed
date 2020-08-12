#ifndef MOUSEDEVICE_H
#define MOUSEDEVICE_H

#include <QObject>
#include <QUrl>

class MouseDevice : public QObject
{
    Q_OBJECT
public:
    MouseDevice(QObject * parent = nullptr);

public slots:
    void click(QObject * target);

    void dragBegin(QObject * target);

    void dragEnd(QObject * target);

    void dropUrls(QObject * target, QList<QUrl> const & urls);

public slots:
    void press(QObject * target, QPointF const & point);

    void move(QObject * target, QPointF const & point);

    void release(QObject * target, QPointF const & point);

    void wheel(QObject * target, const QPointF &point, int delta);

private:
    static QObject * mapToWindow(QObject * target, QPointF const & point, QPointF & windowPoint);
};

#endif // MOUSEDEVICE_H
