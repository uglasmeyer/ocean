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



// Ocean
#include <Ocean.h>
#include <data/DataWorld.h>
#include <data/Interface.h>
#include <data/Configbase.h>
#include <data/Semaphore.h>
#include <Spectrum.h>
#include <Wavedisplay.h>
#include <App.h>

// Qt
//#include <QObject>
#include <QMainWindow>
#include <QDebug>
#include <Qt>
#include <QTimer>
#include <QGraphicsView>

// OceanGUI
#include <ui_mainwindow.h>
#include <File_Dialog.h>
#include <Cutterdialog.h>
#include <DataGraphic.h>
#include <Keyboad_dialog.h>
#include <Rtsp_dialog.h>
#include <Spectrum_dialog.h>
#include <GUIcommon.h>


/**************************************************
 * MainWindow
 * manage the control elements of the main window
 *************************************************/

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

extern Dataworld_class		DaTA;
extern Application_class	App;

class MainWindow
	: public 				QMainWindow
	, virtual 				Logfacility_class
	, public				Frequency_class

{
    Q_OBJECT
    Ui::MainWindow*			ui					;


public:
    explicit MainWindow		( QWidget*	parent 	= nullptr );
    virtual ~MainWindow		();


	Dataworld_class*		DaTA_p				= &DaTA;
	Interface_base			ifb					;
    Appstate_class*			Appstate			;//= &DaTA_p->Appstate;
    Config_class*			Cfg_p 				;//= DaTA_p->Cfg_p;
	fs_t*					fs					;//= DaTA_p->Cfg_p->fs;
    interface_t*			sds_master			;//= DaTA_p->sds_master;
    SDSId_t					SDS_ID				;//= sds_master->config;// active SDS for event logging
    SharedData_class*		Sds					;//= DaTA_p->SDS.GetSds( SDS_ID );
	interface_t*			sds_p 				;//= Sds->addr;
    Semaphore_class*		Sem_p				;//= DaTA_p->Sem_p;

    EventLog_class			Eventlog			;
    EventLog_class*			Eventlog_p			;

    sta_role_map			StaRole_map 		;
    WfDisplay_que_struct	WfDisplay_que		;

    Rtsp_Dialog_class*		Rtsp_Dialog_p		;
    File_Dialog_class*		File_Dialog_p		;
    Spectrum_Dialog_class*  Spectrum_Dialog_p 	;
    Keyboad_Dialog_class*	Keyboard_Dialog_p	;
    CutDesk_Dialog_class*	CutDesk_Dialog_p	;


    keymap_struct			Keymap				{};
    QStringList				Qbps_str_lst		{};
    vector<QString>			Qwd_osc_names		{};
    vector<QString> 		Qwd_wdmode_names	{};
    vector<QRadioButton*> 	rb_S_vec 			{};

    typedef struct cb_state_map
    {
    	QCheckBox*		cb;
    	StA_state_t*	state;
    } cb_state_t;
    typedef struct tb_state_map
    {
    	QToolButton*	cb;
    	StA_state_t*	state;
    } tb_state_t;

    vector<tb_state_t> 		tb_sta_play_vec 	{};
    vector<cb_state_t>		cb_sta_filled_vec	{};
    vector<cb_state_t>		cb_sta_store_vec	{};

    struct sl_lcd_map
    {
    	EVENTKEY_e		event; //
    	QLCDNumber*		lcd;
    	QSlider*		sl;
    	uint8_t*		value;
    };
    typedef sl_lcd_map 	sl_lcd_t;

    struct sl_value_map
    {
    	QSlider*		sl;
    	StA_amp_t*		value;
    };
    typedef sl_value_map sl_value_t;

    struct pB_lbl_map
    {
    	EVENTKEY_e		event;
    	QPushButton*	pB;
		WFID_e*			value;
    };
    typedef pB_lbl_map 	pB_lbl_t;

    vector<sl_value_t>		sl_sta_vec 		{};
    vector<sl_lcd_t>		sl_freq_vec		{};
    vector<sl_lcd_t>		sl_volume_vec	{};
    vector<pB_lbl_t>		wf_vec			{};

private:
    bps_struct				Bps					;
    direvent_t 				EventWAV			;
    QTimer					osc_timer_obj		{};
    QTimer*					dataGraphic_timer	= &osc_timer_obj;
    QTimer					status_timer_obj	{};
    QTimer*					uiUpdate_timer		= &status_timer_obj;
    vector<QPushButton*> 	wf_pb_vec			{};

    DataGraphic_class*		DataDisplay			; // created by new DataGraphic_class;
    QGraphicsScene*     	scene 				; // created by new QGraphicsScene

    void changeAppState( APPID_e appid, string terminal, string args=""  );
    void connect_secundary( OSCID_e oscid, OSCID_e secid, char mode, bool val );
    void initUiVectors();
    void initPanel();
    void initGuiVectors();
    void initDataDisplay();
    void initFreqSlider();
    void initPushbuttons();
    void initStateButtons();
    void initComboBoxes();
    void initOther();
    void initLables();
    void initSignals();
    void initTimer();
    void initWfDisplay_que();
    void sliderFreq( OSCID_e oscid, int value );
    void sliderVolume( sl_lcd_t map );
    void sliderAdjust( OSCID_e oscid, uint8_t value );
    void sliderMixer( StAId_e staid, uint8_t value );
    void setWaveform( OSCID_e oscid, int dir );
    void set_default_role();
    void set_wdrole( StAId_e staid, bool flag );
	void set_wdrole( RoleId_e roleid, bool flag );
    void setwidgetvalues();
    void set_pb_wd_mode_text( WdModeID_t mode );
    void select_Sds( SDSId_t sdsid );
    void setStaStored( StAId_e staId );
    void toggle_sta_store( StAId_e id );
    void updateGuiVectors( interface_t* sds);

public slots:
    void updateColorButtons();
	void UpdateDataDisplay();
	void Update_Statusbar( QString text );

private slots:

	void tb_logState_slot();
	void set_overwritemode( bool );
    void slideFrq( int );
    void toggleStaPlay( StAId_e id );
    void remote_sta_off( StAId_e id );
    void Notes_per_measure( int );
    void Beat_per_sec( int  );

    void dial_PMW_value_changed();
    void mixer_balance();
    void chord_delay();

    void update_progressbar( );

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
    void capture_audio();

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

    void CombineFreq_fmo();
    void CombineFreq_vco();

    void Spectrum_Dialog();
    void ADSR_Dialog();

    void Save_Config();
    void toggle_Mute();

    void adsr_hall();

    void pB_Mode_clicked( int );
    void wavfile_selected( QString arg);
    void pB_OSC_clicked( int );
    void pB_Role_clicked( int );
	void update_CB_external();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
