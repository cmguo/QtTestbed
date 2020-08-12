#ifndef WATCHER_H
#define WATCHER_H

#include <QObject>

class Watcher : public QObject
{
    Q_OBJECT
public:
    explicit Watcher(QObject *parent = nullptr);

public slots:
    bool isVisible(QObject * target);

    void screenShot(QObject * target, QString const & file);
};

#endif // WATCHER_H
