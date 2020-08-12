#include "watcher.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QWidget>

Watcher::Watcher(QObject *parent)
    : QObject(parent)
{

}

bool Watcher::isVisible(QObject *target)
{
    if (QQuickItem* qi = qobject_cast<QQuickItem*>(target)) {
        return qi->window()->isVisible() && qi->isVisible();
    } else if (QWidget * wd = qobject_cast<QWidget *>(target)) {
        return wd->isVisible();
    }
    return false;
}

void Watcher::screenShot(QObject *target, const QString &file)
{
    if (QQuickItem* qi = qobject_cast<QQuickItem*>(target)) {
        // take screenshot of the full window
        auto windowImage = qi->window()->grabWindow();
        // get the rect of the item in window space in pixels, account for the device pixel ratio
        QRectF imageCropRectItemSpace {0, 0, qi->width(), qi->height()};
        auto imageCropRectF = qi->mapRectToScene(imageCropRectItemSpace);
        QRect imageCropRect = QRectF(imageCropRectF.x() * windowImage.devicePixelRatio(),
            imageCropRectF.y() * windowImage.devicePixelRatio(), imageCropRectF.width() * windowImage.devicePixelRatio(),
            imageCropRectF.height() * windowImage.devicePixelRatio()).toAlignedRect();
        // crop the window image to the item rect
        auto image = windowImage.copy(imageCropRect);
        image.save(file);
    } else if (QWidget * wd = qobject_cast<QWidget *>(target)) {
        wd->grab().save(file);
    }
}
