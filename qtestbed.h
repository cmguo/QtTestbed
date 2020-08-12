#ifndef QTTESTBED_H
#define QTTESTBED_H

#include "QtTestbed_global.h"

class QObject;

class QTTESTBED_EXPORT QTestbed
{
public:
    static void init();

    static void start();

    static void addGlobalObject(QObject * obj);
};

#endif // QTTESTBED_H
