#include "rpcfunctions.h"

#include "anyrpc/anyrpc.h"

#include <QEvent>
#include <QWaitCondition>
#include <QMutexLocker>
#include <QDateTime>
#include <QMetaMethod>
#include <QRectF>
#include <QDebug>

#include <QApplication>

using namespace anyrpc;

static QMutex m;

template <typename T>
class ExecEvent : public QEvent
{
public:
    ExecEvent(QWaitCondition & c, T const & f) : QEvent(User), c_(c), t_(f) {}
    virtual ~ExecEvent() { QMutexLocker l(&m); t_(); c_.wakeOne(); }
private:
    QWaitCondition & c_;
    T const t_;
};

template <typename T>
void RPCFunctions::execOnUiThread(T const & t)
{
    QWaitCondition c;
    QEvent * e = new ExecEvent<T>(c, t);
    QMutexLocker l(&m);
    QApplication::postEvent(root, e);
    c.wait(&m);
}

template <typename T>
static QByteArray format(T const & t)
{
    QString s;
    QDebug(&s) << t;
    return s.toUtf8();
}

void RPCFunctions::fromVariant(Value &vv, const QVariant &v)
{
    if (!v.isValid()) {
        return;
    }
    if (v.isNull()) {
        vv.SetNull();
        return;
    }
    if (QMetaType::typeFlags(v.userType()) & QMetaType::PointerToQObject) {
        QByteArray p = "$(" + objectPath(v.value<QObject*>()) + ")";
        vv.SetString(p);
        return;
    }
    switch (v.type()) {
    case QVariant::Bool:
        vv.SetBool(v.toBool());
        break;
    case QVariant::Int:
        vv.SetInt(v.toInt());
        break;
    case QVariant::UInt:
        vv.SetUint(v.toUInt());
        break;
    case QVariant::LongLong:
        vv.SetInt64(v.toLongLong());
        break;
    case QVariant::ULongLong:
        vv.SetUint64(v.toULongLong());
        break;
    case QMetaType::Float:
        vv.SetFloat(v.toFloat());
        break;
    case QVariant::Double:
        vv.SetDouble(v.toDouble());
        break;
    case QVariant::DateTime:
        vv.SetDateTime(v.toDateTime().toSecsSinceEpoch());
        break;
    case QVariant::String:
    case QVariant::ByteArray:
        vv.SetString(v.toByteArray());
        break;
    case QVariant::Size:
        vv.SetString(format(v.toPoint()));
        break;
    case QVariant::SizeF:
        vv.SetString(format(v.toPointF()));
        break;
    case QVariant::Point:
        vv.SetString(format(v.toPoint()));
        break;
    case QVariant::PointF:
        vv.SetString(format(v.toPointF()));
        break;
    case QVariant::Rect:
        vv.SetString(format(v.toRect()));
        break;
    case QVariant::RectF:
        vv.SetString(format(v.toRectF()));
        break;
    case QVariant::StringList:
        vv.SetArray(static_cast<size_t>(v.toStringList().size()));
        for (auto i : v.toStringList()) {
            Value vi; vi.SetString(i.toUtf8());
            vv.PushBack(vi);
        }
        break;
    case QMetaType::QVariantList:
        vv.SetArray(static_cast<size_t>(v.toList().size()));
        for (auto i : v.toList()) {
            Value vi; fromVariant(vi, i);
            vv.PushBack(vi);
        }
        break;
    case QVariant::Map:
        vv.SetMap();
        for (auto i = v.toMap().begin(); i != v.toMap().end(); ++i) {
            Value ki; ki.SetString(i.key().toUtf8());
            Value vi; fromVariant(vi, i.value());
            vv.AddMember(ki, vi);
        }
        break;
    case QVariant::Hash:
        vv.SetMap();
        for (auto i = v.toHash().begin(); i != v.toHash().end(); ++i) {
            Value ki; ki.SetString(i.key().toUtf8());
            Value vi; fromVariant(vi, i.value());
            vv.AddMember(ki, vi);
        }
        break;
    default:
        QString s = v.toString();
        if (!s.isEmpty())
            vv.SetString(s.toUtf8());
    }
}

void RPCFunctions::toVariant(Value &vv, QVariant &v)
{
    if (!vv.IsValid())
        return;
    if (vv.IsNull()) {
        v.setValue(nullptr);
    } else if (vv.IsBool()) {
        v.setValue(vv.GetBool());
    } else if (vv.IsInt()) {
        v.setValue(vv.GetInt());
    } else if (vv.IsUint()) {
        v.setValue(vv.GetUint());
    } else if (vv.IsInt64()) {
        v.setValue(vv.GetInt64());
    } else if (vv.IsUint64()) {
        v.setValue(vv.GetUint64());
    } else if (vv.IsFloat()) {
        v.setValue(vv.GetFloat());
    } else if (vv.IsDouble()) {
        v.setValue(vv.GetDouble());
    } else if (vv.IsString()) {
        QByteArray s(vv.GetString());
        if (s.startsWith("$(") && s.endsWith(')')) {
            QObject * o = getObject(s.mid(2, s.length() - 3));
            if (o)
                v.setValue(o);
            return;
        }
        v.setValue(s);
    } else if (vv.IsArray()) {
        QVariantList l;
        for (size_t i = 0; i < vv.Size(); ++i) {
            QVariant vi; toVariant(vv[i], vi);
            l.append(vi);
        }
        v.setValue(l);
    } else if (vv.IsMap()) {
        QVariantMap m;
        for (auto i = vv.MemberBegin(); i != vv.MemberEnd(); ++i) {
            QVariant vi; toVariant(i.GetValue(), vi);
            m.insert(QByteArray(i.GetKey().GetString()), vi);
        }
        v.setValue(m);
    }
}

void RPCFunctions::children(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 1) ||
            (!params[0].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    result.SetArray();
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            for (QObject * c : childObjects(obj)) {
                Value v;
                v.SetString(objectName(c));
                result.PushBack(v);
            }
        }
    });
}

void RPCFunctions::fullPath(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 1) ||
            (!params[0].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    result.SetArray();
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            result.SetString(objectPath(obj));
        }
    });
}

void RPCFunctions::properties(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 1) ||
            (!params[0].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    result.SetArray();
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            for (int i = 0; i < obj->metaObject()->propertyCount(); ++i) {
                Value v;
                v.SetString(obj->metaObject()->property(i).name());
                result.PushBack(v);
            }
        }
    });
}

void RPCFunctions::property(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 2) ||
            (!params[0].IsString()) ||
            (!params[1].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            QVariant v = obj->property(params[1].GetString());
            fromVariant(result, v);
        }
    });
}

void RPCFunctions::setProperty(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 3) ||
            (!params[0].IsString()) ||
            (!params[1].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            char const * k = params[1].GetString();
            QVariant v;
            toVariant(params[2], v);
            result.SetBool(obj->setProperty(k, v));
        }
    });
}

void RPCFunctions::methods(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 1) ||
            (!params[0].IsString()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    result.SetArray();
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            for (int i = 0; i < obj->metaObject()->methodCount(); ++i) {
                Value v;
                v.SetString(obj->metaObject()->method(i).methodSignature());
                result.PushBack(v);
            }
        }
    });
}

void RPCFunctions::invoke(Value &params, Value &result)
{
    if ((!params.IsArray()) ||
            (params.Size() != 3) ||
            (!params[0].IsString()) ||
            (!params[1].IsString()) ||
            (!params[2].IsArray()))
        throw AnyRpcException(AnyRpcErrorInvalidParams, "Invalid parameters");
    execOnUiThread([&]() {
        QVariant target; toVariant(params[0], target);
        if (QObject * obj = target.value<QObject*>()) {
            int index = obj->metaObject()->indexOfSlot(params[1].GetString());
            if (index < 0)
                return;
            Value & args = params[2];
            QMetaMethod method = obj->metaObject()->method(index);
            if (method.parameterCount() >= 4)
                return;
            QGenericArgument argv[4];
            QVariant varg[4];
            QVariant res(method.returnType(), nullptr);
            QGenericReturnArgument ret(QMetaType::typeName(method.returnType()), res.data());
            for (int i = 0; i < method.parameterCount(); ++i) {
                if (static_cast<size_t>(i) < args.Size())
                    toVariant(args[i], varg[i]);
                int t = method.parameterType(i);
                if (!varg[i].canConvert(t) &&
                        !varg[i].convert(QMetaType::QString)) {
                    return;
                }
                if (!varg[i].convert(t))
                    return;
                argv[i] = QGenericArgument(QMetaType::typeName(t), varg[i].data());
            }
            method.invoke(obj, ret, argv[0], argv[1], argv[2], argv[3]);
            fromVariant(result, res);
        }
    });
}
