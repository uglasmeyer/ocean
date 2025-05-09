/****************************************************************************
** Meta object code from reading C++ file 'Mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "update_CB_external",
    "",
    "updateColorButtons",
    "cB_Capture",
    "str",
    "Rtsp_Dialog",
    "SDS_Dialog",
    "slideFrq",
    "cB_Beat_per_sec",
    "dial_PMW_value_changed",
    "adsr_attack",
    "mixer_balance",
    "get_record_status",
    "Main_slot_volume",
    "VCO_slot_volume",
    "FMO_slot_volume",
    "Slider_OSC_Freq",
    "Slider_FMO_Freq",
    "Slider_VCO_Freq",
    "Slider_VCO_Adjust",
    "Slider_FMO_Adjust",
    "Main_Waveform_slot",
    "VCO_Waveform_slot",
    "FMO_Waveform_slot",
    "exit_synthesizer",
    "GUI_Exit",
    "start_synthesizer",
    "start_audio_srv",
    "start_composer",
    "start_keyboard",
    "connect_oscf",
    "connect_oscv",
    "connect_fmo",
    "connect_vco",
    "Sl_mix0",
    "Sl_mix1",
    "Sl_mix2",
    "Sl_mix3",
    "Sl_mix4",
    "Sl_mix5",
    "Sl_mix6",
    "Sl_mix7",
    "slideVol",
    "memory_clear",
    "SaveRecord",
    "read_polygon_data",
    "Clear_Banks",
    "toggle_mute0",
    "toggle_mute1",
    "toggle_mute2",
    "toggle_mute3",
    "toggle_mute4",
    "toggle_mute5",
    "toggle_mute6",
    "toggle_mute7",
    "toggle_store_sta0",
    "toggle_store_sta1",
    "toggle_store_sta2",
    "toggle_store_sta3",
    "toggle_store_sta4",
    "toggle_store_sta5",
    "toggle_store_sta6",
    "toggle_store_sta7",
    "select_Sds0",
    "select_Sds1",
    "select_Sds2",
    "select_Sds3",
    "CombineFreq",
    "File_Director",
    "Spectrum_Dialog",
    "Save_Config",
    "toggle_Mute",
    "adsr_decay",
    "pB_Debug_clicked",
    "wavfile_selected",
    "arg",
    "pB_oscgroup_clicked",
    "pB_Wavedisplay_clicked",
    "pB_fftmode_clicked",
    "adsr_hall"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[162];
    char stringdata0[11];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[19];
    char stringdata4[11];
    char stringdata5[4];
    char stringdata6[12];
    char stringdata7[11];
    char stringdata8[9];
    char stringdata9[16];
    char stringdata10[23];
    char stringdata11[12];
    char stringdata12[14];
    char stringdata13[18];
    char stringdata14[17];
    char stringdata15[16];
    char stringdata16[16];
    char stringdata17[16];
    char stringdata18[16];
    char stringdata19[16];
    char stringdata20[18];
    char stringdata21[18];
    char stringdata22[19];
    char stringdata23[18];
    char stringdata24[18];
    char stringdata25[17];
    char stringdata26[9];
    char stringdata27[18];
    char stringdata28[16];
    char stringdata29[15];
    char stringdata30[15];
    char stringdata31[13];
    char stringdata32[13];
    char stringdata33[12];
    char stringdata34[12];
    char stringdata35[8];
    char stringdata36[8];
    char stringdata37[8];
    char stringdata38[8];
    char stringdata39[8];
    char stringdata40[8];
    char stringdata41[8];
    char stringdata42[8];
    char stringdata43[9];
    char stringdata44[13];
    char stringdata45[11];
    char stringdata46[18];
    char stringdata47[12];
    char stringdata48[13];
    char stringdata49[13];
    char stringdata50[13];
    char stringdata51[13];
    char stringdata52[13];
    char stringdata53[13];
    char stringdata54[13];
    char stringdata55[13];
    char stringdata56[18];
    char stringdata57[18];
    char stringdata58[18];
    char stringdata59[18];
    char stringdata60[18];
    char stringdata61[18];
    char stringdata62[18];
    char stringdata63[18];
    char stringdata64[12];
    char stringdata65[12];
    char stringdata66[12];
    char stringdata67[12];
    char stringdata68[12];
    char stringdata69[14];
    char stringdata70[16];
    char stringdata71[12];
    char stringdata72[12];
    char stringdata73[11];
    char stringdata74[17];
    char stringdata75[17];
    char stringdata76[4];
    char stringdata77[20];
    char stringdata78[23];
    char stringdata79[19];
    char stringdata80[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 18),  // "update_CB_external"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 18),  // "updateColorButtons"
        QT_MOC_LITERAL(50, 10),  // "cB_Capture"
        QT_MOC_LITERAL(61, 3),  // "str"
        QT_MOC_LITERAL(65, 11),  // "Rtsp_Dialog"
        QT_MOC_LITERAL(77, 10),  // "SDS_Dialog"
        QT_MOC_LITERAL(88, 8),  // "slideFrq"
        QT_MOC_LITERAL(97, 15),  // "cB_Beat_per_sec"
        QT_MOC_LITERAL(113, 22),  // "dial_PMW_value_changed"
        QT_MOC_LITERAL(136, 11),  // "adsr_attack"
        QT_MOC_LITERAL(148, 13),  // "mixer_balance"
        QT_MOC_LITERAL(162, 17),  // "get_record_status"
        QT_MOC_LITERAL(180, 16),  // "Main_slot_volume"
        QT_MOC_LITERAL(197, 15),  // "VCO_slot_volume"
        QT_MOC_LITERAL(213, 15),  // "FMO_slot_volume"
        QT_MOC_LITERAL(229, 15),  // "Slider_OSC_Freq"
        QT_MOC_LITERAL(245, 15),  // "Slider_FMO_Freq"
        QT_MOC_LITERAL(261, 15),  // "Slider_VCO_Freq"
        QT_MOC_LITERAL(277, 17),  // "Slider_VCO_Adjust"
        QT_MOC_LITERAL(295, 17),  // "Slider_FMO_Adjust"
        QT_MOC_LITERAL(313, 18),  // "Main_Waveform_slot"
        QT_MOC_LITERAL(332, 17),  // "VCO_Waveform_slot"
        QT_MOC_LITERAL(350, 17),  // "FMO_Waveform_slot"
        QT_MOC_LITERAL(368, 16),  // "exit_synthesizer"
        QT_MOC_LITERAL(385, 8),  // "GUI_Exit"
        QT_MOC_LITERAL(394, 17),  // "start_synthesizer"
        QT_MOC_LITERAL(412, 15),  // "start_audio_srv"
        QT_MOC_LITERAL(428, 14),  // "start_composer"
        QT_MOC_LITERAL(443, 14),  // "start_keyboard"
        QT_MOC_LITERAL(458, 12),  // "connect_oscf"
        QT_MOC_LITERAL(471, 12),  // "connect_oscv"
        QT_MOC_LITERAL(484, 11),  // "connect_fmo"
        QT_MOC_LITERAL(496, 11),  // "connect_vco"
        QT_MOC_LITERAL(508, 7),  // "Sl_mix0"
        QT_MOC_LITERAL(516, 7),  // "Sl_mix1"
        QT_MOC_LITERAL(524, 7),  // "Sl_mix2"
        QT_MOC_LITERAL(532, 7),  // "Sl_mix3"
        QT_MOC_LITERAL(540, 7),  // "Sl_mix4"
        QT_MOC_LITERAL(548, 7),  // "Sl_mix5"
        QT_MOC_LITERAL(556, 7),  // "Sl_mix6"
        QT_MOC_LITERAL(564, 7),  // "Sl_mix7"
        QT_MOC_LITERAL(572, 8),  // "slideVol"
        QT_MOC_LITERAL(581, 12),  // "memory_clear"
        QT_MOC_LITERAL(594, 10),  // "SaveRecord"
        QT_MOC_LITERAL(605, 17),  // "read_polygon_data"
        QT_MOC_LITERAL(623, 11),  // "Clear_Banks"
        QT_MOC_LITERAL(635, 12),  // "toggle_mute0"
        QT_MOC_LITERAL(648, 12),  // "toggle_mute1"
        QT_MOC_LITERAL(661, 12),  // "toggle_mute2"
        QT_MOC_LITERAL(674, 12),  // "toggle_mute3"
        QT_MOC_LITERAL(687, 12),  // "toggle_mute4"
        QT_MOC_LITERAL(700, 12),  // "toggle_mute5"
        QT_MOC_LITERAL(713, 12),  // "toggle_mute6"
        QT_MOC_LITERAL(726, 12),  // "toggle_mute7"
        QT_MOC_LITERAL(739, 17),  // "toggle_store_sta0"
        QT_MOC_LITERAL(757, 17),  // "toggle_store_sta1"
        QT_MOC_LITERAL(775, 17),  // "toggle_store_sta2"
        QT_MOC_LITERAL(793, 17),  // "toggle_store_sta3"
        QT_MOC_LITERAL(811, 17),  // "toggle_store_sta4"
        QT_MOC_LITERAL(829, 17),  // "toggle_store_sta5"
        QT_MOC_LITERAL(847, 17),  // "toggle_store_sta6"
        QT_MOC_LITERAL(865, 17),  // "toggle_store_sta7"
        QT_MOC_LITERAL(883, 11),  // "select_Sds0"
        QT_MOC_LITERAL(895, 11),  // "select_Sds1"
        QT_MOC_LITERAL(907, 11),  // "select_Sds2"
        QT_MOC_LITERAL(919, 11),  // "select_Sds3"
        QT_MOC_LITERAL(931, 11),  // "CombineFreq"
        QT_MOC_LITERAL(943, 13),  // "File_Director"
        QT_MOC_LITERAL(957, 15),  // "Spectrum_Dialog"
        QT_MOC_LITERAL(973, 11),  // "Save_Config"
        QT_MOC_LITERAL(985, 11),  // "toggle_Mute"
        QT_MOC_LITERAL(997, 10),  // "adsr_decay"
        QT_MOC_LITERAL(1008, 16),  // "pB_Debug_clicked"
        QT_MOC_LITERAL(1025, 16),  // "wavfile_selected"
        QT_MOC_LITERAL(1042, 3),  // "arg"
        QT_MOC_LITERAL(1046, 19),  // "pB_oscgroup_clicked"
        QT_MOC_LITERAL(1066, 22),  // "pB_Wavedisplay_clicked"
        QT_MOC_LITERAL(1089, 18),  // "pB_fftmode_clicked"
        QT_MOC_LITERAL(1108, 9)   // "adsr_hall"
    },
    "MainWindow",
    "update_CB_external",
    "",
    "updateColorButtons",
    "cB_Capture",
    "str",
    "Rtsp_Dialog",
    "SDS_Dialog",
    "slideFrq",
    "cB_Beat_per_sec",
    "dial_PMW_value_changed",
    "adsr_attack",
    "mixer_balance",
    "get_record_status",
    "Main_slot_volume",
    "VCO_slot_volume",
    "FMO_slot_volume",
    "Slider_OSC_Freq",
    "Slider_FMO_Freq",
    "Slider_VCO_Freq",
    "Slider_VCO_Adjust",
    "Slider_FMO_Adjust",
    "Main_Waveform_slot",
    "VCO_Waveform_slot",
    "FMO_Waveform_slot",
    "exit_synthesizer",
    "GUI_Exit",
    "start_synthesizer",
    "start_audio_srv",
    "start_composer",
    "start_keyboard",
    "connect_oscf",
    "connect_oscv",
    "connect_fmo",
    "connect_vco",
    "Sl_mix0",
    "Sl_mix1",
    "Sl_mix2",
    "Sl_mix3",
    "Sl_mix4",
    "Sl_mix5",
    "Sl_mix6",
    "Sl_mix7",
    "slideVol",
    "memory_clear",
    "SaveRecord",
    "read_polygon_data",
    "Clear_Banks",
    "toggle_mute0",
    "toggle_mute1",
    "toggle_mute2",
    "toggle_mute3",
    "toggle_mute4",
    "toggle_mute5",
    "toggle_mute6",
    "toggle_mute7",
    "toggle_store_sta0",
    "toggle_store_sta1",
    "toggle_store_sta2",
    "toggle_store_sta3",
    "toggle_store_sta4",
    "toggle_store_sta5",
    "toggle_store_sta6",
    "toggle_store_sta7",
    "select_Sds0",
    "select_Sds1",
    "select_Sds2",
    "select_Sds3",
    "CombineFreq",
    "File_Director",
    "Spectrum_Dialog",
    "Save_Config",
    "toggle_Mute",
    "adsr_decay",
    "pB_Debug_clicked",
    "wavfile_selected",
    "arg",
    "pB_oscgroup_clicked",
    "pB_Wavedisplay_clicked",
    "pB_fftmode_clicked",
    "adsr_hall"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      77,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  476,    2, 0x0a,    1 /* Public */,
       3,    0,  477,    2, 0x0a,    2 /* Public */,
       4,    1,  478,    2, 0x08,    3 /* Private */,
       6,    0,  481,    2, 0x08,    5 /* Private */,
       7,    0,  482,    2, 0x08,    6 /* Private */,
       8,    1,  483,    2, 0x08,    7 /* Private */,
       9,    1,  486,    2, 0x08,    9 /* Private */,
      10,    0,  489,    2, 0x08,   11 /* Private */,
      11,    0,  490,    2, 0x08,   12 /* Private */,
      12,    0,  491,    2, 0x08,   13 /* Private */,
      13,    0,  492,    2, 0x08,   14 /* Private */,
      14,    0,  493,    2, 0x08,   15 /* Private */,
      15,    0,  494,    2, 0x08,   16 /* Private */,
      16,    0,  495,    2, 0x08,   17 /* Private */,
      17,    1,  496,    2, 0x08,   18 /* Private */,
      18,    1,  499,    2, 0x08,   20 /* Private */,
      19,    1,  502,    2, 0x08,   22 /* Private */,
      20,    1,  505,    2, 0x08,   24 /* Private */,
      21,    1,  508,    2, 0x08,   26 /* Private */,
      22,    1,  511,    2, 0x08,   28 /* Private */,
      23,    1,  514,    2, 0x08,   30 /* Private */,
      24,    1,  517,    2, 0x08,   32 /* Private */,
      25,    0,  520,    2, 0x08,   34 /* Private */,
      26,    0,  521,    2, 0x08,   35 /* Private */,
      27,    0,  522,    2, 0x08,   36 /* Private */,
      28,    0,  523,    2, 0x08,   37 /* Private */,
      29,    0,  524,    2, 0x08,   38 /* Private */,
      30,    0,  525,    2, 0x08,   39 /* Private */,
      31,    1,  526,    2, 0x08,   40 /* Private */,
      32,    1,  529,    2, 0x08,   42 /* Private */,
      33,    1,  532,    2, 0x08,   44 /* Private */,
      34,    1,  535,    2, 0x08,   46 /* Private */,
      35,    1,  538,    2, 0x08,   48 /* Private */,
      36,    1,  541,    2, 0x08,   50 /* Private */,
      37,    1,  544,    2, 0x08,   52 /* Private */,
      38,    1,  547,    2, 0x08,   54 /* Private */,
      39,    1,  550,    2, 0x08,   56 /* Private */,
      40,    1,  553,    2, 0x08,   58 /* Private */,
      41,    1,  556,    2, 0x08,   60 /* Private */,
      42,    1,  559,    2, 0x08,   62 /* Private */,
      43,    1,  562,    2, 0x08,   64 /* Private */,
      44,    0,  565,    2, 0x08,   66 /* Private */,
      45,    0,  566,    2, 0x08,   67 /* Private */,
      46,    0,  567,    2, 0x08,   68 /* Private */,
      47,    0,  568,    2, 0x08,   69 /* Private */,
      48,    0,  569,    2, 0x08,   70 /* Private */,
      49,    0,  570,    2, 0x08,   71 /* Private */,
      50,    0,  571,    2, 0x08,   72 /* Private */,
      51,    0,  572,    2, 0x08,   73 /* Private */,
      52,    0,  573,    2, 0x08,   74 /* Private */,
      53,    0,  574,    2, 0x08,   75 /* Private */,
      54,    0,  575,    2, 0x08,   76 /* Private */,
      55,    0,  576,    2, 0x08,   77 /* Private */,
      56,    0,  577,    2, 0x08,   78 /* Private */,
      57,    0,  578,    2, 0x08,   79 /* Private */,
      58,    0,  579,    2, 0x08,   80 /* Private */,
      59,    0,  580,    2, 0x08,   81 /* Private */,
      60,    0,  581,    2, 0x08,   82 /* Private */,
      61,    0,  582,    2, 0x08,   83 /* Private */,
      62,    0,  583,    2, 0x08,   84 /* Private */,
      63,    0,  584,    2, 0x08,   85 /* Private */,
      64,    0,  585,    2, 0x08,   86 /* Private */,
      65,    0,  586,    2, 0x08,   87 /* Private */,
      66,    0,  587,    2, 0x08,   88 /* Private */,
      67,    0,  588,    2, 0x08,   89 /* Private */,
      68,    0,  589,    2, 0x08,   90 /* Private */,
      69,    0,  590,    2, 0x08,   91 /* Private */,
      70,    0,  591,    2, 0x08,   92 /* Private */,
      71,    0,  592,    2, 0x08,   93 /* Private */,
      72,    0,  593,    2, 0x08,   94 /* Private */,
      73,    0,  594,    2, 0x08,   95 /* Private */,
      74,    0,  595,    2, 0x08,   96 /* Private */,
      75,    1,  596,    2, 0x08,   97 /* Private */,
      77,    0,  599,    2, 0x08,   99 /* Private */,
      78,    0,  600,    2, 0x08,  100 /* Private */,
      79,    0,  601,    2, 0x08,  101 /* Private */,
      80,    0,  602,    2, 0x08,  102 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
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
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
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
    QMetaType::Void, QMetaType::QString,   76,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'update_CB_external'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateColorButtons'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cB_Capture'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'Rtsp_Dialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SDS_Dialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slideFrq'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cB_Beat_per_sec'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'dial_PMW_value_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'adsr_attack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'mixer_balance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'get_record_status'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Main_slot_volume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'VCO_slot_volume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'FMO_slot_volume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Slider_OSC_Freq'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Slider_FMO_Freq'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Slider_VCO_Freq'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Slider_VCO_Adjust'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Slider_FMO_Adjust'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Main_Waveform_slot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'VCO_Waveform_slot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'FMO_Waveform_slot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'exit_synthesizer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'GUI_Exit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'start_synthesizer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'start_audio_srv'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'start_composer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'start_keyboard'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connect_oscf'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'connect_oscv'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'connect_fmo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'connect_vco'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'Sl_mix0'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix4'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix5'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix6'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Sl_mix7'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'slideVol'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'memory_clear'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SaveRecord'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'read_polygon_data'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Clear_Banks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute0'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute4'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute5'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute6'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_mute7'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta0'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta4'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta5'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta6'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_store_sta7'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'select_Sds0'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'select_Sds1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'select_Sds2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'select_Sds3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'CombineFreq'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'File_Director'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Spectrum_Dialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Save_Config'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle_Mute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'adsr_decay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pB_Debug_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'wavfile_selected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'pB_oscgroup_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pB_Wavedisplay_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pB_fftmode_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'adsr_hall'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->update_CB_external(); break;
        case 1: _t->updateColorButtons(); break;
        case 2: _t->cB_Capture((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->Rtsp_Dialog(); break;
        case 4: _t->SDS_Dialog(); break;
        case 5: _t->slideFrq((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->cB_Beat_per_sec((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->dial_PMW_value_changed(); break;
        case 8: _t->adsr_attack(); break;
        case 9: _t->mixer_balance(); break;
        case 10: _t->get_record_status(); break;
        case 11: _t->Main_slot_volume(); break;
        case 12: _t->VCO_slot_volume(); break;
        case 13: _t->FMO_slot_volume(); break;
        case 14: _t->Slider_OSC_Freq((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->Slider_FMO_Freq((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->Slider_VCO_Freq((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->Slider_VCO_Adjust((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->Slider_FMO_Adjust((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->Main_Waveform_slot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->VCO_Waveform_slot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->FMO_Waveform_slot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->exit_synthesizer(); break;
        case 23: _t->GUI_Exit(); break;
        case 24: _t->start_synthesizer(); break;
        case 25: _t->start_audio_srv(); break;
        case 26: _t->start_composer(); break;
        case 27: _t->start_keyboard(); break;
        case 28: _t->connect_oscf((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 29: _t->connect_oscv((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 30: _t->connect_fmo((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 31: _t->connect_vco((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 32: _t->Sl_mix0((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->Sl_mix1((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->Sl_mix2((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->Sl_mix3((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 36: _t->Sl_mix4((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 37: _t->Sl_mix5((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 38: _t->Sl_mix6((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->Sl_mix7((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 40: _t->slideVol((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 41: _t->memory_clear(); break;
        case 42: _t->SaveRecord(); break;
        case 43: _t->read_polygon_data(); break;
        case 44: _t->Clear_Banks(); break;
        case 45: _t->toggle_mute0(); break;
        case 46: _t->toggle_mute1(); break;
        case 47: _t->toggle_mute2(); break;
        case 48: _t->toggle_mute3(); break;
        case 49: _t->toggle_mute4(); break;
        case 50: _t->toggle_mute5(); break;
        case 51: _t->toggle_mute6(); break;
        case 52: _t->toggle_mute7(); break;
        case 53: _t->toggle_store_sta0(); break;
        case 54: _t->toggle_store_sta1(); break;
        case 55: _t->toggle_store_sta2(); break;
        case 56: _t->toggle_store_sta3(); break;
        case 57: _t->toggle_store_sta4(); break;
        case 58: _t->toggle_store_sta5(); break;
        case 59: _t->toggle_store_sta6(); break;
        case 60: _t->toggle_store_sta7(); break;
        case 61: _t->select_Sds0(); break;
        case 62: _t->select_Sds1(); break;
        case 63: _t->select_Sds2(); break;
        case 64: _t->select_Sds3(); break;
        case 65: _t->CombineFreq(); break;
        case 66: _t->File_Director(); break;
        case 67: _t->Spectrum_Dialog(); break;
        case 68: _t->Save_Config(); break;
        case 69: _t->toggle_Mute(); break;
        case 70: _t->adsr_decay(); break;
        case 71: _t->pB_Debug_clicked(); break;
        case 72: _t->wavfile_selected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 73: _t->pB_oscgroup_clicked(); break;
        case 74: _t->pB_Wavedisplay_clicked(); break;
        case 75: _t->pB_fftmode_clicked(); break;
        case 76: _t->adsr_hall(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Logfacility_class"))
        return static_cast< Logfacility_class*>(this);
    if (!strcmp(_clname, "osc_struct"))
        return static_cast< osc_struct*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 77)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 77;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 77)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 77;
    }
    return _id;
}
QT_WARNING_POP
