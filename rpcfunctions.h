#ifndef RPCFUNCTIONS_H
#define RPCFUNCTIONS_H

#include "qobjecthelper.h"

namespace anyrpc { class Value; }

using anyrpc::Value;

class QVariant;

class RPCFunctions : QObjectHelper
{
public:

    static void fromVariant(Value & vv, QVariant const & v);

    static void toVariant(Value & vv, QVariant & v);

    static void children(Value& params, Value& result);

    static void fullPath(Value& params, Value& result);

    static void properties(Value& params, Value& result);

    static void property(Value& params, Value& result);

    static void setProperty(Value& params, Value& result);

    static void methods(Value& params, Value& result);

    static void invoke(Value& params, Value& result);

private:
    template <typename T>
    static void execOnUiThread(T const & t);
};

#endif // RPCFUNCTIONS_H
