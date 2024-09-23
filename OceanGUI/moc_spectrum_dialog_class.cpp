/****************************************************************************
** Meta object code from reading C++ file 'spectrum_dialog_class.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "spectrum_dialog_class.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectrum_dialog_class.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Spectrum_Dialog_class_t {
    QByteArrayData data[14];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Spectrum_Dialog_class_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Spectrum_Dialog_class_t qt_meta_stringdata_Spectrum_Dialog_class = {
    {
QT_MOC_LITERAL(0, 0, 21), // "Spectrum_Dialog_class"
QT_MOC_LITERAL(1, 22, 3), // "vS1"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 3), // "vS2"
QT_MOC_LITERAL(4, 31, 3), // "vS3"
QT_MOC_LITERAL(5, 35, 3), // "vS4"
QT_MOC_LITERAL(6, 39, 3), // "vS5"
QT_MOC_LITERAL(7, 43, 3), // "vS6"
QT_MOC_LITERAL(8, 47, 3), // "vS7"
QT_MOC_LITERAL(9, 51, 3), // "vS8"
QT_MOC_LITERAL(10, 55, 4), // "save"
QT_MOC_LITERAL(11, 60, 15), // "select_spec_fmo"
QT_MOC_LITERAL(12, 76, 15), // "select_spec_vco"
QT_MOC_LITERAL(13, 92, 16) // "select_spec_main"

    },
    "Spectrum_Dialog_class\0vS1\0\0vS2\0vS3\0"
    "vS4\0vS5\0vS6\0vS7\0vS8\0save\0select_spec_fmo\0"
    "select_spec_vco\0select_spec_main"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Spectrum_Dialog_class[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08 /* Private */,
       3,    1,   77,    2, 0x08 /* Private */,
       4,    1,   80,    2, 0x08 /* Private */,
       5,    1,   83,    2, 0x08 /* Private */,
       6,    1,   86,    2, 0x08 /* Private */,
       7,    1,   89,    2, 0x08 /* Private */,
       8,    1,   92,    2, 0x08 /* Private */,
       9,    1,   95,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Spectrum_Dialog_class::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Spectrum_Dialog_class *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->vS1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->vS2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->vS3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->vS4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->vS5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->vS6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->vS7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->vS8((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->save(); break;
        case 9: _t->select_spec_fmo(); break;
        case 10: _t->select_spec_vco(); break;
        case 11: _t->select_spec_main(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Spectrum_Dialog_class::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Spectrum_Dialog_class.data,
    qt_meta_data_Spectrum_Dialog_class,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Spectrum_Dialog_class::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Spectrum_Dialog_class::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Spectrum_Dialog_class.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Spectrum_base"))
        return static_cast< Spectrum_base*>(this);
    return QDialog::qt_metacast(_clname);
}

int Spectrum_Dialog_class::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
