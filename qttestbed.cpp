#include "qttestbed.h"

#include <anyrpc/anyrpc.h>

#include <QObject>

using namespace anyrpc;

QObject * getObject(QString const & path)
{
    QStringList segs = path.split("/", QString::SplitBehavior::SkipEmptyParts);
    if (segs.isEmpty()) {
        return nullptr;
    }
}

void list(Value& params, Value& result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 1) ||
            (!params[0].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    result.SetArray();

}

QtTestbed::QtTestbed()
{
    Server * server = server = new JsonHttpServer;

}
