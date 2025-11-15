#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QDebug>
#include <Qt>
#include <QTimer>
#include <QGraphicsView>

// Ocean
#include <data/Interface.h>
#include <Mixer.h>
#include <Spectrum.h>
#include <App.h>
#include <data/Configbase.h>
#include <data/Semaphore.h>
#include <include/File_Dialog.h>
#include <Wavedisplay_base.h>

// OceanGUI
#include "ui_mainwindow.h"
#include <include/Keyboad_dialog.h>
#include <include/Oszilloscopewidget.h>
#include <include/Rtsp_dialog.h>
#include <include/Spectrum_dialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow :
		public QMainWindow,
		virtual Logfacility_class,
		osc_struct
{
    Q_OBJECT
	string					className			= "";
public:
	process_t				Process				{};
	Config_class			Cfg					{};
	file_structure*			fs					= Cfg.fs;

	Semaphore_class			Sem					{ Cfg.Config.Sem_key };
	Dataworld_class 		DaTA				{ &Cfg, &Sem };
	Application_class		App					{ &DaTA };

    EventLog_class			Eventlog			{ &DaTA };
    EventLog_class*			Eventlog_p			= &Eventlog;
    Appstate_class*			Appstate			= &DaTA.Appstate;
    Config_class*			Cfg_p 				= DaTA.Cfg_p;
    interface_t*			sds_master			= DaTA.sds_master;
    Id_t					SDS_ID				= sds_master->config;// active SDS for event logging
    Interface_class*		Sds					= DaTA.SDS.GetSds( SDS_ID );
	interface_t*			sds 				= Sds->addr;//DaTA.GetSdsAddr();

    Spectrum_class          Spectrum			{};
    Semaphore_class*		Sem_p				= DaTA.Sem_p;

    Rtsp_Dialog_class		Rtsp_Dialog_obj		{ this, &DaTA};
    Rtsp_Dialog_class*		Rtsp_Dialog_p		= &Rtsp_Dialog_obj;

    File_Dialog_class		File_Dialog_obj		{ this, &DaTA, Eventlog_p  };
    File_Dialog_class*		File_Dialog_p		= &File_Dialog_obj;

    Spectrum_Dialog_class  	Spectrum_Dialog_obj { this, DaTA.Sds_p, Eventlog_p };
    Spectrum_Dialog_class*  Spectrum_Dialog_p 	= &Spectrum_Dialog_obj;

    Keyboad_Dialog_class	Keyboard_Dialog_obj	{ this, &DaTA, Eventlog_p };
    Keyboad_Dialog_class*	Keyboard_Dialog_p	= &Keyboard_Dialog_obj;

    QComboBox*              CB_external         = nullptr;
    QString                 Instrument_name     = QReadStr( Sds, INSTRUMENTSTR_KEY ) ;
    QRect 					Spectrum_Dialog_Rect= QRect( QPoint(0,0),QSize(0,0) );
    vector<QString> 		QWaveform_vec		{};
    QStringList				Qbps_str_lst		{};
    vector<QString>			Qwd_osc_names		{};
    vector<QString>			Qwd_role_names		{};
    vector<QString> 		Qwd_wdmode_names	{};
    vector<QString>			Qwd_fftmodes		{};
    QStringList				QCapture_str_lst	{};
    keymap_struct			Keymap				{};
    vector<QRadioButton*> 	rb_S_vec 			{};

    typedef struct cb_state_map
    {
    	STAID_e			id; // Mixer id
    	QCheckBox*		cb;
    	bool*			state;
    } cb_state_t;
    vector<cb_state_t> 		cb_play_sta_vec 	{};
    vector<cb_state_t>		cb_store_sta_vec	{};
    vector<cb_state_t>		cb_filled_sta_vec	{};

    typedef struct sl_value_map
    {
    	STAID_e			id; // Mixer id
    	QSlider*		sl;
    	uint8_t*		value;
    } sl_value_t;
    struct sl_lcd_map
    {
    	EVENTKEY_e		event; //
    	QLCDNumber*		lcd;
    	QSlider*		sl;
    	uint8_t*		value;
		int				max;
    };
    typedef sl_lcd_map 	sl_lcd_t;
    struct sB_lbl_map
    {
    	EVENTKEY_e		event;
    	QSpinBox* 		sb;
    	QLabel* 		lbl;
		uint8_t*		value;
    };
    typedef sB_lbl_map 	sB_lbl_t;
    vector<sl_value_t>		sl_sta_vec 			{};
    vector<sl_lcd_t>		sl_frqidx_vec		{};
    vector<sl_lcd_t>		sl_volume_vec		{};
    vector<sB_lbl_t>		sB_lbl_vec			{};

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
    QGraphicsScene*     scene 				= &Scene ;
    OszilloscopeWidget* OscWidget_item;			//	created by "new";


    void setwidgetvalues();
    void initPanel();
    void select_Sds( Id_t sdsid );
    void initGuiVectors( interface_t* sds);
    void initOscillatorDisplay();
    void initFreqSlider();
    void initScrollbars();
    void initStateButtons();
    void initComboBoxes();
    void initLables();
    void initUiConnectors();
    void initTimer();
    void sliderFreq( sl_lcd_t map, uint8_t value );
    void sliderVolume( sl_lcd_t map );
    void mixer_slider( sl_value_t map );
    void waveform_slot( uint8_t*, uint8_t, int, EVENTKEY_e, QLabel* );
    void set_wdrole( OscroleId_t roleid );


public slots:
	void update_CB_external();
    void updateColorButtons();

private slots:

//	void show_time_elapsed();

	void Capture( QString str );

	void Rtsp_Dialog();
	void SDS_Dialog();

    void slideFrq( int );
    void cB_Beat_per_sec( int );
    void dial_PMW_value_changed();
    void mixer_balance();
    void chord_delay();


    void get_record_status( );

    void Main_slot_volume();
    void VCO_slot_volume();
    void FMO_slot_volume();

    void Slider_OSC_Freq( int );
    void Slider_FMO_Freq( int);
    void Slider_VCO_Freq( int);

    void Slider_VCO_Adjust( int );
    void Slider_FMO_Adjust( int );

    void Main_Waveform_slot( int );
    void VCO_Waveform_slot( int );
    void FMO_Waveform_slot( int );

    void exit_synthesizer( APPID );
    void GUI_Exit();
    void start_synthesizer();
    void start_audio_srv();
    void start_composer();
    void start_keyboard();

    void connect_oscf( bool );
    void connect_oscv( bool );
    void connect_fmov( bool );
    void connect_vcov( bool );

    void Sl_mix0( int );
    void Sl_mix1( int );
    void Sl_mix2( int );
    void Sl_mix3( int );
    void Sl_mix4( int );
    void Sl_mix5( int );
    void Sl_mix6( int );
    void Sl_mix7( int );

    void slideVol(int);
    void memory_clear();
    void SaveRecord();
    void read_polygon_data();

    void Clear_Banks();

    void setStaPlay( uint8_t id );
    void setStaPlay0();
    void setStaPlay1();
    void setStaPlay2();
    void setStaPlay3();
    void setStaPlay4();
    void setStaPlay5();
    void setStaPlay6();
    void setStaPlay7();

    void setStaStored( uint8_t staId );
    void setStaStored0();
    void setStaStored1();
    void setStaStored2();
    void setStaStored3();
    void setStaStored4();
    void setStaStored5();
    void setStaStored6();
    void setStaStored7();

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

    void CombineFreq();

    void File_Director();
    void Spectrum_Dialog();
    void ADSR_Dialog();

    void Save_Config();
    void toggle_Mute();

    void adsr_hall();

    void pB_Debug_clicked();
    void wavfile_selected( const QString &arg);
    void pB_oscgroup_clicked();
    void pB_Wavedisplay_clicked();
    void pB_fftmode_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H


