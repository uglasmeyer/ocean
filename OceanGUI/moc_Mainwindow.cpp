/****************************************************************************
** Meta object code from reading C++ file 'Mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "Mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[75];
    char stringdata0[1068];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 11), // "Rtsp_Dialog"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 22), // "pB_Wavedisplay_clicked"
QT_MOC_LITERAL(4, 47, 28), // "dial_soft_freq_value_changed"
QT_MOC_LITERAL(5, 76, 15), // "cB_Beat_per_sec"
QT_MOC_LITERAL(6, 92, 22), // "dial_PMW_value_changed"
QT_MOC_LITERAL(7, 115, 24), // "dial_decay_value_changed"
QT_MOC_LITERAL(8, 140, 17), // "get_record_status"
QT_MOC_LITERAL(9, 158, 12), // "MAIN_slot_Hz"
QT_MOC_LITERAL(10, 171, 16), // "MAIN_slot_volume"
QT_MOC_LITERAL(11, 188, 15), // "VCO_slot_volume"
QT_MOC_LITERAL(12, 204, 21), // "Slider_FMO_Hz_changed"
QT_MOC_LITERAL(13, 226, 21), // "Slider_VCO_Hz_changed"
QT_MOC_LITERAL(14, 248, 15), // "FMO_slot_volume"
QT_MOC_LITERAL(15, 264, 13), // "waveform_slot"
QT_MOC_LITERAL(16, 278, 8), // "uint8_t*"
QT_MOC_LITERAL(17, 287, 7), // "uint8_t"
QT_MOC_LITERAL(18, 295, 7), // "QLabel*"
QT_MOC_LITERAL(19, 303, 18), // "Main_Waveform_slot"
QT_MOC_LITERAL(20, 322, 17), // "VCO_Waveform_slot"
QT_MOC_LITERAL(21, 340, 17), // "FMO_Waveform_slot"
QT_MOC_LITERAL(22, 358, 15), // "Controller_Exit"
QT_MOC_LITERAL(23, 374, 8), // "GUI_Exit"
QT_MOC_LITERAL(24, 383, 10), // "Audio_Exit"
QT_MOC_LITERAL(25, 394, 17), // "start_synthesizer"
QT_MOC_LITERAL(26, 412, 15), // "start_audio_srv"
QT_MOC_LITERAL(27, 428, 14), // "start_composer"
QT_MOC_LITERAL(28, 443, 10), // "set_mode_f"
QT_MOC_LITERAL(29, 454, 10), // "set_mode_v"
QT_MOC_LITERAL(30, 465, 10), // "set_mode_o"
QT_MOC_LITERAL(31, 476, 7), // "Sl_mix0"
QT_MOC_LITERAL(32, 484, 7), // "Sl_mix1"
QT_MOC_LITERAL(33, 492, 7), // "Sl_mix2"
QT_MOC_LITERAL(34, 500, 7), // "Sl_mix3"
QT_MOC_LITERAL(35, 508, 7), // "Sl_mix4"
QT_MOC_LITERAL(36, 516, 7), // "Sl_mix5"
QT_MOC_LITERAL(37, 524, 7), // "Sl_mix6"
QT_MOC_LITERAL(38, 532, 7), // "Sl_mix7"
QT_MOC_LITERAL(39, 540, 22), // "slot_dial_ramp_up_down"
QT_MOC_LITERAL(40, 563, 12), // "memory_clear"
QT_MOC_LITERAL(41, 576, 10), // "SaveRecord"
QT_MOC_LITERAL(42, 587, 17), // "read_polygon_data"
QT_MOC_LITERAL(43, 605, 11), // "Clear_Banks"
QT_MOC_LITERAL(44, 617, 12), // "toggle_mute0"
QT_MOC_LITERAL(45, 630, 12), // "toggle_mute1"
QT_MOC_LITERAL(46, 643, 12), // "toggle_mute2"
QT_MOC_LITERAL(47, 656, 12), // "toggle_mute3"
QT_MOC_LITERAL(48, 669, 12), // "toggle_mute4"
QT_MOC_LITERAL(49, 682, 12), // "toggle_mute5"
QT_MOC_LITERAL(50, 695, 12), // "toggle_mute6"
QT_MOC_LITERAL(51, 708, 12), // "toggle_mute7"
QT_MOC_LITERAL(52, 721, 17), // "toggle_store_sta0"
QT_MOC_LITERAL(53, 739, 17), // "toggle_store_sta1"
QT_MOC_LITERAL(54, 757, 17), // "toggle_store_sta2"
QT_MOC_LITERAL(55, 775, 17), // "toggle_store_sta3"
QT_MOC_LITERAL(56, 793, 17), // "toggle_store_sta4"
QT_MOC_LITERAL(57, 811, 17), // "toggle_store_sta5"
QT_MOC_LITERAL(58, 829, 17), // "toggle_store_sta6"
QT_MOC_LITERAL(59, 847, 17), // "toggle_store_sta7"
QT_MOC_LITERAL(60, 865, 9), // "select_S0"
QT_MOC_LITERAL(61, 875, 9), // "select_S1"
QT_MOC_LITERAL(62, 885, 9), // "select_S2"
QT_MOC_LITERAL(63, 895, 9), // "select_S3"
QT_MOC_LITERAL(64, 905, 13), // "File_Director"
QT_MOC_LITERAL(65, 919, 15), // "Spectrum_Dialog"
QT_MOC_LITERAL(66, 935, 11), // "Save_Config"
QT_MOC_LITERAL(67, 947, 11), // "toggle_Mute"
QT_MOC_LITERAL(68, 959, 11), // "connect_fmo"
QT_MOC_LITERAL(69, 971, 11), // "connect_vco"
QT_MOC_LITERAL(70, 983, 17), // "main_adsr_sustain"
QT_MOC_LITERAL(71, 1001, 16), // "pB_Debug_clicked"
QT_MOC_LITERAL(72, 1018, 16), // "wavfile_selected"
QT_MOC_LITERAL(73, 1035, 3), // "arg"
QT_MOC_LITERAL(74, 1039, 28) // "hs_hall_effect_value_changed"

    },
    "MainWindow\0Rtsp_Dialog\0\0pB_Wavedisplay_clicked\0"
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
    "memory_clear\0SaveRecord\0read_polygon_data\0"
    "Clear_Banks\0toggle_mute0\0toggle_mute1\0"
    "toggle_mute2\0toggle_mute3\0toggle_mute4\0"
    "toggle_mute5\0toggle_mute6\0toggle_mute7\0"
    "toggle_store_sta0\0toggle_store_sta1\0"
    "toggle_store_sta2\0toggle_store_sta3\0"
    "toggle_store_sta4\0toggle_store_sta5\0"
    "toggle_store_sta6\0toggle_store_sta7\0"
    "select_S0\0select_S1\0select_S2\0select_S3\0"
    "File_Director\0Spectrum_Dialog\0Save_Config\0"
    "toggle_Mute\0connect_fmo\0connect_vco\0"
    "main_adsr_sustain\0pB_Debug_clicked\0"
    "wavfile_selected\0arg\0hs_hall_effect_value_changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      69,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  359,    2, 0x08 /* Private */,
       3,    0,  360,    2, 0x08 /* Private */,
       4,    0,  361,    2, 0x08 /* Private */,
       5,    1,  362,    2, 0x08 /* Private */,
       6,    0,  365,    2, 0x08 /* Private */,
       7,    0,  366,    2, 0x08 /* Private */,
       8,    0,  367,    2, 0x08 /* Private */,
       9,    0,  368,    2, 0x08 /* Private */,
      10,    0,  369,    2, 0x08 /* Private */,
      11,    0,  370,    2, 0x08 /* Private */,
      12,    1,  371,    2, 0x08 /* Private */,
      13,    1,  374,    2, 0x08 /* Private */,
      14,    0,  377,    2, 0x08 /* Private */,
      15,    5,  378,    2, 0x08 /* Private */,
      19,    1,  389,    2, 0x08 /* Private */,
      20,    1,  392,    2, 0x08 /* Private */,
      21,    1,  395,    2, 0x08 /* Private */,
      22,    0,  398,    2, 0x08 /* Private */,
      23,    0,  399,    2, 0x08 /* Private */,
      24,    0,  400,    2, 0x08 /* Private */,
      25,    0,  401,    2, 0x08 /* Private */,
      26,    0,  402,    2, 0x08 /* Private */,
      27,    0,  403,    2, 0x08 /* Private */,
      28,    0,  404,    2, 0x08 /* Private */,
      29,    0,  405,    2, 0x08 /* Private */,
      30,    0,  406,    2, 0x08 /* Private */,
      31,    1,  407,    2, 0x08 /* Private */,
      32,    1,  410,    2, 0x08 /* Private */,
      33,    1,  413,    2, 0x08 /* Private */,
      34,    1,  416,    2, 0x08 /* Private */,
      35,    1,  419,    2, 0x08 /* Private */,
      36,    1,  422,    2, 0x08 /* Private */,
      37,    1,  425,    2, 0x08 /* Private */,
      38,    1,  428,    2, 0x08 /* Private */,
      39,    0,  431,    2, 0x08 /* Private */,
      40,    0,  432,    2, 0x08 /* Private */,
      41,    0,  433,    2, 0x08 /* Private */,
      42,    0,  434,    2, 0x08 /* Private */,
      43,    0,  435,    2, 0x08 /* Private */,
      44,    1,  436,    2, 0x08 /* Private */,
      45,    1,  439,    2, 0x08 /* Private */,
      46,    1,  442,    2, 0x08 /* Private */,
      47,    1,  445,    2, 0x08 /* Private */,
      48,    1,  448,    2, 0x08 /* Private */,
      49,    1,  451,    2, 0x08 /* Private */,
      50,    1,  454,    2, 0x08 /* Private */,
      51,    1,  457,    2, 0x08 /* Private */,
      52,    0,  460,    2, 0x08 /* Private */,
      53,    0,  461,    2, 0x08 /* Private */,
      54,    0,  462,    2, 0x08 /* Private */,
      55,    0,  463,    2, 0x08 /* Private */,
      56,    0,  464,    2, 0x08 /* Private */,
      57,    0,  465,    2, 0x08 /* Private */,
      58,    0,  466,    2, 0x08 /* Private */,
      59,    0,  467,    2, 0x08 /* Private */,
      60,    0,  468,    2, 0x08 /* Private */,
      61,    0,  469,    2, 0x08 /* Private */,
      62,    0,  470,    2, 0x08 /* Private */,
      63,    0,  471,    2, 0x08 /* Private */,
      64,    0,  472,    2, 0x08 /* Private */,
      65,    0,  473,    2, 0x08 /* Private */,
      66,    0,  474,    2, 0x08 /* Private */,
      67,    0,  475,    2, 0x08 /* Private */,
      68,    0,  476,    2, 0x08 /* Private */,
      69,    0,  477,    2, 0x08 /* Private */,
      70,    0,  478,    2, 0x08 /* Private */,
      71,    0,  479,    2, 0x08 /* Private */,
      72,    1,  480,    2, 0x08 /* Private */,
      74,    1,  483,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
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
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 17, QMetaType::Int, QMetaType::Int, 0x80000000 | 18,    2,    2,    2,    2,    2,
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
    QMetaType::Void, QMetaType::QString,   73,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Rtsp_Dialog(); break;
        case 1: _t->pB_Wavedisplay_clicked(); break;
        case 2: _t->dial_soft_freq_value_changed(); break;
        case 3: _t->cB_Beat_per_sec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->dial_PMW_value_changed(); break;
        case 5: _t->dial_decay_value_changed(); break;
        case 6: _t->get_record_status(); break;
        case 7: _t->MAIN_slot_Hz(); break;
        case 8: _t->MAIN_slot_volume(); break;
        case 9: _t->VCO_slot_volume(); break;
        case 10: _t->Slider_FMO_Hz_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->Slider_VCO_Hz_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->FMO_slot_volume(); break;
        case 13: _t->waveform_slot((*reinterpret_cast< uint8_t*(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QLabel*(*)>(_a[5]))); break;
        case 14: _t->Main_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->VCO_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->FMO_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->Controller_Exit(); break;
        case 18: _t->GUI_Exit(); break;
        case 19: _t->Audio_Exit(); break;
        case 20: _t->start_synthesizer(); break;
        case 21: _t->start_audio_srv(); break;
        case 22: _t->start_composer(); break;
        case 23: _t->set_mode_f(); break;
        case 24: _t->set_mode_v(); break;
        case 25: _t->set_mode_o(); break;
        case 26: _t->Sl_mix0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->Sl_mix1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->Sl_mix2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->Sl_mix3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->Sl_mix4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->Sl_mix5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->Sl_mix6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->Sl_mix7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->slot_dial_ramp_up_down(); break;
        case 35: _t->memory_clear(); break;
        case 36: _t->SaveRecord(); break;
        case 37: _t->read_polygon_data(); break;
        case 38: _t->Clear_Banks(); break;
        case 39: _t->toggle_mute0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->toggle_mute1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->toggle_mute2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->toggle_mute3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->toggle_mute4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->toggle_mute5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->toggle_mute6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->toggle_mute7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->toggle_store_sta0(); break;
        case 48: _t->toggle_store_sta1(); break;
        case 49: _t->toggle_store_sta2(); break;
        case 50: _t->toggle_store_sta3(); break;
        case 51: _t->toggle_store_sta4(); break;
        case 52: _t->toggle_store_sta5(); break;
        case 53: _t->toggle_store_sta6(); break;
        case 54: _t->toggle_store_sta7(); break;
        case 55: _t->select_S0(); break;
        case 56: _t->select_S1(); break;
        case 57: _t->select_S2(); break;
        case 58: _t->select_S3(); break;
        case 59: _t->File_Director(); break;
        case 60: _t->Spectrum_Dialog(); break;
        case 61: _t->Save_Config(); break;
        case 62: _t->toggle_Mute(); break;
        case 63: _t->connect_fmo(); break;
        case 64: _t->connect_vco(); break;
        case 65: _t->main_adsr_sustain(); break;
        case 66: _t->pB_Debug_clicked(); break;
        case 67: _t->wavfile_selected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 68: _t->hs_hall_effect_value_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
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
        if (_id < 69)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 69;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 69)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 69;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
