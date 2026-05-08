/****************************************************************************
** Meta object code from reading C++ file 'AnalysisWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../@Optic_test/Widgets/MainWidgets/AnalysisWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnalysisWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnalysisWidget_t {
    QByteArrayData data[21];
    char stringdata0[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnalysisWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnalysisWidget_t qt_meta_stringdata_AnalysisWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AnalysisWidget"
QT_MOC_LITERAL(1, 15, 17), // "sendSerialCommand"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "command"
QT_MOC_LITERAL(4, 42, 18), // "requestTRFAnalysis"
QT_MOC_LITERAL(5, 61, 5), // "cycle"
QT_MOC_LITERAL(6, 67, 23), // "onBtnTRFAnalysisClicked"
QT_MOC_LITERAL(7, 91, 13), // "onBandChanged"
QT_MOC_LITERAL(8, 105, 5), // "index"
QT_MOC_LITERAL(9, 111, 23), // "onBtnAnalysisLedClicked"
QT_MOC_LITERAL(10, 135, 16), // "onBtnSaveClicked"
QT_MOC_LITERAL(11, 152, 16), // "onBtnLoadClicked"
QT_MOC_LITERAL(12, 169, 16), // "onBtnTestClicked"
QT_MOC_LITERAL(13, 186, 18), // "onBtnScreenClicked"
QT_MOC_LITERAL(14, 205, 15), // "onBtnAddClicked"
QT_MOC_LITERAL(15, 221, 22), // "onBtnRemoveItemClicked"
QT_MOC_LITERAL(16, 244, 20), // "onBtnSaveListClicked"
QT_MOC_LITERAL(17, 265, 22), // "onBtnShowResultClicked"
QT_MOC_LITERAL(18, 288, 26), // "onListWidgetAllItemClicked"
QT_MOC_LITERAL(19, 315, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(20, 332, 4) // "item"

    },
    "AnalysisWidget\0sendSerialCommand\0\0"
    "command\0requestTRFAnalysis\0cycle\0"
    "onBtnTRFAnalysisClicked\0onBandChanged\0"
    "index\0onBtnAnalysisLedClicked\0"
    "onBtnSaveClicked\0onBtnLoadClicked\0"
    "onBtnTestClicked\0onBtnScreenClicked\0"
    "onBtnAddClicked\0onBtnRemoveItemClicked\0"
    "onBtnSaveListClicked\0onBtnShowResultClicked\0"
    "onListWidgetAllItemClicked\0QListWidgetItem*\0"
    "item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnalysisWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   90,    2, 0x08 /* Private */,
       7,    1,   91,    2, 0x08 /* Private */,
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    0,   99,    2, 0x08 /* Private */,
      15,    0,  100,    2, 0x08 /* Private */,
      16,    0,  101,    2, 0x08 /* Private */,
      17,    0,  102,    2, 0x08 /* Private */,
      18,    1,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

void AnalysisWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnalysisWidget *_t = static_cast<AnalysisWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSerialCommand((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->requestTRFAnalysis((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onBtnTRFAnalysisClicked(); break;
        case 3: _t->onBandChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onBtnAnalysisLedClicked(); break;
        case 5: _t->onBtnSaveClicked(); break;
        case 6: _t->onBtnLoadClicked(); break;
        case 7: _t->onBtnTestClicked(); break;
        case 8: _t->onBtnScreenClicked(); break;
        case 9: _t->onBtnAddClicked(); break;
        case 10: _t->onBtnRemoveItemClicked(); break;
        case 11: _t->onBtnSaveListClicked(); break;
        case 12: _t->onBtnShowResultClicked(); break;
        case 13: _t->onListWidgetAllItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AnalysisWidget::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AnalysisWidget::sendSerialCommand)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AnalysisWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AnalysisWidget::requestTRFAnalysis)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AnalysisWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AnalysisWidget.data,
      qt_meta_data_AnalysisWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AnalysisWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnalysisWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnalysisWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AnalysisWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AnalysisWidget::sendSerialCommand(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AnalysisWidget::requestTRFAnalysis(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
