#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
//#include <QGuiApplication>
#include <QMainWindow>
#include <QDebug>
#include <Qt>
#include <QTimer>
#include <QGraphicsView>

// Synthesizer
#include <data/Interface.h>
#include <Mixer.h>
#include <Spectrum.h>
#include <App.h>
#include <File_Dialog.h>
#include <Oszilloscopewidget.h>
#include <Spectrum_dialog_class.h>
#include <data/Semaphore.h>
#include <Rtsp_dialog_class.h>


// qtcreator
#include "ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow, 	virtual public Logfacility_class
{
    Q_OBJECT

public:
    Dataworld_class			DaTA_class{ GUI_ID };
    Dataworld_class*		DaTA 	= &DaTA_class;
    Application_class		App			{ &DaTA_class };

    Config_class*			Cfg 	= DaTA->Cfg_p;
    Interface_class*		Sds		= DaTA->Sds_p;

    Spectrum_base          	Spectrum			{};
    Semaphore_class*		Sem					= DaTA->Sds.Sem_p;

    Rtsp_Dialog_class		Rtsp_Dialog_obj		{ this, DaTA, Sem };
    Rtsp_Dialog_class*		Rtsp_Dialog_p		= &Rtsp_Dialog_obj;

    File_Dialog_class		File_Dialog_obj		{ this, DaTA->Sds_p };
    File_Dialog_class*		File_Dialog_p		= &File_Dialog_obj;

    Spectrum_Dialog_class  	Spectrum_Dialog_Obj { this, DaTA->Sds_p };
    Spectrum_Dialog_class*  Spectrum_Dialog_p 	= &Spectrum_Dialog_Obj;

    QComboBox*              CB_external         = nullptr;
    QString                 Instrument_name     = "default";
    vector<QString> 		Qwavedisplay_type_str_vec
												{};
    vector<QString> 		QWaveform_vec		{};
    QStringList				Qbps_str_list		{};
    vector<QRadioButton*> 	rb_sta_vec			{};
    vector<QCheckBox*>		cb_sta_vec			{};
    vector<QSlider*>		sl_sta_vec			{};


    void setwidgetvalues();
    void Updatewidgets();
    explicit MainWindow(	QWidget*			parent 	= nullptr);
    ~MainWindow();
    //


private slots:

//	void show_time_elapsed();
	void Rtsp_Dialog();

    void pB_Wavedisplay_clicked();
    void dial_soft_freq_value_changed();
    void cB_Beat_per_sec( int );
    void dial_PMW_value_changed();
    void dial_decay_value_changed();
    void get_record_status( );

    void MAIN_slot_Hz();
    void MAIN_slot_volume();
    void VCO_slot_volume();
    void Slider_FMO_Hz_changed(int);
    void Slider_VCO_Hz_changed(int);
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

    void Sl_mix0( int );
    void Sl_mix1( int );
    void Sl_mix2( int );
    void Sl_mix3( int );
    void Sl_mix4( int );
    void Sl_mix5( int );
    void Sl_mix6( int );
    void Sl_mix7( int );

    void slot_dial_ramp_up_down();
    void memory_clear();
    void SaveRecord();
    void read_polygon_data();

    void Clear_Banks();

    void toggle_mute0(int);
    void toggle_mute1(int);
    void toggle_mute2(int);
    void toggle_mute3(int);
    void toggle_mute4(int);
    void toggle_mute5(int);
    void toggle_mute6(int);
    void toggle_mute7(int);


    void toggle_store_sta0();
    void toggle_store_sta1();
    void toggle_store_sta2();
    void toggle_store_sta3();
    void toggle_store_sta4();
    void toggle_store_sta5();
    void toggle_store_sta6();
    void toggle_store_sta7();

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
    Ui::MainWindow*		ui;
    QTimer				osc_timer_obj{};
    QTimer*				osc_timer			= &osc_timer_obj;
    QTimer				status_timer_obj{};
    QTimer*				status_timer		= &status_timer_obj;
    QTimer				record_timer_obj{};
    QTimer*				record_timer		= &record_timer_obj;
    QGraphicsScene  	Scene { this };
    QGraphicsScene*     scene 	= &Scene;
    OszilloscopeWidget* item	= nullptr;
};

#endif // MAINWINDOW_H
