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
    char stringdata0[1115];
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
QT_MOC_LITERAL(3, 24, 28), // "dial_soft_freq_value_changed"
QT_MOC_LITERAL(4, 53, 15), // "cB_Beat_per_sec"
QT_MOC_LITERAL(5, 69, 22), // "dial_PMW_value_changed"
QT_MOC_LITERAL(6, 92, 24), // "dial_decay_value_changed"
QT_MOC_LITERAL(7, 117, 17), // "get_record_status"
QT_MOC_LITERAL(8, 135, 12), // "MAIN_slot_Hz"
QT_MOC_LITERAL(9, 148, 16), // "MAIN_slot_volume"
QT_MOC_LITERAL(10, 165, 15), // "VCO_slot_volume"
QT_MOC_LITERAL(11, 181, 21), // "Slider_FMO_Hz_changed"
QT_MOC_LITERAL(12, 203, 21), // "Slider_VCO_Hz_changed"
QT_MOC_LITERAL(13, 225, 15), // "FMO_slot_volume"
QT_MOC_LITERAL(14, 241, 13), // "waveform_slot"
QT_MOC_LITERAL(15, 255, 8), // "uint8_t*"
QT_MOC_LITERAL(16, 264, 7), // "uint8_t"
QT_MOC_LITERAL(17, 272, 7), // "QLabel*"
QT_MOC_LITERAL(18, 280, 18), // "Main_Waveform_slot"
QT_MOC_LITERAL(19, 299, 17), // "VCO_Waveform_slot"
QT_MOC_LITERAL(20, 317, 17), // "FMO_Waveform_slot"
QT_MOC_LITERAL(21, 335, 15), // "Controller_Exit"
QT_MOC_LITERAL(22, 351, 8), // "GUI_Exit"
QT_MOC_LITERAL(23, 360, 10), // "Audio_Exit"
QT_MOC_LITERAL(24, 371, 17), // "start_synthesizer"
QT_MOC_LITERAL(25, 389, 15), // "start_audio_srv"
QT_MOC_LITERAL(26, 405, 14), // "start_composer"
QT_MOC_LITERAL(27, 420, 10), // "set_mode_f"
QT_MOC_LITERAL(28, 431, 10), // "set_mode_v"
QT_MOC_LITERAL(29, 442, 10), // "set_mode_o"
QT_MOC_LITERAL(30, 453, 7), // "Sl_mix0"
QT_MOC_LITERAL(31, 461, 7), // "Sl_mix1"
QT_MOC_LITERAL(32, 469, 7), // "Sl_mix2"
QT_MOC_LITERAL(33, 477, 7), // "Sl_mix3"
QT_MOC_LITERAL(34, 485, 7), // "Sl_mix4"
QT_MOC_LITERAL(35, 493, 7), // "Sl_mix5"
QT_MOC_LITERAL(36, 501, 7), // "Sl_mix6"
QT_MOC_LITERAL(37, 509, 7), // "Sl_mix7"
QT_MOC_LITERAL(38, 517, 22), // "slot_dial_ramp_up_down"
QT_MOC_LITERAL(39, 540, 12), // "memory_clear"
QT_MOC_LITERAL(40, 553, 10), // "SaveRecord"
QT_MOC_LITERAL(41, 564, 17), // "read_polygon_data"
QT_MOC_LITERAL(42, 582, 11), // "Clear_Banks"
QT_MOC_LITERAL(43, 594, 12), // "toggle_mute0"
QT_MOC_LITERAL(44, 607, 12), // "toggle_mute1"
QT_MOC_LITERAL(45, 620, 12), // "toggle_mute2"
QT_MOC_LITERAL(46, 633, 12), // "toggle_mute3"
QT_MOC_LITERAL(47, 646, 12), // "toggle_mute4"
QT_MOC_LITERAL(48, 659, 12), // "toggle_mute5"
QT_MOC_LITERAL(49, 672, 12), // "toggle_mute6"
QT_MOC_LITERAL(50, 685, 12), // "toggle_mute7"
QT_MOC_LITERAL(51, 698, 17), // "toggle_store_sta0"
QT_MOC_LITERAL(52, 716, 17), // "toggle_store_sta1"
QT_MOC_LITERAL(53, 734, 17), // "toggle_store_sta2"
QT_MOC_LITERAL(54, 752, 17), // "toggle_store_sta3"
QT_MOC_LITERAL(55, 770, 17), // "toggle_store_sta4"
QT_MOC_LITERAL(56, 788, 17), // "toggle_store_sta5"
QT_MOC_LITERAL(57, 806, 17), // "toggle_store_sta6"
QT_MOC_LITERAL(58, 824, 17), // "toggle_store_sta7"
QT_MOC_LITERAL(59, 842, 11), // "select_Sds0"
QT_MOC_LITERAL(60, 854, 11), // "select_Sds1"
QT_MOC_LITERAL(61, 866, 11), // "select_Sds2"
QT_MOC_LITERAL(62, 878, 11), // "select_Sds3"
QT_MOC_LITERAL(63, 890, 13), // "File_Director"
QT_MOC_LITERAL(64, 904, 15), // "Spectrum_Dialog"
QT_MOC_LITERAL(65, 920, 11), // "Save_Config"
QT_MOC_LITERAL(66, 932, 11), // "toggle_Mute"
QT_MOC_LITERAL(67, 944, 11), // "connect_fmo"
QT_MOC_LITERAL(68, 956, 11), // "connect_vco"
QT_MOC_LITERAL(69, 968, 17), // "main_adsr_sustain"
QT_MOC_LITERAL(70, 986, 16), // "pB_Debug_clicked"
QT_MOC_LITERAL(71, 1003, 16), // "wavfile_selected"
QT_MOC_LITERAL(72, 1020, 3), // "arg"
QT_MOC_LITERAL(73, 1024, 19), // "pB_oscgroup_clicked"
QT_MOC_LITERAL(74, 1044, 22), // "pB_Wavedisplay_clicked"
QT_MOC_LITERAL(75, 1067, 18), // "pB_fftmode_clicked"
QT_MOC_LITERAL(76, 1086, 28) // "hs_hall_effect_value_changed"

    },
    "MainWindow\0Rtsp_Dialog\0\0"
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
      71,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  369,    2, 0x08 /* Private */,
       3,    0,  370,    2, 0x08 /* Private */,
       4,    1,  371,    2, 0x08 /* Private */,
       5,    0,  374,    2, 0x08 /* Private */,
       6,    0,  375,    2, 0x08 /* Private */,
       7,    0,  376,    2, 0x08 /* Private */,
       8,    0,  377,    2, 0x08 /* Private */,
       9,    0,  378,    2, 0x08 /* Private */,
      10,    0,  379,    2, 0x08 /* Private */,
      11,    1,  380,    2, 0x08 /* Private */,
      12,    1,  383,    2, 0x08 /* Private */,
      13,    0,  386,    2, 0x08 /* Private */,
      14,    5,  387,    2, 0x08 /* Private */,
      18,    1,  398,    2, 0x08 /* Private */,
      19,    1,  401,    2, 0x08 /* Private */,
      20,    1,  404,    2, 0x08 /* Private */,
      21,    0,  407,    2, 0x08 /* Private */,
      22,    0,  408,    2, 0x08 /* Private */,
      23,    0,  409,    2, 0x08 /* Private */,
      24,    0,  410,    2, 0x08 /* Private */,
      25,    0,  411,    2, 0x08 /* Private */,
      26,    0,  412,    2, 0x08 /* Private */,
      27,    0,  413,    2, 0x08 /* Private */,
      28,    0,  414,    2, 0x08 /* Private */,
      29,    0,  415,    2, 0x08 /* Private */,
      30,    1,  416,    2, 0x08 /* Private */,
      31,    1,  419,    2, 0x08 /* Private */,
      32,    1,  422,    2, 0x08 /* Private */,
      33,    1,  425,    2, 0x08 /* Private */,
      34,    1,  428,    2, 0x08 /* Private */,
      35,    1,  431,    2, 0x08 /* Private */,
      36,    1,  434,    2, 0x08 /* Private */,
      37,    1,  437,    2, 0x08 /* Private */,
      38,    0,  440,    2, 0x08 /* Private */,
      39,    0,  441,    2, 0x08 /* Private */,
      40,    0,  442,    2, 0x08 /* Private */,
      41,    0,  443,    2, 0x08 /* Private */,
      42,    0,  444,    2, 0x08 /* Private */,
      43,    0,  445,    2, 0x08 /* Private */,
      44,    0,  446,    2, 0x08 /* Private */,
      45,    0,  447,    2, 0x08 /* Private */,
      46,    0,  448,    2, 0x08 /* Private */,
      47,    0,  449,    2, 0x08 /* Private */,
      48,    0,  450,    2, 0x08 /* Private */,
      49,    0,  451,    2, 0x08 /* Private */,
      50,    0,  452,    2, 0x08 /* Private */,
      51,    0,  453,    2, 0x08 /* Private */,
      52,    0,  454,    2, 0x08 /* Private */,
      53,    0,  455,    2, 0x08 /* Private */,
      54,    0,  456,    2, 0x08 /* Private */,
      55,    0,  457,    2, 0x08 /* Private */,
      56,    0,  458,    2, 0x08 /* Private */,
      57,    0,  459,    2, 0x08 /* Private */,
      58,    0,  460,    2, 0x08 /* Private */,
      59,    0,  461,    2, 0x08 /* Private */,
      60,    0,  462,    2, 0x08 /* Private */,
      61,    0,  463,    2, 0x08 /* Private */,
      62,    0,  464,    2, 0x08 /* Private */,
      63,    0,  465,    2, 0x08 /* Private */,
      64,    0,  466,    2, 0x08 /* Private */,
      65,    0,  467,    2, 0x08 /* Private */,
      66,    0,  468,    2, 0x08 /* Private */,
      67,    0,  469,    2, 0x08 /* Private */,
      68,    0,  470,    2, 0x08 /* Private */,
      69,    0,  471,    2, 0x08 /* Private */,
      70,    0,  472,    2, 0x08 /* Private */,
      71,    1,  473,    2, 0x08 /* Private */,
      73,    0,  476,    2, 0x08 /* Private */,
      74,    0,  477,    2, 0x08 /* Private */,
      75,    0,  478,    2, 0x08 /* Private */,
      76,    1,  479,    2, 0x08 /* Private */,

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
        case 0: _t->Rtsp_Dialog(); break;
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
        case 35: _t->SaveRecord(); break;
        case 36: _t->read_polygon_data(); break;
        case 37: _t->Clear_Banks(); break;
        case 38: _t->toggle_mute0(); break;
        case 39: _t->toggle_mute1(); break;
        case 40: _t->toggle_mute2(); break;
        case 41: _t->toggle_mute3(); break;
        case 42: _t->toggle_mute4(); break;
        case 43: _t->toggle_mute5(); break;
        case 44: _t->toggle_mute6(); break;
        case 45: _t->toggle_mute7(); break;
        case 46: _t->toggle_store_sta0(); break;
        case 47: _t->toggle_store_sta1(); break;
        case 48: _t->toggle_store_sta2(); break;
        case 49: _t->toggle_store_sta3(); break;
        case 50: _t->toggle_store_sta4(); break;
        case 51: _t->toggle_store_sta5(); break;
        case 52: _t->toggle_store_sta6(); break;
        case 53: _t->toggle_store_sta7(); break;
        case 54: _t->select_Sds0(); break;
        case 55: _t->select_Sds1(); break;
        case 56: _t->select_Sds2(); break;
        case 57: _t->select_Sds3(); break;
        case 58: _t->File_Director(); break;
        case 59: _t->Spectrum_Dialog(); break;
        case 60: _t->Save_Config(); break;
        case 61: _t->toggle_Mute(); break;
        case 62: _t->connect_fmo(); break;
        case 63: _t->connect_vco(); break;
        case 64: _t->main_adsr_sustain(); break;
        case 65: _t->pB_Debug_clicked(); break;
        case 66: _t->wavfile_selected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 67: _t->pB_oscgroup_clicked(); break;
        case 68: _t->pB_Wavedisplay_clicked(); break;
        case 69: _t->pB_fftmode_clicked(); break;
        case 70: _t->hs_hall_effect_value_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
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
        if (_id < 71)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 71;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 71)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 71;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
