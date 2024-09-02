/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[63];
    char stringdata0[897];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 22), // "pB_Wavedisplay_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 28), // "dial_soft_freq_value_changed"
QT_MOC_LITERAL(4, 64, 15), // "cB_Beat_per_sec"
QT_MOC_LITERAL(5, 80, 22), // "dial_PMW_value_changed"
QT_MOC_LITERAL(6, 103, 24), // "dial_decay_value_changed"
QT_MOC_LITERAL(7, 128, 17), // "get_record_status"
QT_MOC_LITERAL(8, 146, 12), // "MAIN_slot_Hz"
QT_MOC_LITERAL(9, 159, 16), // "MAIN_slot_volume"
QT_MOC_LITERAL(10, 176, 15), // "VCO_slot_volume"
QT_MOC_LITERAL(11, 192, 21), // "Slider_FMO_Hz_changed"
QT_MOC_LITERAL(12, 214, 21), // "Slider_VCO_Hz_changed"
QT_MOC_LITERAL(13, 236, 15), // "FMO_slot_volume"
QT_MOC_LITERAL(14, 252, 13), // "waveform_slot"
QT_MOC_LITERAL(15, 266, 8), // "uint8_t*"
QT_MOC_LITERAL(16, 275, 7), // "uint8_t"
QT_MOC_LITERAL(17, 283, 7), // "QLabel*"
QT_MOC_LITERAL(18, 291, 18), // "Main_Waveform_slot"
QT_MOC_LITERAL(19, 310, 17), // "VCO_Waveform_slot"
QT_MOC_LITERAL(20, 328, 17), // "FMO_Waveform_slot"
QT_MOC_LITERAL(21, 346, 15), // "Controller_Exit"
QT_MOC_LITERAL(22, 362, 8), // "GUI_Exit"
QT_MOC_LITERAL(23, 371, 10), // "Audio_Exit"
QT_MOC_LITERAL(24, 382, 17), // "start_synthesizer"
QT_MOC_LITERAL(25, 400, 15), // "start_audio_srv"
QT_MOC_LITERAL(26, 416, 14), // "start_composer"
QT_MOC_LITERAL(27, 431, 10), // "set_mode_f"
QT_MOC_LITERAL(28, 442, 10), // "set_mode_v"
QT_MOC_LITERAL(29, 453, 10), // "set_mode_o"
QT_MOC_LITERAL(30, 464, 7), // "Sl_mix0"
QT_MOC_LITERAL(31, 472, 7), // "Sl_mix1"
QT_MOC_LITERAL(32, 480, 7), // "Sl_mix2"
QT_MOC_LITERAL(33, 488, 7), // "Sl_mix3"
QT_MOC_LITERAL(34, 496, 7), // "Sl_mix4"
QT_MOC_LITERAL(35, 504, 7), // "Sl_mix5"
QT_MOC_LITERAL(36, 512, 7), // "Sl_mix6"
QT_MOC_LITERAL(37, 520, 7), // "Sl_mix7"
QT_MOC_LITERAL(38, 528, 22), // "slot_dial_ramp_up_down"
QT_MOC_LITERAL(39, 551, 12), // "memory_clear"
QT_MOC_LITERAL(40, 564, 13), // "toggle_Record"
QT_MOC_LITERAL(41, 578, 17), // "read_polygon_data"
QT_MOC_LITERAL(42, 596, 5), // "Store"
QT_MOC_LITERAL(43, 602, 11), // "Clear_Banks"
QT_MOC_LITERAL(44, 614, 14), // "change_status0"
QT_MOC_LITERAL(45, 629, 14), // "change_status1"
QT_MOC_LITERAL(46, 644, 14), // "change_status2"
QT_MOC_LITERAL(47, 659, 14), // "change_status3"
QT_MOC_LITERAL(48, 674, 14), // "change_status4"
QT_MOC_LITERAL(49, 689, 14), // "change_status5"
QT_MOC_LITERAL(50, 704, 14), // "change_status7"
QT_MOC_LITERAL(51, 719, 13), // "File_Director"
QT_MOC_LITERAL(52, 733, 15), // "Spectrum_Dialog"
QT_MOC_LITERAL(53, 749, 11), // "Save_Config"
QT_MOC_LITERAL(54, 761, 11), // "toggle_Mute"
QT_MOC_LITERAL(55, 773, 11), // "connect_fmo"
QT_MOC_LITERAL(56, 785, 11), // "connect_vco"
QT_MOC_LITERAL(57, 797, 17), // "main_adsr_sustain"
QT_MOC_LITERAL(58, 815, 14), // "melody_connect"
QT_MOC_LITERAL(59, 830, 16), // "pB_Debug_clicked"
QT_MOC_LITERAL(60, 847, 16), // "wavfile_selected"
QT_MOC_LITERAL(61, 864, 3), // "arg"
QT_MOC_LITERAL(62, 868, 28) // "hs_hall_effect_value_changed"

    },
    "MainWindow\0pB_Wavedisplay_clicked\0\0"
    "dial_soft_freq_value_changed\0"
    "cB_Beat_per_sec\0dial_PMW_value_changed\0"
    "dial_decay_value_changed\0get_record_status\0"
    "MAIN_slot_Hz\0MAIN_slot_volume\0"
    "VCO_slot_volume\0Slider_FMO_Hz_changed\0"
    "Slider_VCO_Hz_changed\0FMO_slot_volume\0"
    "waveform_slot\0uint8_t*\0uint8_t\0QLabel*\0"
    "Main_Waveform_slot\0VCO_Waveform_slot\0"
    "FMO_Waveform_slot\0Controller_Exit\0"
    "GUI_Exit\0Audio_Exit\0start_synthesizer\0"
    "start_audio_srv\0start_composer\0"
    "set_mode_f\0set_mode_v\0set_mode_o\0"
    "Sl_mix0\0Sl_mix1\0Sl_mix2\0Sl_mix3\0Sl_mix4\0"
    "Sl_mix5\0Sl_mix6\0Sl_mix7\0slot_dial_ramp_up_down\0"
    "memory_clear\0toggle_Record\0read_polygon_data\0"
    "Store\0Clear_Banks\0change_status0\0"
    "change_status1\0change_status2\0"
    "change_status3\0change_status4\0"
    "change_status5\0change_status7\0"
    "File_Director\0Spectrum_Dialog\0Save_Config\0"
    "toggle_Mute\0connect_fmo\0connect_vco\0"
    "main_adsr_sustain\0melody_connect\0"
    "pB_Debug_clicked\0wavfile_selected\0arg\0"
    "hs_hall_effect_value_changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      57,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  299,    2, 0x08 /* Private */,
       3,    0,  300,    2, 0x08 /* Private */,
       4,    1,  301,    2, 0x08 /* Private */,
       5,    0,  304,    2, 0x08 /* Private */,
       6,    0,  305,    2, 0x08 /* Private */,
       7,    0,  306,    2, 0x08 /* Private */,
       8,    0,  307,    2, 0x08 /* Private */,
       9,    0,  308,    2, 0x08 /* Private */,
      10,    0,  309,    2, 0x08 /* Private */,
      11,    1,  310,    2, 0x08 /* Private */,
      12,    1,  313,    2, 0x08 /* Private */,
      13,    0,  316,    2, 0x08 /* Private */,
      14,    5,  317,    2, 0x08 /* Private */,
      18,    1,  328,    2, 0x08 /* Private */,
      19,    1,  331,    2, 0x08 /* Private */,
      20,    1,  334,    2, 0x08 /* Private */,
      21,    0,  337,    2, 0x08 /* Private */,
      22,    0,  338,    2, 0x08 /* Private */,
      23,    0,  339,    2, 0x08 /* Private */,
      24,    0,  340,    2, 0x08 /* Private */,
      25,    0,  341,    2, 0x08 /* Private */,
      26,    0,  342,    2, 0x08 /* Private */,
      27,    0,  343,    2, 0x08 /* Private */,
      28,    0,  344,    2, 0x08 /* Private */,
      29,    0,  345,    2, 0x08 /* Private */,
      30,    1,  346,    2, 0x08 /* Private */,
      31,    1,  349,    2, 0x08 /* Private */,
      32,    1,  352,    2, 0x08 /* Private */,
      33,    1,  355,    2, 0x08 /* Private */,
      34,    1,  358,    2, 0x08 /* Private */,
      35,    1,  361,    2, 0x08 /* Private */,
      36,    1,  364,    2, 0x08 /* Private */,
      37,    1,  367,    2, 0x08 /* Private */,
      38,    0,  370,    2, 0x08 /* Private */,
      39,    0,  371,    2, 0x08 /* Private */,
      40,    0,  372,    2, 0x08 /* Private */,
      41,    0,  373,    2, 0x08 /* Private */,
      42,    0,  374,    2, 0x08 /* Private */,
      43,    0,  375,    2, 0x08 /* Private */,
      44,    0,  376,    2, 0x08 /* Private */,
      45,    0,  377,    2, 0x08 /* Private */,
      46,    0,  378,    2, 0x08 /* Private */,
      47,    0,  379,    2, 0x08 /* Private */,
      48,    0,  380,    2, 0x08 /* Private */,
      49,    0,  381,    2, 0x08 /* Private */,
      50,    0,  382,    2, 0x08 /* Private */,
      51,    0,  383,    2, 0x08 /* Private */,
      52,    0,  384,    2, 0x08 /* Private */,
      53,    0,  385,    2, 0x08 /* Private */,
      54,    0,  386,    2, 0x08 /* Private */,
      55,    0,  387,    2, 0x08 /* Private */,
      56,    0,  388,    2, 0x08 /* Private */,
      57,    0,  389,    2, 0x08 /* Private */,
      58,    0,  390,    2, 0x08 /* Private */,
      59,    0,  391,    2, 0x08 /* Private */,
      60,    1,  392,    2, 0x08 /* Private */,
      62,    1,  395,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 16, QMetaType::Int, QMetaType::Int, 0x80000000 | 17,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   61,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->pB_Wavedisplay_clicked(); break;
        case 1: _t->dial_soft_freq_value_changed(); break;
        case 2: _t->cB_Beat_per_sec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->dial_PMW_value_changed(); break;
        case 4: _t->dial_decay_value_changed(); break;
        case 5: _t->get_record_status(); break;
        case 6: _t->MAIN_slot_Hz(); break;
        case 7: _t->MAIN_slot_volume(); break;
        case 8: _t->VCO_slot_volume(); break;
        case 9: _t->Slider_FMO_Hz_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->Slider_VCO_Hz_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->FMO_slot_volume(); break;
        case 12: _t->waveform_slot((*reinterpret_cast< uint8_t*(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QLabel*(*)>(_a[5]))); break;
        case 13: _t->Main_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->VCO_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->FMO_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->Controller_Exit(); break;
        case 17: _t->GUI_Exit(); break;
        case 18: _t->Audio_Exit(); break;
        case 19: _t->start_synthesizer(); break;
        case 20: _t->start_audio_srv(); break;
        case 21: _t->start_composer(); break;
        case 22: _t->set_mode_f(); break;
        case 23: _t->set_mode_v(); break;
        case 24: _t->set_mode_o(); break;
        case 25: _t->Sl_mix0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->Sl_mix1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->Sl_mix2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->Sl_mix3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->Sl_mix4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->Sl_mix5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->Sl_mix6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->Sl_mix7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->slot_dial_ramp_up_down(); break;
        case 34: _t->memory_clear(); break;
        case 35: _t->toggle_Record(); break;
        case 36: _t->read_polygon_data(); break;
        case 37: _t->Store(); break;
        case 38: _t->Clear_Banks(); break;
        case 39: _t->change_status0(); break;
        case 40: _t->change_status1(); break;
        case 41: _t->change_status2(); break;
        case 42: _t->change_status3(); break;
        case 43: _t->change_status4(); break;
        case 44: _t->change_status5(); break;
        case 45: _t->change_status7(); break;
        case 46: _t->File_Director(); break;
        case 47: _t->Spectrum_Dialog(); break;
        case 48: _t->Save_Config(); break;
        case 49: _t->toggle_Mute(); break;
        case 50: _t->connect_fmo(); break;
        case 51: _t->connect_vco(); break;
        case 52: _t->main_adsr_sustain(); break;
        case 53: _t->melody_connect(); break;
        case 54: _t->pB_Debug_clicked(); break;
        case 55: _t->wavfile_selected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 56: _t->hs_hall_effect_value_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 4:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLabel* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Logfacility_class"))
        return static_cast< Logfacility_class*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 57)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 57;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 57)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 57;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
