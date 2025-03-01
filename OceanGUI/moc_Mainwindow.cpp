/****************************************************************************
** Meta object code from reading C++ file 'Mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
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
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[77];
    char stringdata0[1125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "pb_Capture"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "Rtsp_Dialog"
QT_MOC_LITERAL(4, 35, 10), // "SDS_Dialog"
QT_MOC_LITERAL(5, 46, 28), // "dial_soft_freq_value_changed"
QT_MOC_LITERAL(6, 75, 15), // "cB_Beat_per_sec"
QT_MOC_LITERAL(7, 91, 22), // "dial_PMW_value_changed"
QT_MOC_LITERAL(8, 114, 24), // "dial_decay_value_changed"
QT_MOC_LITERAL(9, 139, 17), // "get_record_status"
QT_MOC_LITERAL(10, 157, 16), // "MAIN_slot_volume"
QT_MOC_LITERAL(11, 174, 15), // "VCO_slot_volume"
QT_MOC_LITERAL(12, 190, 15), // "FMO_slot_volume"
QT_MOC_LITERAL(13, 206, 15), // "Slider_OSC_Freq"
QT_MOC_LITERAL(14, 222, 15), // "Slider_FMO_Freq"
QT_MOC_LITERAL(15, 238, 15), // "Slider_VCO_Freq"
QT_MOC_LITERAL(16, 254, 17), // "Slider_VCO_Adjust"
QT_MOC_LITERAL(17, 272, 17), // "Slider_FMO_Adjust"
QT_MOC_LITERAL(18, 290, 18), // "Main_Waveform_slot"
QT_MOC_LITERAL(19, 309, 17), // "VCO_Waveform_slot"
QT_MOC_LITERAL(20, 327, 17), // "FMO_Waveform_slot"
QT_MOC_LITERAL(21, 345, 15), // "Controller_Exit"
QT_MOC_LITERAL(22, 361, 8), // "GUI_Exit"
QT_MOC_LITERAL(23, 370, 10), // "Audio_Exit"
QT_MOC_LITERAL(24, 381, 17), // "start_synthesizer"
QT_MOC_LITERAL(25, 399, 15), // "start_audio_srv"
QT_MOC_LITERAL(26, 415, 14), // "start_composer"
QT_MOC_LITERAL(27, 430, 10), // "set_mode_f"
QT_MOC_LITERAL(28, 441, 10), // "set_mode_v"
QT_MOC_LITERAL(29, 452, 10), // "set_mode_o"
QT_MOC_LITERAL(30, 463, 7), // "Sl_mix0"
QT_MOC_LITERAL(31, 471, 7), // "Sl_mix1"
QT_MOC_LITERAL(32, 479, 7), // "Sl_mix2"
QT_MOC_LITERAL(33, 487, 7), // "Sl_mix3"
QT_MOC_LITERAL(34, 495, 7), // "Sl_mix4"
QT_MOC_LITERAL(35, 503, 7), // "Sl_mix5"
QT_MOC_LITERAL(36, 511, 7), // "Sl_mix6"
QT_MOC_LITERAL(37, 519, 7), // "Sl_mix7"
QT_MOC_LITERAL(38, 527, 22), // "slot_dial_ramp_up_down"
QT_MOC_LITERAL(39, 550, 12), // "memory_clear"
QT_MOC_LITERAL(40, 563, 10), // "SaveRecord"
QT_MOC_LITERAL(41, 574, 17), // "read_polygon_data"
QT_MOC_LITERAL(42, 592, 11), // "Clear_Banks"
QT_MOC_LITERAL(43, 604, 12), // "toggle_mute0"
QT_MOC_LITERAL(44, 617, 12), // "toggle_mute1"
QT_MOC_LITERAL(45, 630, 12), // "toggle_mute2"
QT_MOC_LITERAL(46, 643, 12), // "toggle_mute3"
QT_MOC_LITERAL(47, 656, 12), // "toggle_mute4"
QT_MOC_LITERAL(48, 669, 12), // "toggle_mute5"
QT_MOC_LITERAL(49, 682, 12), // "toggle_mute6"
QT_MOC_LITERAL(50, 695, 12), // "toggle_mute7"
QT_MOC_LITERAL(51, 708, 17), // "toggle_store_sta0"
QT_MOC_LITERAL(52, 726, 17), // "toggle_store_sta1"
QT_MOC_LITERAL(53, 744, 17), // "toggle_store_sta2"
QT_MOC_LITERAL(54, 762, 17), // "toggle_store_sta3"
QT_MOC_LITERAL(55, 780, 17), // "toggle_store_sta4"
QT_MOC_LITERAL(56, 798, 17), // "toggle_store_sta5"
QT_MOC_LITERAL(57, 816, 17), // "toggle_store_sta6"
QT_MOC_LITERAL(58, 834, 17), // "toggle_store_sta7"
QT_MOC_LITERAL(59, 852, 11), // "select_Sds0"
QT_MOC_LITERAL(60, 864, 11), // "select_Sds1"
QT_MOC_LITERAL(61, 876, 11), // "select_Sds2"
QT_MOC_LITERAL(62, 888, 11), // "select_Sds3"
QT_MOC_LITERAL(63, 900, 13), // "File_Director"
QT_MOC_LITERAL(64, 914, 15), // "Spectrum_Dialog"
QT_MOC_LITERAL(65, 930, 11), // "Save_Config"
QT_MOC_LITERAL(66, 942, 11), // "toggle_Mute"
QT_MOC_LITERAL(67, 954, 11), // "connect_fmo"
QT_MOC_LITERAL(68, 966, 11), // "connect_vco"
QT_MOC_LITERAL(69, 978, 17), // "main_adsr_sustain"
QT_MOC_LITERAL(70, 996, 16), // "pB_Debug_clicked"
QT_MOC_LITERAL(71, 1013, 16), // "wavfile_selected"
QT_MOC_LITERAL(72, 1030, 3), // "arg"
QT_MOC_LITERAL(73, 1034, 19), // "pB_oscgroup_clicked"
QT_MOC_LITERAL(74, 1054, 22), // "pB_Wavedisplay_clicked"
QT_MOC_LITERAL(75, 1077, 18), // "pB_fftmode_clicked"
QT_MOC_LITERAL(76, 1096, 28) // "hs_hall_effect_value_changed"

    },
    "MainWindow\0pb_Capture\0\0Rtsp_Dialog\0"
    "SDS_Dialog\0dial_soft_freq_value_changed\0"
    "cB_Beat_per_sec\0dial_PMW_value_changed\0"
    "dial_decay_value_changed\0get_record_status\0"
    "MAIN_slot_volume\0VCO_slot_volume\0"
    "FMO_slot_volume\0Slider_OSC_Freq\0"
    "Slider_FMO_Freq\0Slider_VCO_Freq\0"
    "Slider_VCO_Adjust\0Slider_FMO_Adjust\0"
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
    "select_Sds0\0select_Sds1\0select_Sds2\0"
    "select_Sds3\0File_Director\0Spectrum_Dialog\0"
    "Save_Config\0toggle_Mute\0connect_fmo\0"
    "connect_vco\0main_adsr_sustain\0"
    "pB_Debug_clicked\0wavfile_selected\0arg\0"
    "pB_oscgroup_clicked\0pB_Wavedisplay_clicked\0"
    "pB_fftmode_clicked\0hs_hall_effect_value_changed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      74,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  384,    2, 0x08 /* Private */,
       3,    0,  385,    2, 0x08 /* Private */,
       4,    0,  386,    2, 0x08 /* Private */,
       5,    0,  387,    2, 0x08 /* Private */,
       6,    1,  388,    2, 0x08 /* Private */,
       7,    0,  391,    2, 0x08 /* Private */,
       8,    0,  392,    2, 0x08 /* Private */,
       9,    0,  393,    2, 0x08 /* Private */,
      10,    0,  394,    2, 0x08 /* Private */,
      11,    0,  395,    2, 0x08 /* Private */,
      12,    0,  396,    2, 0x08 /* Private */,
      13,    1,  397,    2, 0x08 /* Private */,
      14,    1,  400,    2, 0x08 /* Private */,
      15,    1,  403,    2, 0x08 /* Private */,
      16,    1,  406,    2, 0x08 /* Private */,
      17,    1,  409,    2, 0x08 /* Private */,
      18,    1,  412,    2, 0x08 /* Private */,
      19,    1,  415,    2, 0x08 /* Private */,
      20,    1,  418,    2, 0x08 /* Private */,
      21,    0,  421,    2, 0x08 /* Private */,
      22,    0,  422,    2, 0x08 /* Private */,
      23,    0,  423,    2, 0x08 /* Private */,
      24,    0,  424,    2, 0x08 /* Private */,
      25,    0,  425,    2, 0x08 /* Private */,
      26,    0,  426,    2, 0x08 /* Private */,
      27,    0,  427,    2, 0x08 /* Private */,
      28,    0,  428,    2, 0x08 /* Private */,
      29,    0,  429,    2, 0x08 /* Private */,
      30,    1,  430,    2, 0x08 /* Private */,
      31,    1,  433,    2, 0x08 /* Private */,
      32,    1,  436,    2, 0x08 /* Private */,
      33,    1,  439,    2, 0x08 /* Private */,
      34,    1,  442,    2, 0x08 /* Private */,
      35,    1,  445,    2, 0x08 /* Private */,
      36,    1,  448,    2, 0x08 /* Private */,
      37,    1,  451,    2, 0x08 /* Private */,
      38,    0,  454,    2, 0x08 /* Private */,
      39,    0,  455,    2, 0x08 /* Private */,
      40,    0,  456,    2, 0x08 /* Private */,
      41,    0,  457,    2, 0x08 /* Private */,
      42,    0,  458,    2, 0x08 /* Private */,
      43,    0,  459,    2, 0x08 /* Private */,
      44,    0,  460,    2, 0x08 /* Private */,
      45,    0,  461,    2, 0x08 /* Private */,
      46,    0,  462,    2, 0x08 /* Private */,
      47,    0,  463,    2, 0x08 /* Private */,
      48,    0,  464,    2, 0x08 /* Private */,
      49,    0,  465,    2, 0x08 /* Private */,
      50,    0,  466,    2, 0x08 /* Private */,
      51,    0,  467,    2, 0x08 /* Private */,
      52,    0,  468,    2, 0x08 /* Private */,
      53,    0,  469,    2, 0x08 /* Private */,
      54,    0,  470,    2, 0x08 /* Private */,
      55,    0,  471,    2, 0x08 /* Private */,
      56,    0,  472,    2, 0x08 /* Private */,
      57,    0,  473,    2, 0x08 /* Private */,
      58,    0,  474,    2, 0x08 /* Private */,
      59,    0,  475,    2, 0x08 /* Private */,
      60,    0,  476,    2, 0x08 /* Private */,
      61,    0,  477,    2, 0x08 /* Private */,
      62,    0,  478,    2, 0x08 /* Private */,
      63,    0,  479,    2, 0x08 /* Private */,
      64,    0,  480,    2, 0x08 /* Private */,
      65,    0,  481,    2, 0x08 /* Private */,
      66,    0,  482,    2, 0x08 /* Private */,
      67,    0,  483,    2, 0x08 /* Private */,
      68,    0,  484,    2, 0x08 /* Private */,
      69,    0,  485,    2, 0x08 /* Private */,
      70,    0,  486,    2, 0x08 /* Private */,
      71,    1,  487,    2, 0x08 /* Private */,
      73,    0,  490,    2, 0x08 /* Private */,
      74,    0,  491,    2, 0x08 /* Private */,
      75,    0,  492,    2, 0x08 /* Private */,
      76,    1,  493,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
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
    QMetaType::Void, QMetaType::QString,   72,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->pb_Capture(); break;
        case 1: _t->Rtsp_Dialog(); break;
        case 2: _t->SDS_Dialog(); break;
        case 3: _t->dial_soft_freq_value_changed(); break;
        case 4: _t->cB_Beat_per_sec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->dial_PMW_value_changed(); break;
        case 6: _t->dial_decay_value_changed(); break;
        case 7: _t->get_record_status(); break;
        case 8: _t->MAIN_slot_volume(); break;
        case 9: _t->VCO_slot_volume(); break;
        case 10: _t->FMO_slot_volume(); break;
        case 11: _t->Slider_OSC_Freq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->Slider_FMO_Freq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->Slider_VCO_Freq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->Slider_VCO_Adjust((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->Slider_FMO_Adjust((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->Main_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->VCO_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->FMO_Waveform_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->Controller_Exit(); break;
        case 20: _t->GUI_Exit(); break;
        case 21: _t->Audio_Exit(); break;
        case 22: _t->start_synthesizer(); break;
        case 23: _t->start_audio_srv(); break;
        case 24: _t->start_composer(); break;
        case 25: _t->set_mode_f(); break;
        case 26: _t->set_mode_v(); break;
        case 27: _t->set_mode_o(); break;
        case 28: _t->Sl_mix0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->Sl_mix1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->Sl_mix2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->Sl_mix3((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->Sl_mix4((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->Sl_mix5((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->Sl_mix6((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->Sl_mix7((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->slot_dial_ramp_up_down(); break;
        case 37: _t->memory_clear(); break;
        case 38: _t->SaveRecord(); break;
        case 39: _t->read_polygon_data(); break;
        case 40: _t->Clear_Banks(); break;
        case 41: _t->toggle_mute0(); break;
        case 42: _t->toggle_mute1(); break;
        case 43: _t->toggle_mute2(); break;
        case 44: _t->toggle_mute3(); break;
        case 45: _t->toggle_mute4(); break;
        case 46: _t->toggle_mute5(); break;
        case 47: _t->toggle_mute6(); break;
        case 48: _t->toggle_mute7(); break;
        case 49: _t->toggle_store_sta0(); break;
        case 50: _t->toggle_store_sta1(); break;
        case 51: _t->toggle_store_sta2(); break;
        case 52: _t->toggle_store_sta3(); break;
        case 53: _t->toggle_store_sta4(); break;
        case 54: _t->toggle_store_sta5(); break;
        case 55: _t->toggle_store_sta6(); break;
        case 56: _t->toggle_store_sta7(); break;
        case 57: _t->select_Sds0(); break;
        case 58: _t->select_Sds1(); break;
        case 59: _t->select_Sds2(); break;
        case 60: _t->select_Sds3(); break;
        case 61: _t->File_Director(); break;
        case 62: _t->Spectrum_Dialog(); break;
        case 63: _t->Save_Config(); break;
        case 64: _t->toggle_Mute(); break;
        case 65: _t->connect_fmo(); break;
        case 66: _t->connect_vco(); break;
        case 67: _t->main_adsr_sustain(); break;
        case 68: _t->pB_Debug_clicked(); break;
        case 69: _t->wavfile_selected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 70: _t->pB_oscgroup_clicked(); break;
        case 71: _t->pB_Wavedisplay_clicked(); break;
        case 72: _t->pB_fftmode_clicked(); break;
        case 73: _t->hs_hall_effect_value_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
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
        if (_id < 74)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 74;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 74)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 74;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
