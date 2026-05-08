/****************************************************************************
** Meta object code from reading C++ file 'CSerialNetworkWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../@Optic_test/Common/CommonParentWidget/CSerialNetworkWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CSerialNetworkWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSerialNetworkWidget_t {
    QByteArrayData data[20];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSerialNetworkWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSerialNetworkWidget_t qt_meta_stringdata_CSerialNetworkWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CSerialNetworkWidget"
QT_MOC_LITERAL(1, 21, 11), // "onReadReady"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 8), // "recvData"
QT_MOC_LITERAL(4, 43, 17), // "onReadReadyCommon"
QT_MOC_LITERAL(5, 61, 18), // "onConnectedNetwork"
QT_MOC_LITERAL(6, 80, 19), // "onDisconnectNetwork"
QT_MOC_LITERAL(7, 100, 19), // "onClickedHomeButton"
QT_MOC_LITERAL(8, 120, 15), // "onProgressReady"
QT_MOC_LITERAL(9, 136, 5), // "value"
QT_MOC_LITERAL(10, 142, 6), // "onRecv"
QT_MOC_LITERAL(11, 149, 12), // "onRecvCommon"
QT_MOC_LITERAL(12, 162, 18), // "onSendedGUIProcess"
QT_MOC_LITERAL(13, 181, 10), // "isNeedResp"
QT_MOC_LITERAL(14, 192, 14), // "onForcedEnable"
QT_MOC_LITERAL(15, 207, 8), // "isEnable"
QT_MOC_LITERAL(16, 216, 18), // "onSetForcedDisable"
QT_MOC_LITERAL(17, 235, 17), // "onSetForcedEnable"
QT_MOC_LITERAL(18, 253, 10), // "onProgress"
QT_MOC_LITERAL(19, 264, 16) // "onChildDestroyed"

    },
    "CSerialNetworkWidget\0onReadReady\0\0"
    "recvData\0onReadReadyCommon\0"
    "onConnectedNetwork\0onDisconnectNetwork\0"
    "onClickedHomeButton\0onProgressReady\0"
    "value\0onRecv\0onRecvCommon\0onSendedGUIProcess\0"
    "isNeedResp\0onForcedEnable\0isEnable\0"
    "onSetForcedDisable\0onSetForcedEnable\0"
    "onProgress\0onChildDestroyed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSerialNetworkWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       5,    0,   90,    2, 0x06 /* Public */,
       6,    0,   91,    2, 0x06 /* Public */,
       7,    0,   92,    2, 0x06 /* Public */,
       8,    1,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   96,    2, 0x08 /* Private */,
      11,    1,   99,    2, 0x08 /* Private */,
      12,    1,  102,    2, 0x08 /* Private */,
      14,    1,  105,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,
      18,    1,  110,    2, 0x08 /* Private */,
      19,    1,  113,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::QObjectStar,    2,

       0        // eod
};

void CSerialNetworkWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSerialNetworkWidget *_t = static_cast<CSerialNetworkWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onReadReady((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->onReadReadyCommon((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->onConnectedNetwork(); break;
        case 3: _t->onDisconnectNetwork(); break;
        case 4: _t->onClickedHomeButton(); break;
        case 5: _t->onProgressReady((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onRecv((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->onRecvCommon((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->onSendedGUIProcess((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onForcedEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->onSetForcedDisable(); break;
        case 11: _t->onSetForcedEnable(); break;
        case 12: _t->onProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->onChildDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CSerialNetworkWidget::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSerialNetworkWidget::onReadReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CSerialNetworkWidget::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSerialNetworkWidget::onReadReadyCommon)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CSerialNetworkWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSerialNetworkWidget::onConnectedNetwork)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CSerialNetworkWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSerialNetworkWidget::onDisconnectNetwork)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CSerialNetworkWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSerialNetworkWidget::onClickedHomeButton)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CSerialNetworkWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSerialNetworkWidget::onProgressReady)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CSerialNetworkWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSerialNetworkWidget.data,
      qt_meta_data_CSerialNetworkWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CSerialNetworkWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSerialNetworkWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CSerialNetworkWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CSerialNetworkWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CSerialNetworkWidget::onReadReady(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSerialNetworkWidget::onReadReadyCommon(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CSerialNetworkWidget::onConnectedNetwork()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CSerialNetworkWidget::onDisconnectNetwork()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CSerialNetworkWidget::onClickedHomeButton()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CSerialNetworkWidget::onProgressReady(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
