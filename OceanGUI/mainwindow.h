#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QGuiApplication>
#include <QMainWindow>
#include <QDebug>
#include <Qt>
#include <QTimer>
#include <QGraphicsView>

// Synthesizer
#include <Interface.h>
#include <mixer.h>
#include <Spectrum.h>

// qtcreator
#include "File_Dialog.h"
#include "ui_mainwindow.h"
#include <spectrum_dialog_class.h>
#include <oszilloscopewidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

void read_filelist( QComboBox* CB, string path, QString type );

class MainWindow : public QMainWindow, Logfacility_class
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Interface_class     	GUI;
    Spectrum_class          Spectrum			{};
    File_Dialog_class*      Dialog_File         = nullptr;
    Spectrum_Dialog_class*  Spectrum_Dialog_Obj = nullptr;
    QComboBox*              CB_external         = nullptr;
    QString                 Instrument_name     = "default";
    vector<QString> 		Qwavedisplay_type_str_vec {};
    vector<QString> 		QWaveform_vec		{};

    void setwidgetvalues();
    void Updatewidgets();
    //


private slots:

    void pB_Wavedisplay_clicked();
    void dial_soft_freq_value_changed();
    void sB_Duration( int );
    void dial_PMW_value_changed();
    void dial_decay_value_changed();
    void get_record_status( );

    void MAIN_slot_Hz();
    void MAIN_slot_volume();
    void VCO_slot_Hz();
    void VCO_slot_volume();
    void Slider_FMO_Hz_changed(int);
    void FMO_slot_volume();

    void waveform_slot( uint8_t*, uint8_t, int, int, QLabel* );
    void Main_Waveform_slot( int );
    void VCO_Waveform_slot( int );
    void FMO_Waveform_slot( int );

    void Controller_Exit();
    void GUI_Exit();
    void Audio_Exit();
    void start_synthesizer();
    void start_audio_srv();
    void start_composer();

    void set_mode_f();
    void set_mode_v();
    void set_mode_o();

    void Sl_mix1( int );
    void Sl_mix2( int );
    void Sl_mix3( int );
    void Sl_mix4( int );
    void Sl_mix5( int );
    void Sl_mix6( int );
    void Sl_mix7( int );
    void slot_dial_ramp_up_down();
    void memory_clear();
    void toggle_Record();

    void read_polygon_data();

    void Store();
    void Clear_Banks();

    void change_status1();
    void change_status2();
    void change_status3();
    void change_status4();
    void change_status5();

    void File_Director();
    void Spectrum_Dialog();

    void Save_Config();
    void toggle_Mute();
    void connect_fmo();
    void connect_vco();

    void main_adsr_sustain();

    void melody_connect();
    void pB_Debug_clicked();
    void wavfile_selected( const QString &arg);

    void hs_hall_effect_value_changed(int);

private:
    Ui::MainWindow      *ui;
    QGraphicsScene      *scene;
    OszilloscopeWidget  *item;
};

#endif // MAINWINDOW_H
