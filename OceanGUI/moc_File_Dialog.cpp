/****************************************************************************
** Meta object code from reading C++ file 'File_Dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "File_Dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'File_Dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_File_Dialog_class_t {
    QByteArrayData data[11];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_File_Dialog_class_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_File_Dialog_class_t qt_meta_stringdata_File_Dialog_class = {
    {
QT_MOC_LITERAL(0, 0, 17), // "File_Dialog_class"
QT_MOC_LITERAL(1, 18, 29), // "on_cb_notefilenames_activated"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 4), // "arg1"
QT_MOC_LITERAL(4, 54, 31), // "on_cb_instrumentfiles_activated"
QT_MOC_LITERAL(5, 86, 26), // "pb_Instrument_Done_clicked"
QT_MOC_LITERAL(6, 113, 21), // "pb_Notes_Done_clicked"
QT_MOC_LITERAL(7, 135, 14), // "cB_NotesPerSec"
QT_MOC_LITERAL(8, 150, 13), // "cB_Convention"
QT_MOC_LITERAL(9, 164, 9), // "sB_Octave"
QT_MOC_LITERAL(10, 174, 12) // "cb_Notestype"

    },
    "File_Dialog_class\0on_cb_notefilenames_activated\0"
    "\0arg1\0on_cb_instrumentfiles_activated\0"
    "pb_Instrument_Done_clicked\0"
    "pb_Notes_Done_clicked\0cB_NotesPerSec\0"
    "cB_Convention\0sB_Octave\0cb_Notestype"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_File_Dialog_class[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       4,    1,   57,    2, 0x08 /* Private */,
       5,    0,   60,    2, 0x08 /* Private */,
       6,    0,   61,    2, 0x08 /* Private */,
       7,    1,   62,    2, 0x08 /* Private */,
       8,    1,   65,    2, 0x08 /* Private */,
       9,    1,   68,    2, 0x08 /* Private */,
      10,    1,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void File_Dialog_class::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<File_Dialog_class *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_cb_notefilenames_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_cb_instrumentfiles_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->pb_Instrument_Done_clicked(); break;
        case 3: _t->pb_Notes_Done_clicked(); break;
        case 4: _t->cB_NotesPerSec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->cB_Convention((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->sB_Octave((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->cb_Notestype((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject File_Dialog_class::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_File_Dialog_class.data,
    qt_meta_data_File_Dialog_class,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *File_Dialog_class::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *File_Dialog_class::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_File_Dialog_class.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Logfacility_class"))
        return static_cast< Logfacility_class*>(this);
    if (!strcmp(_clname, "Note_class"))
        return static_cast< Note_class*>(this);
    return QDialog::qt_metacast(_clname);
}

int File_Dialog_class::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
