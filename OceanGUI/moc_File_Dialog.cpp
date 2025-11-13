/****************************************************************************
** Meta object code from reading C++ file 'File_Dialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/File_Dialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'File_Dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17File_Dialog_classE_t {};
} // unnamed namespace

template <> constexpr inline auto File_Dialog_class::qt_create_metaobjectdata<qt_meta_tag_ZN17File_Dialog_classE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "File_Dialog_class",
        "Setup_widgets",
        "",
        "Notes_Select",
        "arg1",
        "Instrument_Select",
        "Instrument_Save",
        "Notes_Done_clicked",
        "cB_NotesPerSec",
        "cB_Convention",
        "sB_Octave",
        "cb_Notestype",
        "Longnote",
        "EditMusicXML"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'Setup_widgets'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'Notes_Select'
        QtMocHelpers::SlotData<void(QString)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'Instrument_Select'
        QtMocHelpers::SlotData<void(QString)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'Instrument_Save'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Notes_Done_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'cB_NotesPerSec'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'cB_Convention'
        QtMocHelpers::SlotData<void(int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'sB_Octave'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'cb_Notestype'
        QtMocHelpers::SlotData<void(int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Longnote'
        QtMocHelpers::SlotData<void(bool)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'EditMusicXML'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<File_Dialog_class, qt_meta_tag_ZN17File_Dialog_classE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject File_Dialog_class::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17File_Dialog_classE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17File_Dialog_classE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17File_Dialog_classE_t>.metaTypes,
    nullptr
} };

void File_Dialog_class::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<File_Dialog_class *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->Setup_widgets(); break;
        case 1: _t->Notes_Select((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->Instrument_Select((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->Instrument_Save(); break;
        case 4: _t->Notes_Done_clicked(); break;
        case 5: _t->cB_NotesPerSec((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->cB_Convention((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->sB_Octave((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->cb_Notestype((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->Longnote((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->EditMusicXML(); break;
        default: ;
        }
    }
}

const QMetaObject *File_Dialog_class::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *File_Dialog_class::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17File_Dialog_classE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Logfacility_class"))
        return static_cast< Logfacility_class*>(this);
    if (!strcmp(_clname, "Note_class"))
        return static_cast< Note_class*>(this);
    if (!strcmp(_clname, "sdsstate_struct"))
        return static_cast< sdsstate_struct*>(this);
    return QDialog::qt_metacast(_clname);
}

int File_Dialog_class::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
