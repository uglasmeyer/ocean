/****************************************************************************
** Meta object code from reading C++ file 'Keyboad_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/Keyboad_dialog.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Keyboad_dialog.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN20Keyboad_Dialog_classE_t {};
} // unnamed namespace

template <> constexpr inline auto Keyboad_Dialog_class::qt_create_metaobjectdata<qt_meta_tag_ZN20Keyboad_Dialog_classE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Keyboad_Dialog_class",
        "buffer_mode",
        "",
        "base_octave",
        "flats",
        "sharps",
        "decay_mode",
        "sliding_mode",
        "save"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'buffer_mode'
        QtMocHelpers::SlotData<void(int)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'base_octave'
        QtMocHelpers::SlotData<void(int)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'flats'
        QtMocHelpers::SlotData<void(int)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'sharps'
        QtMocHelpers::SlotData<void(int)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'decay_mode'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'sliding_mode'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'save'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Keyboad_Dialog_class, qt_meta_tag_ZN20Keyboad_Dialog_classE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Keyboad_Dialog_class::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20Keyboad_Dialog_classE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20Keyboad_Dialog_classE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20Keyboad_Dialog_classE_t>.metaTypes,
    nullptr
} };

void Keyboad_Dialog_class::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Keyboad_Dialog_class *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->buffer_mode((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->base_octave((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->flats((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->sharps((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->decay_mode((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->sliding_mode((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->save(); break;
        default: ;
        }
    }
}

const QMetaObject *Keyboad_Dialog_class::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Keyboad_Dialog_class::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20Keyboad_Dialog_classE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Keyboad_Dialog_class::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
