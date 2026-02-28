/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * Mainwindow.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QObject>
#include <QMainWindow>
#include <QDebug>
#include <Qt>
#include <QTimer>
#include <QGraphicsView>

// Ocean
#include <data/Interface.h>
#include <data/Configbase.h>
#include <data/Semaphore.h>
#include <Spectrum.h>
#include <App.h>
#include <Wavedisplay_base.h>

// OceanGUI
#include <ui_mainwindow.h>
#include <include/File_Dialog.h>
#include <include/Cutterdialog.h>
#include <include/Keyboad_dialog.h>
#include <include/Rtsp_dialog.h>
#include <include/Spectrum_dialog.h>
#include <DataGraphicClass.h>


/**************************************************
 * WfDisplay_que_struct
 *************************************************/
struct WfDisplay_que_struct
{
	typedef vector<RoleId_e>
					roleid_vec_t;

    roleid_vec_t	Vec					{};
    RoleId_e		prev_role			;
    interface_t*	sds					;
    sta_role_map*	roleMap				;
    bool			debug				;

					WfDisplay_que_struct( interface_t* _sds, sta_role_map* _map );
					~WfDisplay_que_struct() = default;
	void 			Add					( RoleId_e role );
	void 			Init				();
	void 			Remove				( RoleId_e role );
	void 			Set_prev			();
	void 			Show				();

};

/**************************************************
 * MainWindow
 *************************************************/
QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

extern	Dataworld_class 	DaTA;
extern	Application_class	App;

class MainWindow
	: public 				QMainWindow
	, virtual 				Logfacility_class
	, virtual 				wavedisplay_struct

{
    Q_OBJECT
    Ui::MainWindow*			ui					;

public:
    explicit MainWindow		( QWidget*	parent 	= nullptr );
    virtual ~MainWindow		();


	Dataworld_class*		DaTA_p				= &DaTA;
    Appstate_class*			Appstate			= &DaTA_p->Appstate;
    Config_class*			Cfg_p 				= DaTA_p->Cfg_p;
	fs_t*					fs					= Cfg_p->fs;
    interface_t*			sds_master			= DaTA_p->sds_master;
    Id_t					SDS_ID				= sds_master->config;// active SDS for event logging
    SharedData_class*		Sds					= DaTA_p->SDS.GetSds( SDS_ID );
	interface_t*			sds_p 				= Sds->addr;//DaTA_p->GetSdsAddr();
    Semaphore_class*		Sem_p				= DaTA_p->Sem_p;

    EventLog_class			Eventlog			{ DaTA_p };
    EventLog_class*			Eventlog_p			= &Eventlog;

//    Rtsp_Dialog_class		Rtsp_Dialog_obj		{ this, DaTA_p};
    Rtsp_Dialog_class*		Rtsp_Dialog_p		;//= &Rtsp_Dialog_obj;

//    File_Dialog_class		File_Dialog_obj		{ this, DaTA_p, Eventlog_p  };
    File_Dialog_class*		File_Dialog_p		;//= &File_Dialog_obj;

//    Spectrum_Dialog_class  	Spectrum_Dialog_obj { this, DaTA_p, Eventlog_p };
    Spectrum_Dialog_class*  Spectrum_Dialog_p 	;//= &Spectrum_Dialog_obj;

//    Keyboad_Dialog_class	Keyboard_Dialog_obj	{ this, DaTA_p, Eventlog_p };
    Keyboad_Dialog_class*	Keyboard_Dialog_p	;//= &Keyboard_Dialog_obj;

//    CutDesk_Dialog_class	CutterDialog_obj	{ this, DaTA_p, Eventlog_p };
    CutDesk_Dialog_class*	CutDesk_Dialog_p	;//= &CutterDialog_obj;


    vector<QString> 		QWaveform_vec		{};
    QStringList				Qbps_str_lst		{};
    vector<QString>			Qwd_osc_names		{};
    vector<QString>			Qwd_role_names		{};
    vector<QString> 		Qwd_wdmode_names	{};
    vector<QString>			Qwd_fftmodes		{};
    keymap_struct			Keymap				{};
    vector<QRadioButton*> 	rb_S_vec 			{};

    sta_role_map 			StaRole_map 		= sta_role_map();
    WfDisplay_que_struct	WfDisplay_que		{ sds_p, &StaRole_map };
    const range_T<int> 		wd_mode_range 		{ 0, WD_MODE_SIZE-1 };

    struct cb_state_map
    {
    	StAId_e			id; // Mixer id
    	QCheckBox*		cb;
    	bool*			state;
    	uint8_t			align = 0;
    };
    typedef cb_state_map cb_state_t;

    vector<cb_state_t> 		cb_play_sta_vec 	{};
    vector<cb_state_t>		cb_store_sta_vec	{};
    vector<cb_state_t>		cb_filled_sta_vec	{};

    struct sl_value_map
    {
    	StAId_e			id; // Mixer id
    	QSlider*		sl;
    	uint8_t*		value;
//    	uint8_t			align = 0;
    };
    typedef sl_value_map sl_value_t;

    struct sl_lcd_map
    {
    	EVENTKEY_e		event; //
    	QLCDNumber*		lcd;
    	QSlider*		sl;
    	uint8_t*		value;
		int				max;
//    	uint8_t			align = 0;
    };
    typedef sl_lcd_map 	sl_lcd_t;

    struct sB_lbl_map
    {
    	EVENTKEY_e		event;
    	QSpinBox* 		sb;
    	QLabel* 		lbl;
		uint8_t*		value;
//    	uint8_t			align = 0;
    };
    typedef sB_lbl_map 	sB_lbl_t;

    vector<sl_value_t>		sl_sta_vec 			{};
    vector<sl_lcd_t>		sl_frqidx_vec		{};
    vector<sl_lcd_t>		sl_volume_vec		{};
    vector<sB_lbl_t>		sB_lbl_vec			{};

private:
    bps_struct				Bps					;

    QTimer					osc_timer_obj		{};
    QTimer*					osc_timer			= &osc_timer_obj;
    QTimer					status_timer_obj	{};
    QTimer*					status_timer		= &status_timer_obj;

    DataGraphic_class*		OscWidget_item		; // created by new DataGraphic_class;
    QGraphicsScene*     	scene 				; // created by new QGraphicsScene

    void setwidgetvalues();
    void select_Sds( Id_t sdsid );
    void initPanel();
    void initGuiVectors( interface_t* sds);
    void initOscillatorDisplay();
    void initFreqSlider();
    void initScrollbars();
    void initStateButtons();
    void initComboBoxes();
    void initLables();
    void initUiConnectors();
    void initTimer();
    void sliderFreq( sl_lcd_t map, int value );
    void sliderVolume( sl_lcd_t map );
    void mixer_slider( sl_value_t map );
    void waveform_slot( uint8_t*, uint8_t, OSCID_e, EVENTKEY_e, QLabel* );
    void set_wdrole( RoleId_e roleid );
    void unset_wdrole( RoleId_e roleid );
    void setStaPlay( StAId_e id );
    void setStaStored( StAId_e staId );
    void toggle_store_sta( StAId_e id );
    void changeAppState( APPID appid, string terminal, string binary  );

public slots:
	void update_CB_external();
    void updateColorButtons();

private slots:

    void slideFrq( int );
    void Notes_per_measure( int );
    void Beat_per_sec( int  );

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

    void GUI_Exit();
	void start_sdsview();
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

    void clear_StAs_play();

    void setStaPlay0();
    void setStaPlay1();
    void setStaPlay2();
    void setStaPlay3();
    void setStaPlay4();
    void setStaPlay5();
    void setStaPlay6();
    void setStaPlay7();

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

    void Spectrum_Dialog();
    void ADSR_Dialog();

    void Save_Config();
    void toggle_Mute();

    void adsr_hall();

    void pB_Debug_clicked();
    void wavfile_selected( QString arg);
    void pB_oscgroup_clicked();
    void pB_Wavedisplay_clicked();
    void pB_fftmode_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
