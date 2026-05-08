/****************************************************************************
** Meta object code from reading C++ file 'SerialCtrl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../@Optic_test/Network/SerialCtrl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SerialCtrl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialCtrl_t {
    QByteArrayData data[21];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialCtrl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialCtrl_t qt_meta_stringdata_SerialCtrl = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SerialCtrl"
QT_MOC_LITERAL(1, 11, 12), // "onSendedData"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10), // "isNeedResp"
QT_MOC_LITERAL(4, 36, 10), // "onRecvData"
QT_MOC_LITERAL(5, 47, 4), // "data"
QT_MOC_LITERAL(6, 52, 16), // "onRecvCommonData"
QT_MOC_LITERAL(7, 69, 13), // "onClearSerial"
QT_MOC_LITERAL(8, 83, 16), // "onDeviceErrorMsg"
QT_MOC_LITERAL(9, 100, 3), // "msg"
QT_MOC_LITERAL(10, 104, 14), // "onDeviceIsBusy"
QT_MOC_LITERAL(11, 119, 17), // "onDeviceSendError"
QT_MOC_LITERAL(12, 137, 17), // "onSetForcedEnable"
QT_MOC_LITERAL(13, 155, 11), // "onSetEnable"
QT_MOC_LITERAL(14, 167, 14), // "onProgressData"
QT_MOC_LITERAL(15, 182, 5), // "value"
QT_MOC_LITERAL(16, 188, 18), // "onCallBackFunction"
QT_MOC_LITERAL(17, 207, 8), // "recvData"
QT_MOC_LITERAL(18, 216, 14), // "writePrintData"
QT_MOC_LITERAL(19, 231, 8), // "sendData"
QT_MOC_LITERAL(20, 240, 10) // "clearQueue"

    },
    "SerialCtrl\0onSendedData\0\0isNeedResp\0"
    "onRecvData\0data\0onRecvCommonData\0"
    "onClearSerial\0onDeviceErrorMsg\0msg\0"
    "onDeviceIsBusy\0onDeviceSendError\0"
    "onSetForcedEnable\0onSetEnable\0"
    "onProgressData\0value\0onCallBackFunction\0"
    "recvData\0writePrintData\0sendData\0"
    "clearQueue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialCtrl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       6,    1,   85,    2, 0x06 /* Public */,
       7,    0,   88,    2, 0x06 /* Public */,
       8,    1,   89,    2, 0x06 /* Public */,
      10,    0,   92,    2, 0x06 /* Public */,
      11,    0,   93,    2, 0x06 /* Public */,
      12,    0,   94,    2, 0x06 /* Public */,
      13,    1,   95,    2, 0x06 /* Public */,
      14,    1,   98,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,  101,    2, 0x08 /* Private */,
      18,    1,  104,    2, 0x0a /* Public */,
      20,    0,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,   17,
    QMetaType::Void, QMetaType::QByteArray,   19,
    QMetaType::Void,

       0        // eod
};

void SerialCtrl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialCtrl *_t = static_cast<SerialCtrl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSendedData((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onRecvData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->onRecvCommonData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->onClearSerial(); break;
        case 4: _t->onDeviceErrorMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onDeviceIsBusy(); break;
        case 6: _t->onDeviceSendError(); break;
        case 7: _t->onSetForcedEnable(); break;
        case 8: _t->onSetEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onProgressData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onCallBackFunction((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 11: _t->writePrintData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 12: _t->clearQueue(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialCtrl::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onSendedData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onRecvData)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onRecvCommonData)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onClearSerial)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onDeviceErrorMsg)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onDeviceIsBusy)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onDeviceSendError)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onSetForcedEnable)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onSetEnable)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (SerialCtrl::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialCtrl::onProgressData)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialCtrl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SerialCtrl.data,
      qt_meta_data_SerialCtrl,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SerialCtrl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialCtrl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialCtrl.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SerialCtrl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void SerialCtrl::onSendedData(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialCtrl::onRecvData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SerialCtrl::onRecvCommonData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialCtrl::onClearSerial()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SerialCtrl::onDeviceErrorMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SerialCtrl::onDeviceIsBusy()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SerialCtrl::onDeviceSendError()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SerialCtrl::onSetForcedEnable()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void SerialCtrl::onSetEnable(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SerialCtrl::onProgressData(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
