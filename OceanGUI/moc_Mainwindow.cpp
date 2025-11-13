/****************************************************************************
** Meta object code from reading C++ file 'Mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/Mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "update_CB_external",
        "",
        "updateColorButtons",
        "Capture",
        "str",
        "Rtsp_Dialog",
        "SDS_Dialog",
        "slideFrq",
        "cB_Beat_per_sec",
        "dial_PMW_value_changed",
        "mixer_balance",
        "chord_delay",
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
        "APPID",
        "GUI_Exit",
        "start_synthesizer",
        "start_audio_srv",
        "start_composer",
        "start_keyboard",
        "connect_oscf",
        "connect_oscv",
        "connect_fmov",
        "connect_vcov",
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
        "setStaPlay",
        "uint8_t",
        "id",
        "setStaPlay0",
        "setStaPlay1",
        "setStaPlay2",
        "setStaPlay3",
        "setStaPlay4",
        "setStaPlay5",
        "setStaPlay6",
        "setStaPlay7",
        "setStaStored",
        "staId",
        "setStaStored0",
        "setStaStored1",
        "setStaStored2",
        "setStaStored3",
        "setStaStored4",
        "setStaStored5",
        "setStaStored6",
        "setStaStored7",
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
        "ADSR_Dialog",
        "Save_Config",
        "toggle_Mute",
        "adsr_hall",
        "pB_Debug_clicked",
        "wavfile_selected",
        "arg",
        "pB_oscgroup_clicked",
        "pB_Wavedisplay_clicked",
        "pB_fftmode_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'update_CB_external'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateColorButtons'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'Capture'
        QtMocHelpers::SlotData<void(QString)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Slot 'Rtsp_Dialog'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'SDS_Dialog'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slideFrq'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'cB_Beat_per_sec'
        QtMocHelpers::SlotData<void(int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'dial_PMW_value_changed'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'mixer_balance'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'chord_delay'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'get_record_status'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Main_slot_volume'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'VCO_slot_volume'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'FMO_slot_volume'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Slider_OSC_Freq'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Slider_FMO_Freq'
        QtMocHelpers::SlotData<void(int)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Slider_VCO_Freq'
        QtMocHelpers::SlotData<void(int)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Slider_VCO_Adjust'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Slider_FMO_Adjust'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Main_Waveform_slot'
        QtMocHelpers::SlotData<void(int)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'VCO_Waveform_slot'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'FMO_Waveform_slot'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'exit_synthesizer'
        QtMocHelpers::SlotData<void(APPID)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 26, 2 },
        }}),
        // Slot 'GUI_Exit'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'start_synthesizer'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'start_audio_srv'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'start_composer'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'start_keyboard'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'connect_oscf'
        QtMocHelpers::SlotData<void(bool)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'connect_oscv'
        QtMocHelpers::SlotData<void(bool)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'connect_fmov'
        QtMocHelpers::SlotData<void(bool)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'connect_vcov'
        QtMocHelpers::SlotData<void(bool)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'Sl_mix0'
        QtMocHelpers::SlotData<void(int)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix1'
        QtMocHelpers::SlotData<void(int)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix2'
        QtMocHelpers::SlotData<void(int)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix3'
        QtMocHelpers::SlotData<void(int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix4'
        QtMocHelpers::SlotData<void(int)>(40, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix5'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix6'
        QtMocHelpers::SlotData<void(int)>(42, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'Sl_mix7'
        QtMocHelpers::SlotData<void(int)>(43, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'slideVol'
        QtMocHelpers::SlotData<void(int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'memory_clear'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'SaveRecord'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'read_polygon_data'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Clear_Banks'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay'
        QtMocHelpers::SlotData<void(uint8_t)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 50, 51 },
        }}),
        // Slot 'setStaPlay0'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay1'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay2'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay3'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay4'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay5'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay6'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaPlay7'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored'
        QtMocHelpers::SlotData<void(uint8_t)>(60, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 50, 61 },
        }}),
        // Slot 'setStaStored0'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored1'
        QtMocHelpers::SlotData<void()>(63, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored2'
        QtMocHelpers::SlotData<void()>(64, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored3'
        QtMocHelpers::SlotData<void()>(65, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored4'
        QtMocHelpers::SlotData<void()>(66, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored5'
        QtMocHelpers::SlotData<void()>(67, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored6'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setStaStored7'
        QtMocHelpers::SlotData<void()>(69, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta0'
        QtMocHelpers::SlotData<void()>(70, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta1'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta2'
        QtMocHelpers::SlotData<void()>(72, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta3'
        QtMocHelpers::SlotData<void()>(73, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta4'
        QtMocHelpers::SlotData<void()>(74, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta5'
        QtMocHelpers::SlotData<void()>(75, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta6'
        QtMocHelpers::SlotData<void()>(76, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_store_sta7'
        QtMocHelpers::SlotData<void()>(77, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'select_Sds0'
        QtMocHelpers::SlotData<void()>(78, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'select_Sds1'
        QtMocHelpers::SlotData<void()>(79, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'select_Sds2'
        QtMocHelpers::SlotData<void()>(80, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'select_Sds3'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'CombineFreq'
        QtMocHelpers::SlotData<void()>(82, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'File_Director'
        QtMocHelpers::SlotData<void()>(83, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Spectrum_Dialog'
        QtMocHelpers::SlotData<void()>(84, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'ADSR_Dialog'
        QtMocHelpers::SlotData<void()>(85, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'Save_Config'
        QtMocHelpers::SlotData<void()>(86, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggle_Mute'
        QtMocHelpers::SlotData<void()>(87, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'adsr_hall'
        QtMocHelpers::SlotData<void()>(88, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pB_Debug_clicked'
        QtMocHelpers::SlotData<void()>(89, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'wavfile_selected'
        QtMocHelpers::SlotData<void(const QString &)>(90, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 91 },
        }}),
        // Slot 'pB_oscgroup_clicked'
        QtMocHelpers::SlotData<void()>(92, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pB_Wavedisplay_clicked'
        QtMocHelpers::SlotData<void()>(93, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pB_fftmode_clicked'
        QtMocHelpers::SlotData<void()>(94, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->update_CB_external(); break;
        case 1: _t->updateColorButtons(); break;
        case 2: _t->Capture((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->Rtsp_Dialog(); break;
        case 4: _t->SDS_Dialog(); break;
        case 5: _t->slideFrq((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->cB_Beat_per_sec((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->dial_PMW_value_changed(); break;
        case 8: _t->mixer_balance(); break;
        case 9: _t->chord_delay(); break;
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
        case 22: _t->exit_synthesizer((*reinterpret_cast< std::add_pointer_t<APPID>>(_a[1]))); break;
        case 23: _t->GUI_Exit(); break;
        case 24: _t->start_synthesizer(); break;
        case 25: _t->start_audio_srv(); break;
        case 26: _t->start_composer(); break;
        case 27: _t->start_keyboard(); break;
        case 28: _t->connect_oscf((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 29: _t->connect_oscv((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 30: _t->connect_fmov((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 31: _t->connect_vcov((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
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
        case 45: _t->setStaPlay((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1]))); break;
        case 46: _t->setStaPlay0(); break;
        case 47: _t->setStaPlay1(); break;
        case 48: _t->setStaPlay2(); break;
        case 49: _t->setStaPlay3(); break;
        case 50: _t->setStaPlay4(); break;
        case 51: _t->setStaPlay5(); break;
        case 52: _t->setStaPlay6(); break;
        case 53: _t->setStaPlay7(); break;
        case 54: _t->setStaStored((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1]))); break;
        case 55: _t->setStaStored0(); break;
        case 56: _t->setStaStored1(); break;
        case 57: _t->setStaStored2(); break;
        case 58: _t->setStaStored3(); break;
        case 59: _t->setStaStored4(); break;
        case 60: _t->setStaStored5(); break;
        case 61: _t->setStaStored6(); break;
        case 62: _t->setStaStored7(); break;
        case 63: _t->toggle_store_sta0(); break;
        case 64: _t->toggle_store_sta1(); break;
        case 65: _t->toggle_store_sta2(); break;
        case 66: _t->toggle_store_sta3(); break;
        case 67: _t->toggle_store_sta4(); break;
        case 68: _t->toggle_store_sta5(); break;
        case 69: _t->toggle_store_sta6(); break;
        case 70: _t->toggle_store_sta7(); break;
        case 71: _t->select_Sds0(); break;
        case 72: _t->select_Sds1(); break;
        case 73: _t->select_Sds2(); break;
        case 74: _t->select_Sds3(); break;
        case 75: _t->CombineFreq(); break;
        case 76: _t->File_Director(); break;
        case 77: _t->Spectrum_Dialog(); break;
        case 78: _t->ADSR_Dialog(); break;
        case 79: _t->Save_Config(); break;
        case 80: _t->toggle_Mute(); break;
        case 81: _t->adsr_hall(); break;
        case 82: _t->pB_Debug_clicked(); break;
        case 83: _t->wavfile_selected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 84: _t->pB_oscgroup_clicked(); break;
        case 85: _t->pB_Wavedisplay_clicked(); break;
        case 86: _t->pB_fftmode_clicked(); break;
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
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
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
        if (_id < 87)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 87;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 87)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 87;
    }
    return _id;
}
QT_WARNING_POP
