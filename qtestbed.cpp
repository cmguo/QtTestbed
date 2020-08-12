#include "qtestbed.h"
#include "rpcfunctions.h"

#include <anyrpc/anyrpc.h>

#include <QObject>

using namespace anyrpc;

static Server * server = nullptr;

void QTestbed::init()
{
    QObjectHelper::init();
    server = new JsonHttpServer;
    MethodManager *methodManager = server->GetMethodManager();
    methodManager->AddFunction(&RPCFunctions::children, "children", "list child objects");
    methodManager->AddFunction(&RPCFunctions::fullPath, "fullPath", "full path");
    methodManager->AddFunction(&RPCFunctions::properties, "properties", "list properties");
    methodManager->AddFunction(&RPCFunctions::property, "property", "get property");
    methodManager->AddFunction(&RPCFunctions::setProperty, "setProperty", "set property");
    methodManager->AddFunction(&RPCFunctions::methods, "methods", "list methods");
    methodManager->AddFunction(&RPCFunctions::invoke, "invoke", "invoke method");
}

void QTestbed::start()
{
    server->BindAndListen(1888);
    server->StartThread();
}

void QTestbed::addGlobalObject(QObject *obj)
{
    QObjectHelper::addGlobalObject(obj);
}
