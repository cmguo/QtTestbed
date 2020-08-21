#include "qtestbed.h"
#include "rpcfunctions.h"

#ifdef PROD_TEST

#include <anyrpc/anyrpc.h>

using namespace anyrpc;

static Server * server = nullptr;

#endif

void QTestbed::init()
{
    QObjectHelper::init();
#ifdef PROD_TEST
    server = new JsonHttpServer;
    MethodManager *methodManager = server->GetMethodManager();
    methodManager->AddFunction(&RPCFunctions::children, "children", "list child objects");
    methodManager->AddFunction(&RPCFunctions::fullPath, "fullPath", "full path");
    methodManager->AddFunction(&RPCFunctions::properties, "properties", "list properties");
    methodManager->AddFunction(&RPCFunctions::property, "property", "get property");
    methodManager->AddFunction(&RPCFunctions::setProperty, "setProperty", "set property");
    methodManager->AddFunction(&RPCFunctions::methods, "methods", "list methods");
    methodManager->AddFunction(&RPCFunctions::invoke, "invoke", "invoke method");
#endif
}

void QTestbed::start()
{
#ifdef PROD_TEST
    server->BindAndListen(1888);
    server->StartThread();
#endif
}

void QTestbed::addGlobalObject(QObject *obj)
{
    QObjectHelper::addGlobalObject(obj);
}
