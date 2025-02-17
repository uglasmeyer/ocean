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
#include <data/Semaphore.h>
#include <Rtsp_dialog_class.h>
#include <Wavedisplay_base.h>


// qtcreator
#include "ui_mainwindow.h"
#include <File_Dialog.h>
#include <Oszilloscopewidget.h>
#include <Spectrum_dialog_class.h>
#include <Sds_dialog_class.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow, 	virtual public Logfacility_class
{
    Q_OBJECT

public:
    Dataworld_class			DaTA_class			{ GUI_ID };
    Dataworld_class*		DaTA 				= &DaTA_class;
    Application_class		App					{ DaTA };

    Config_class*			Cfg 				= DaTA->Cfg_p;
    interface_t*			Sds_master			= DaTA->sds_master;
    uint8_t					SDS_ID				= Sds_master->config;
    Interface_class*		Sds					= DaTA->GetSds( SDS_ID );

    Spectrum_class          Spectrum			{};
    Semaphore_class*		Sem					= DaTA->Sem_p;

    Rtsp_Dialog_class		Rtsp_Dialog_obj		{ this, DaTA};
    Rtsp_Dialog_class*		Rtsp_Dialog_p		= &Rtsp_Dialog_obj;

    File_Dialog_class		File_Dialog_obj		{ this, DaTA->Sds_p, DaTA->Sem_p };
    File_Dialog_class*		File_Dialog_p		= &File_Dialog_obj;

    Spectrum_Dialog_class  	Spectrum_Dialog_obj { this, DaTA->Sds_p };
    Spectrum_Dialog_class*  Spectrum_Dialog_p 	= &Spectrum_Dialog_obj;

    SDS_Dialog_class		SDS_Dialog_Obj		{ this, Sds };
    SDS_Dialog_class*		SDS_Dialog_p		= &SDS_Dialog_Obj;

    QComboBox*              CB_external         = nullptr;
    QString                 Instrument_name     = QString::fromStdString( Sds->Read_str( INSTRUMENTSTR_KEY ) );

    vector<QString> 		QWaveform_vec		{};
    QStringList				Qbps_str_lst		{};
    vector<QString>			Qwd_osc_names		{};
    vector<QString>			Qwd_display_names	{};
    vector<QString> 		Qwd_wdmode_names	{};
    vector<QString>			Qwd_fftmodes		{};

    vector<QRadioButton*> 	rb_S_vec 			{};
    typedef struct rb_state_map
    {
    	int				id; // Mixer id
    	QRadioButton*	rb;
    	bool*			state;
    } rb_state_t;
    vector<rb_state_t>		rb_sta_vec 			{};

    typedef struct cb_state_map
    {
    	int				id; // Mixer id
    	QCheckBox*		cb;
    	bool*			state;
    } cb_state_t;
    vector<cb_state_t> 		cb_sta_vec 			{};

    typedef struct sl_value_map
    {
    	int				id; // Mixer id
    	QSlider*		sl;
    	uint8_t*		value;
    } sl_value_t;
    vector<sl_value_t>		sl_sta_vec 			{};

    vector<wave_t*>			wp_vec				{ 	&Sds->addr->VCO_wp,
    												&Sds->addr->FMO_wp,
													&Sds->addr->OSC_wp };
;


    explicit MainWindow(	QWidget*			parent 	= nullptr);
    virtual ~MainWindow();
    //

private:
//    Ui::MainWindow*		ui;
    unique_ptr<Ui::MainWindow>		ui;

    QTimer				osc_timer_obj		{};
    QTimer*				osc_timer			= &osc_timer_obj;
    QTimer				status_timer_obj	{};
    QTimer*				status_timer		= &status_timer_obj;
    QGraphicsScene  	Scene 				{ this };
    QGraphicsScene*     scene 				{ &Scene };
    OszilloscopeWidget* OscW_item;			//	created by "new";

    const int
	OSCID = osc_struct::OSCID,
    VCOID = osc_struct::VCOID,
	FMOID = osc_struct::FMOID;

    void setwidgetvalues();
    void updateWidgets();
    void initPanel();
    void select_Sds( uint sdsid );
    void initMixerVector();
    void initOscWidget();
    void initFreqSlider();
    void initScrollbars();
    void initWavedisplay();
    void initComboBoxes();
    void initUiConnectors();
    void initTimer();
    void sliderFreq( uint8_t oscid, QLCDNumber* lcd, int value, char key );
    void sliderVolume( uint8_t oscid, QLCDNumber* lcd, int value, char key);
    void setButtonColor( QPushButton* pb, QColor color  );


private slots:

//	void show_time_elapsed();
	void Rtsp_Dialog();
	void SDS_Dialog();

    void dial_soft_freq_value_changed();
    void cB_Beat_per_sec( int );
    void dial_PMW_value_changed();
    void dial_decay_value_changed();
    void get_record_status( );

    void MAIN_slot_volume();
    void VCO_slot_volume();
    void FMO_slot_volume();

    void Slider_OSC_Freq( int );
    void Slider_FMO_Freq( int);
    void Slider_VCO_Freq( int);

    void Slider_VCO_Adjust( int );
    void Slider_FMO_Adjust( int );

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

    void toggle_mute0();
    void toggle_mute1();
    void toggle_mute2();
    void toggle_mute3();
    void toggle_mute4();
    void toggle_mute5();
    void toggle_mute6();
    void toggle_mute7();


    void toggle_store_sta0();
    void toggle_store_sta1();
    void toggle_store_sta2();
    void toggle_store_sta3();
    void toggle_store_sta4();
    void toggle_store_sta5();
    void toggle_store_sta6();
    void toggle_store_sta7();

    void select_Sds0();
    void select_Sds1();
    void select_Sds2();
    void select_Sds3();

    void File_Director();
    void Spectrum_Dialog();

    void Save_Config();
    void toggle_Mute();
    void connect_fmo();
    void connect_vco();

    void main_adsr_sustain();

    void pB_Debug_clicked();
    void wavfile_selected( const QString &arg);
    void pB_oscgroup_clicked();
    void pB_Wavedisplay_clicked();
    void pB_fftmode_clicked();

    void hs_hall_effect_value_changed(int);

};

#endif // MAINWINDOW_H


