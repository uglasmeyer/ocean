/**********************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * oceanGUI.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

// Qt

// Ocean

// OceanGUI
#include <Mainwindow.h>

const string 		Module 			= OCEANGUI;
typedef MainWindow::cb_state_map 	cb_state_t;
typedef MainWindow::sl_value_map 	sl_value_t;

auto set_sl_sta_value = [ ]( MainWindow* M )
{
    for( sl_value_t map : M->sl_sta_vec )
    	map.sl->setValue( *map.value );
};
auto set_cb_ssta_value = [ ]( MainWindow* M )
{
	for( cb_state_t map : M->cb_store_sta_vec )
    	map.cb->setChecked( (bool) *map.state );
};
auto set_cb_psta_value = [ ]( MainWindow* M )
{
    for( cb_state_t map : M->cb_play_sta_vec )
    	map.cb->setChecked( *map.state );
};
auto set_cb_filled_value = [  ]( MainWindow* M  )
{
	for( cb_state_t map : M->cb_filled_sta_vec )
		map.cb->setChecked( *map.state );
};

auto AppIdRole = [ ]( APPID_e appid )
{
	map<APPID_e, RoleId_e> AppIdRole_map // @suppress("Invalid arguments")
	{
			{ AUDIOID		, AUDIOROLE },
			{ SYNTHID		, INSTRROLE },
			{ KEYBOARDID	, KBDROLE 	},
			{ COMPOSERID	, INSTRROLE }
	};

	auto role_itr = AppIdRole_map.find( appid );
	if( role_itr != AppIdRole_map.end() )
		return role_itr->second;
	else
		return ROLE_SIZE;
};

/**************************************************
 * MainWindow
 *************************************************/

MainWindow::MainWindow		( QWidget *parent )
	: Logfacility_class		( "OceanGUI" )
	, ui					( new Ui::MainWindow( ) )
	, ifb					( DaTA_p )
	, Eventlog				( DaTA_p )
	, Eventlog_p			( &Eventlog )
	, StaRole_map			()
	, WfDisplay_que			( ifb.sds_p, &StaRole_map )
	, Rtsp_Dialog_p			( new Rtsp_Dialog_class 	{ this, DaTA_p } )
	, File_Dialog_p			( new File_Dialog_class 	{ this, DaTA_p, Eventlog_p, &WfDisplay_que } )
	, Spectrum_Dialog_p 	( new Spectrum_Dialog_class { this, DaTA_p, Eventlog_p } )
	, Keyboard_Dialog_p		( new Keyboad_Dialog_class	{ this, DaTA_p, Eventlog_p } )
	, CutDesk_Dialog_p		( new CutDesk_Dialog_class	{ this, DaTA_p, Eventlog_p } )
	, Bps					()
{
   	Appstate				= &DaTA_p->Appstate;
    Cfg_p 					= DaTA_p->Cfg_p;
	fs						= DaTA_p->Cfg_p->fs;
	Sem_p					= DaTA_p->Sem_p;

	initWfDisplay_que		();

    ui->setupUi				( this );
    initUiVectors			();
    initDataDisplay			();

    sds_master				= ifb.sds_master;
    Sds						= ifb.Sds;
    SDS_ID					= MAXCONFIG;			// force select_Sds
    select_Sds				( sds_master->comId );	// needs ui

    sds_master->Record_state= StateId_e::INACTIVE;

	initPanel				();
	initLables				();
    initScrollbars			();
    initFreqSlider			();
    initComboBoxes			();
    initSignals				();
	initStateButtons		();
    updateColorButtons		();
    initTimer				();

    initOther				();
    UpdateDataDisplay		();
    updateColorButtons		();
    Info( "User Interface initialized" );
}

MainWindow::~MainWindow()
{
	DESTRUCTOR( className );
	delete Rtsp_Dialog_p;
	delete File_Dialog_p;
	delete DataDisplay;
	delete Spectrum_Dialog_p;
	delete Keyboard_Dialog_p;
	delete CutDesk_Dialog_p;
	delete scene;
	delete ui;
}

void MainWindow::keyPressEvent( QKeyEvent *event)
{
	kbdkey_t	 	key 	{};
					key.Int	= Keymap.Qt_key( event->key() ) ;

//					coutf << "keypressed" << key.Int << endl;
	switch ( key.Int )
	{
		case KEYCODE::ESC :
		{
	        event->accept();
			GUI_Exit();
			break;
		}
		case 0 :
		{
	        event->accept();
			break;
		}
		default :
		{
	        event->accept();
			set_wdrole( KBDROLE );
			sds_p->Kbd_state.key = key.Int;
			break;
		}
	} // switch  key
	QMainWindow::keyPressEvent( event );
}

void MainWindow::wheelEvent( QWheelEvent* event )
{
	// Crtl-keyboard button and wheel event
    if ( event->modifiers() & Qt::ControlModifier )
    {
    	float zoomFactor = sds_p->WD_state.zoom;
		// angleDelta().y() gives the vertical scroll amount in 1/8 degrees
		int delta = event->angleDelta().y();

		if (delta > 0)
		{
			zoomFactor *= 1.1; // Zoom in
		}
		if (delta < 0)
		{
			zoomFactor /= 1.1; // Zoom out
		}

		// Clamp zoom factor to avoid extreme values
		zoomFactor = std::clamp( zoomFactor, 0.1f, 1.0f );
    	Sds->Set( sds_p->WD_state.zoom, zoomFactor );
        event->accept();
    }
    else
    {
          // Pass unhandled events to base class (e.g., for scrolling)
          QWidget::wheelEvent(event);
    }
}

void MainWindow::UpdateDataDisplay()
{
    Data_t* Data = &sds_p->wavedata[0];
	DataDisplay->Set_sds( sds_p );

    if( sds_p->WD_state.roleId == AUDIOROLE )
    {
    	Data = &sds_master->wavedata[0];
    	DataDisplay->Set_sds( sds_master );
    }
	DataDisplay->ReadData	( Data, wavedisplay_len );
	scene->update();
}

void MainWindow::GUI_Exit()
{
    Info( "Exit" );

    QApplication::quit();
}

void MainWindow::mixer_balance()
{
	int value = ui->hs_balance->value();
	Sds->Set( sds_master->mixer_balance, (int8_t) value );
}

void MainWindow::chord_delay()
{
	int value = ui->hs_chord_delay->value();
	Sds->Set( sds_p->Noteline_prefix.chord_delay, value );
}

/**************************************************
 * Wavedisplay management
 *************************************************/
void MainWindow::set_wdrole( RoleId_e roleid )
{
	WfDisplay_que.Add( roleid );
	ui->pB_WdRole->setText( Qwd_role_names[ roleid ] );
	Sds->Set( sds_p->WD_state.zoom, 1.0f );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );
}

void MainWindow::unset_wdrole( RoleId_e role )
{
	WfDisplay_que.Remove( role );
}

void MainWindow::pB_Role_clicked()
{
	RoleId_e 	prev_role	= sds_p->WD_state.roleId;
	unset_wdrole			( prev_role );
	RoleId_e 	role	 	= check_cycle2( roleid_range, ++prev_role );
	set_wdrole				( role );
};

void MainWindow::pB_Mode_clicked()
{
	WdModeID_t counter = sds_p->WD_state.mode;
	counter = check_cycle2( wd_mode_range,  ++counter );
    Sds->Set( sds_p->WD_state.mode , counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );

    ui->pB_wd_mode->setText( Qwd_wdmode_names[ counter ] );
}

void MainWindow::pB_OSC_clicked()
{
    OSCID_e counter = sds_p->WD_state.oscId ;
	counter = check_cycle2( oscid_range, ++counter );
    Sds->Set( sds_p->WD_state.oscId, counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY);

    ui->pB_oscgroup->setText( Qwd_osc_names[ counter ] );
}
//**********************************************************************

void MainWindow::wavfile_selected( QString arg)
{
    if ( arg.length() > 0 )
    {
        string str = arg.toStdString();
        ifb.Sds_master->Write_str( WAVFILE_STR, str );
        Eventlog.add( SDS_ID, READ_WAVFILE_KEY);
        if( CutDesk_Dialog_p->isVisible() )
        {
        	CutDesk_Dialog_p->Setup();
        }
        else
        {
        	set_wdrole( EXTERNALROLE );
        }
    }
}

void MainWindow::adsr_hall( )
{
	uint8_t value = ui->hs_hall_effect->value();
    Sds->Set(sds_p->adsr_arr[OSCID].hall , value);
    Eventlog.add( SDS_ID, ADSR_KEY);
}

void MainWindow::Beat_per_sec( int bps_id )
{
	uint8_t bps = Bps.Bps_vec[bps_id];
	Sds->Set( sds_p->instrumentClock, bps );
    Eventlog.add( SDS_ID, BEATCLOCK_KEY );
}

void MainWindow::Notes_per_measure( int bps_id ) // keyboard Beat per measure
{
	uint8_t bps = Bps.Bps_vec[bps_id];
	Sds->Set( sds_p->beatClock, bps );
}

void MainWindow::slideFrq( int value )
{
    Sds->Set(sds_p->features[OSCID].frq_slide_value , (uint8_t)value);
    if( value == 0 )
    	sds_p->frq_slidermode = FIXED;
    else
    	sds_p->frq_slidermode = SLIDE;
    Eventlog.add( SDS_ID, FREQDYNAMIC_KEY);
};

void MainWindow::dial_PMW_value_changed()
{
    uint8_t value = ui->hs_pmw->value();
    Sds->Set(sds_p->features[VCOID].PWM , value );
    Eventlog.add( SDS_ID,PWMDIALKEY);
}

void MainWindow::ADSR_Dialog()
{
	this->Spectrum_Dialog_p->Dialog( true );
}

void MainWindow::Spectrum_Dialog()
{
    this->Spectrum_Dialog_p->Dialog( false);
}

void MainWindow::waveform_slot(	uint8_t* wf_addr,
								uint8_t wfid,
								OSCID_e oscid,
								EVENTKEY_e wf_key,
								QLabel* label  )
{
	Sds->Set( *wf_addr, wfid );
	label->setText( QWaveform_vec[ wfid ] );
	Sds->Set( sds_p->WD_state.oscId, oscid );
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, wf_key);
}

void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &sds_p->spectrum_arr[OSCID].wfid[0], _wfid, OSCID, SETWAVEFORMMAINKEY, ui->wf_osc );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &sds_p->spectrum_arr[FMOID].wfid[0], _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &sds_p->spectrum_arr[VCOID].wfid[0], _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}

void MainWindow::select_Sds( Id_t sdsid )
{
	if( sdsid == SDS_ID )
		return; // no update without change
	if ( CutDesk_Dialog_p->isVisible() )
	{
	   	rb_S_vec[ sds_master->comId ]->setChecked( true );
		return; // no change if cutdesk is active
	}

	if( sds_master->mixer_state.composer )
		ifb.Set_Sds		(); 		// sds from comid
	else
		ifb.Set_comid	( sdsid ); // select comid by radio button
	this->Sds 			= ifb.Sds;
	this->SDS_ID		= ifb.SDS_ID;
	this->sds_p 		= ifb.sds_p;

    Info( "Activate SDS ", to_string( (int) this->sds_p->SDS_Id ));

    updateGuiVectors				( this->sds_p );
	WfDisplay_que.Set_que			( this->sds_p );
	DataDisplay->Set_sds			( this->sds_p );
	File_Dialog_p->SetSds			();// sds from comid
	Spectrum_Dialog_p->SetSds		();
	Rtsp_Dialog_p->Proc_table_update( this->sds_p, SYNTHID );
	Keyboard_Dialog_p->SetSds		();
	CutDesk_Dialog_p->SetSds		();
	set_StateButton					( ui->pBSynthesizer	,
										Appstate->GetState( sds_p		, SYNTHID),
										SYNTHESIZER );

   	rb_S_vec[ sds_master->comId ]->setChecked( true );
	Sds->Set( this->sds_p->UpdateFlag	, true);

};

void MainWindow::select_Sds0()
{
	select_Sds( 0);
}
void MainWindow::select_Sds1()
{
	select_Sds( 1);
}
void MainWindow::select_Sds2()
{
	select_Sds( 2);
}
void MainWindow::select_Sds3()
{
	select_Sds( 3);
}

auto CombineFreq = []( MainWindow* M, QCheckBox* cB, OSCID_e oscid )
{
	M->Sds->Set( M->sds_p->features[oscid].combine, cB->isChecked() );
};

void MainWindow::CombineFreq_vco()
{
	CombineFreq( this, ui->cB_Combine_vco, VCOID );
}
void MainWindow::CombineFreq_fmo()
{
	CombineFreq( this, ui->cB_Combine_fmo, FMOID );
}

void MainWindow::setStaPlay( StAId_e staid )
{
    RoleId_e role		= StaRole_map.GetRoleid( staid );
//    bool 	checked		= cb_play_sta_vec[staid].cb->isChecked();
    bool	unchecked		= not sds_p->StA_state_arr[staid].play ;

    		Sds->Set	( sds_p->StA_Id , staid );
    		if( unchecked )
    		{
    			Sds->Set( sds_p->StA_state_arr[staid].start, true );
    			set_wdrole	( role );
    		}
    		else
    		{
    			Sds->Set( sds_p->StA_state_arr[staid].play, false );
    			unset_wdrole( role );
    		}
    		Eventlog.add( SDS_ID, SETSTA_KEY );
}

void MainWindow::setStaPlay0(  )
{
	setStaPlay( STA_USER00 );
}
void MainWindow::setStaPlay1(  )
{
	setStaPlay( STA_USER01 );
}
void MainWindow::setStaPlay2(  )
{
	setStaPlay( STA_USER02 );
}
void MainWindow::setStaPlay3(  )
{
	setStaPlay( STA_USER03 );
}
void MainWindow::setStaPlay4(  )
{
	setStaPlay( STA_INSTRUMENT );
}
void MainWindow::setStaPlay5(  )
{
	setStaPlay( STA_KEYBOARD );
}
void MainWindow::setStaPlay6(  )
{
	setStaPlay( STA_NOTES );
}
void MainWindow::setStaPlay7(  )
{
	setStaPlay( STA_EXTERNAL );
}

void MainWindow::setStaStored( StAId_e staId )
{
    Sds->Set( sds_p->StA_Id, staId );
    Eventlog.add( SDS_ID, CLEAR_KEY );
}
void MainWindow::setStaStored0()
{
	setStaStored( STA_USER00 );
}
void MainWindow::setStaStored1()
{
	setStaStored( STA_USER01 );
}
void MainWindow::setStaStored2()
{
	setStaStored( STA_USER02 );
}
void MainWindow::setStaStored3()
{
	setStaStored( STA_USER03 );
}
void MainWindow::setStaStored4()
{
	setStaStored( STA_INSTRUMENT );
}
void MainWindow::setStaStored5()
{
	setStaStored( STA_KEYBOARD );
}
void MainWindow::setStaStored6()
{
	setStaStored( STA_NOTES );
}
void MainWindow::setStaStored7()
{
	setStaStored( STA_EXTERNAL );
}
void MainWindow::memory_clear()
{
	setStaStored( STAID_ALL );
}

void MainWindow::toggle_store_sta( StAId_e id )
{
	bool store = cb_store_sta_vec[id].cb->isChecked();
	Sds->Set( sds_p->StA_state_arr[id].store, store );
	Sds->Set( sds_p->StA_Id , id );
	Eventlog.add( SDS_ID, STA_RECORD_KEY );
    set_wdrole( AUDIOROLE );
};

void MainWindow::toggle_store_sta0()
{
	toggle_store_sta( STA_USER00 );
}
void MainWindow::toggle_store_sta1()
{
	toggle_store_sta( STA_USER01 );
}
void MainWindow::toggle_store_sta2()
{
	toggle_store_sta( STA_USER02 );
}
void MainWindow::toggle_store_sta3()
{
	toggle_store_sta( STA_USER03 );
}
void MainWindow::toggle_store_sta4()
{
	toggle_store_sta( STA_INSTRUMENT );
}
void MainWindow::toggle_store_sta5()
{
	toggle_store_sta( STA_KEYBOARD );
}
void MainWindow::toggle_store_sta6()
{
	toggle_store_sta( STA_NOTES );
}
void MainWindow::toggle_store_sta7()
{
	toggle_store_sta( STA_EXTERNAL );
}


void MainWindow::mixer_slider( sl_value_t map )
{
    Sds->Set( sds_p->StA_Id , map.id );
    Sds->Set( *map.value, (uint8_t)map.sl->value() );
    Eventlog.add( SDS_ID, STA_VOLUME_KEY );
};

void MainWindow::Sl_mix0( int value )
{
	mixer_slider( sl_sta_vec[STA_USER00] );
};
void MainWindow::Sl_mix1( int value )
{
	mixer_slider( sl_sta_vec[STA_USER01] );
};
void MainWindow::Sl_mix2( int value )
{
	mixer_slider( sl_sta_vec[STA_USER02] );
};
void MainWindow::Sl_mix3( int value )
{
	mixer_slider( sl_sta_vec[STA_USER03] );
};
void MainWindow::Sl_mix4( int value )
{
	mixer_slider( sl_sta_vec[STA_INSTRUMENT] );
};
void MainWindow::Sl_mix5( int value )
{
	mixer_slider( sl_sta_vec[STA_KEYBOARD] );
};
void MainWindow::Sl_mix6( int value )
{
	mixer_slider( sl_sta_vec[STA_NOTES] );
};
void MainWindow::Sl_mix7( int value )
{
	mixer_slider( sl_sta_vec[STA_EXTERNAL] );
};

void MainWindow::slideVol( int value )
{
    Sds->Set( sds_master->Master_vol_slide_value, (uint8_t)value); // % of 4*max_seconds
    Eventlog.add( MASTER_SDSID, MASTERAMP_KEY );
}

void MainWindow::clear_StAs_play()
{
	for( cb_state_t map : cb_play_sta_vec )
		map.cb->setChecked(false);
	Sds->Set( sds_p->StA_Id, STAID_ALL );
    Eventlog.add( SDS_ID, MUTESTA_KEY );
}

void MainWindow::toggle_Mute()
{
	Eventlog.add( SDS_ID, MASTERAMP_MUTE_KEY);
}

void MainWindow::update_sliderFreq( OSCID_e oscid, int idx )
{
	float freq 		= Spectrum_Dialog_p->Frequency.GetFrq( idx );
	sl_lcd_vec[oscid].lcd->display(  freq  );
};

void MainWindow::sliderFreq( OSCID_e oscid, int value )
{
	update_sliderFreq( oscid, value );
	Sds->Set( *sl_lcd_vec[oscid].value, uint8_t( value ) );
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, sl_lcd_vec[oscid].event );
};
void MainWindow::Slider_OSC_Freq( int value )
{
	sliderFreq( OSCID, value );
}
void MainWindow::Slider_VCO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_VCO_LFO->show() : ui->lb_VCO_LFO->hide();

	sliderFreq( VCOID, value );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );

}
void MainWindow::Slider_FMO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_FMO_LFO->show() : ui->lb_FMO_LFO->hide();

	sliderFreq( FMOID, value );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}

void MainWindow::sliderVolume( sl_lcd_t map )
{
	uint8_t value = map.sl->value();
	Sds->Set( *map.value, value);
	map.lcd->display( value );
	Eventlog.add( SDS_ID, map.event);
};

void MainWindow::Main_slot_volume()
{
	sliderVolume( sl_volume_vec[ OSCID ] );
}
void MainWindow::VCO_slot_volume()
{
	sliderVolume( sl_volume_vec[ VCOID ] );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}
void MainWindow::FMO_slot_volume()
{
	sliderVolume( sl_volume_vec[ FMOID ] );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
}

void MainWindow::Slider_VCO_Adjust( int value )
{
	Sds->Set( sds_p->features[VCOID].adjust, (uint8_t) value);
	Sds->Set( sds_p->WD_state.oscId, VCOID );
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, ADJUST_KEY);
}
void MainWindow::Slider_FMO_Adjust( int value )
{
	Sds->Set( sds_p->features[FMOID].adjust, (uint8_t) value);
	Sds->Set( sds_p->WD_state.oscId, FMOID );
	set_wdrole( INSTRROLE );
	Eventlog.add( SDS_ID, ADJUST_KEY );

}

void MainWindow::changeAppState( APPID_e appid, string terminal, string args  )
{

	Id_t sdsid = App.ChangeAppState( sds_p, appid, terminal, args );
	select_Sds( sdsid );
	if( Rtsp_Dialog_p->isVisible() )
		Rtsp_Dialog_p->Proc_table_update_all();

}

void MainWindow::start_audio_srv()
{
	changeAppState( AUDIOID, Cfg_p->Config.Nohup );
}
void MainWindow::start_synthesizer()
{
	changeAppState( SYNTHID, Cfg_p->Config.Nohup );
}
void MainWindow::start_keyboard()
{
	changeAppState( KEYBOARDID, Cfg_p->Config.Term );
}
void MainWindow::start_sdsview()
{
	changeAppState( SDSVIEWID, Cfg_p->Config.Term );
}
void MainWindow::start_composer()
{
	changeAppState( COMPOSERID, Cfg_p->Config.Term, sds_master->Session );
}

void MainWindow::Save_Config()
{
	Eventlog.add( SDS_ID, SAVEDEFAULT_KEY );
}

void MainWindow::connect_secundary( OSCID_e oscid, OSCID_e secid, char mode, bool val )
{
	if ( mode == CONF )
	{
		if( val )
			Sds->Set( sds_p->connect_arr[oscid].frq, secid );
		else
			Sds->Set( sds_p->connect_arr[oscid].frq, oscid );
	}
	if ( mode == CONV )
	{
		if( val )
			Sds->Set( sds_p->connect_arr[oscid].vol, secid );
		else
			Sds->Set( sds_p->connect_arr[oscid].vol, oscid );
	}
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );
};

void MainWindow::connect_oscf( bool val )
{
	connect_secundary( OSCID, FMOID, CONF, val );
}
void MainWindow::connect_oscv( bool val )
{
	connect_secundary( OSCID, VCOID, CONV, val );
}
void MainWindow::connect_fmov( bool val )
{
	connect_secundary( FMOID, VCOID, CONV, val );
}
void MainWindow::connect_vcov( bool val )
{
	connect_secundary( VCOID, FMOID, CONV, val );
}

void MainWindow::update_progressbar( )
{
    int pb_value = sds_master->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}

void MainWindow::capture_audio()
{
	StateId_e state = sds_master->Capture_state;
	if ( state == StateId_e::RECORDING )
	{
		state = StateId_e::INACTIVE;
		unset_wdrole( EXTERNALROLE );
	}
	else
	{
		state = StateId_e::RECORDSTART;
		set_wdrole( EXTERNALROLE );
	}
	Sds->Set( sds_master->Capture_state, state );
	Eventlog.add( SDS_ID, CAPTURE_KEY );
}

void MainWindow::set_overwritemode( bool mode )
{
	sds_master->overwritemode = mode;
}
void MainWindow::SaveRecord()
{
	// define wav file
	if( sds_master->overwritemode )
		Sds->Set( sds_master->FileNo, 0_uint ); // take filename from the Other-cstring
	else
		Sds->Set( sds_master->FileNo ,1_uint ); // 1=automatic numbering
												// see also: Config.MAXWAVFILES, fs->counter_file

    if ( sds_master->Record_state == StateId_e::RECORDING )
    {
        Eventlog.add( SDS_ID, STOPRECORD_KEY );	// event receiver is Synthesizer eventhandler
    }											// event data is managed by AudioServer
    if ( sds_master->Record_state == StateId_e::INACTIVE )
    {
        Eventlog.add( SDS_ID, STARTRECORD_KEY );// Audioserver is informed indirectly by
        										// Synthesizer or Composer
        										// considering synchronization with other items
    }
}

void MainWindow::update_CB_external()
{
	Info( "update recording info");
	EventWAV.Update_cb();
}

void MainWindow::setwidgetvalues()
{
	Comment( DEBUG, "MainWindow::setwidgetvalues" );
	if( Appstate->IsExitserver( sds_master, GUI_ID ) ) // externally triggered shutdown of th GUI
		GUI_Exit();

	if( sds_master->mixer_state.composer )
		select_Sds( sds_master->comId );

	for ( int oscid : oscIds )
	{
		int val = *sl_volume_vec[oscid].value;
		sl_volume_vec[oscid].sl->setValue( val );
		sl_volume_vec[oscid].lcd->display( val );

		int idx = sds_p->spectrum_arr[oscid].frqidx[0];
		sl_lcd_vec[oscid].sl->setValue( idx );
		sl_lcd_vec[oscid].lcd->display( frqArray[ idx ]);

			idx = *sB_lbl_vec[oscid].value;
		sB_lbl_vec[oscid].lbl->setText( QWaveform_vec[ idx ] );
		sB_lbl_vec[oscid].sb->setValue( idx );
	};
    ui->hs_pmw->setValue			( (int) sds_p->features[VCOID].PWM  );
    ui->hs_hall_effect->setValue	( (int) sds_p->adsr_arr[OSCID].hall );
    ui->Slider_slideFrq->setValue	( (int) sds_p->features[OSCID].frq_slide_value );
    ui->Slider_slideVol->setValue	( sds_master->Master_vol_slide_value);//Master_Amp);
    ui->hs_balance->setValue		( sds_master->mixer_balance );
    ui->cb_overwrite->setChecked	( sds_master->overwritemode );

    update_progressbar();

    QString
	Qstr 	= sds_p->mixer_state.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    Qstr	= int2char( sds_p->beatClock );
    ui->CB_kbd_bps->setCurrentText( Qstr );
    Qstr	= int2char( sds_p->instrumentClock );
    ui->CB_inst_bps->setCurrentText( Qstr );

	set_sl_sta_value	( this ); // consider feedback from the synthesizer/composer
	set_cb_filled_value	( this ); // consider feedback from the composer
	set_cb_psta_value	( this ); // "

	QPalette	pBar_palette	= QPalette	{};
	if( sds_p->overload )
	{
		pBar_palette.setColor( QPalette::Active, QPalette::Button, Qt::yellow );
		ui->Pbar_telapsed->setPalette( pBar_palette );
		sds_p->overload = false;
	}
	else
	{
		pBar_palette.setColor( QPalette::Active, QPalette::Button, Qt::blue );
		ui->Pbar_telapsed->setPalette( pBar_palette );
	}
	ui->Pbar_telapsed->setValue( sds_p->time_elapsed );

	WD_data_t	wd_data	= sds_p->WD_state;
	WdModeID_t	wd_mode	= check_cycle2( wd_mode_range, sds_p->WD_state.mode );
	ui->pB_wd_mode->setText( Qwd_wdmode_names[ wd_mode ] );
	ui->pB_WdRole->setText( Qwd_role_names[ wd_data.roleId ] );
    ui->pB_oscgroup->setText( Qwd_osc_names[ wd_data.oscId ] );

   	ui->cB_Combine_fmo->setChecked( sds_p->features[FMOID].combine );
   	ui->cB_Combine_vco->setChecked( sds_p->features[VCOID].combine );

	ui->cb_connect_fmov->setChecked( (bool)(sds_p->connect_arr[FMOID].vol != FMOID) );
	ui->cb_connect_vcov->setChecked( (bool)(sds_p->connect_arr[VCOID].vol != VCOID) );
	ui->cb_connect_oscv->setChecked( (bool)(sds_p->connect_arr[OSCID].vol == VCOID) );
	ui->cb_connect_oscf->setChecked( (bool)(sds_p->connect_arr[OSCID].frq == FMOID) );

    double frq_slide_duration = sds_p->features[OSCID].frq_slide_value * max_sec * percent;
    ui->lbl_frqglide_sec->setText( QString::number( frq_slide_duration ) + "\n[sec[");

    // adsr mode is set to false by the wave display class
    // re calculate adsr data if needed
    Sds->Set( sds_p->WD_state.adsrmode, Spectrum_Dialog_p->isVisible() ); // gen. adsr data or not

	if( sds_p->StA_state_arr[ STA_KEYBOARD ].play  )
	{
		if ( not sds_master->mixer_state.composer )
		{
			if( not Keyboard_Dialog_p->isVisible() )
			{
				Keyboard_Dialog_p->Setup_Widget();

				Keyboard_Dialog_p->show();
				this->activateWindow();
				this->setFocus();
			}
		}
	}
	else
	{
		if( Keyboard_Dialog_p->isVisible() )
		{
			Keyboard_Dialog_p->hide();
		}
	}

    updateColorButtons();
}

void MainWindow::updateColorButtons()
{
	Comment( DEBUG, "MainWindow::updateColorButtons" );

	set_StateButton( ui->pBAudioServer	, Appstate->GetState( sds_master, AUDIOID ), AUDIOSERVER );
	set_StateButton( ui->pBSynthesizer	, Appstate->GetState( sds_p		, SYNTHID), SYNTHESIZER );
	set_StateButton( ui->pb_Keyboard	, Appstate->GetState( sds_p		, KEYBOARDID), SYNTHKBD );
	set_StateButton( ui->pb_SDSview		, Appstate->GetState( sds_master, SDSVIEWID), SDSVIEW );
	set_StateButton( ui->pBComposer		, Appstate->GetState( sds_master, COMPOSERID),COMPOSER );
    set_StateButton( ui->pBtoggleRecord	, sds_master->Record_state	, "Record" );
    set_StateButton( ui->pB_Capture		, sds_master->Capture_state	, "Capture" );

    setButton( ui->pB_Cutter	, CutDesk_Dialog_p->isVisible()	*2 + 1 );
    setButton( ui->pB_Rtsp		, Rtsp_Dialog_p->isVisible()	*2 + 1 );
    setButton( ui->pB_play_notes, File_Dialog_p->isVisible()	*2 + 1 );
    setButton( ui->pB_Spectrum	, Spectrum_Dialog_p->isVisible()*2 + 1 );
    setButton( ui->pB_Mute		, sds_p->mixer_state.mute		+ 1  );

}

void MainWindow::Update_Statusbar( QString str )
{
	stringstream strs {};
	strs << left <<
			setw(20) <<	"Scorename: " <<
			setw(20) << sds_master->Session <<
			setw(20) << "Notes: "  <<
			setw(20) << sds_p->Notes <<
			setw(20) << "Instrument: " <<
			setw(20) << sds_p->Instrument;
    ui->statusbar->showMessage( Qstring( strs.str() ));
}
