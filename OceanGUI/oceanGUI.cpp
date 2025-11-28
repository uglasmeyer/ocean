/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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

// OceanGUI
#include <include/Spectrum_dialog.h>
#include <include/Mainwindow.h>
#include <include/Oszilloscopewidget.h>
#include <ui_mainwindow.h>

// Ocean
#include <EventKeys.h>
#include <Wavedisplay_base.h>
#include <Logfacility.h>
#include <Mixer.h>
#include <Ocean.h>
#include <Kbd.h>
#include <Mixerbase.h>

// Qt
#include <QLabel>
#include <QPolygon>
#include <QTimer>
#include <QRect>


const string 		Module 		= OCEANGUI;
sta_role_map 		StaRole_map = sta_role_map();

//Ui::MainWindow	Ui_Mainwindow_obj{};

auto set_cb_ssta_value = [ ]( MainWindow* M )
{

	for( MainWindow::cb_state_t map : M->cb_store_sta_vec )
    {
    	map.cb->setChecked( (bool) *map.state );
    }
};
auto set_cb_psta_value = [ ]( MainWindow* M )
{
    for( MainWindow::cb_state_t map : M->cb_play_sta_vec )
    	map.cb->setChecked( *map.state );
};
auto set_sl_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::sl_value_t map : M->sl_sta_vec )
    	map.sl->setValue( *map.value );
};
auto set_cb_filled_value = [  ]( MainWindow* M  )
{
	for( MainWindow::cb_state_t map : M->cb_filled_sta_vec )
		map.cb->setChecked( *map.state );
};

MainWindow::MainWindow(	QWidget *parent ) :
		Logfacility_class( "OceanGUI" ),
		osc_struct(),
		ui(new Ui::MainWindow{} )
{
	className		= Logfacility_class::className;
	ui->setupUi(this);
	initPanel();
	initLables();

	Sds->Set( sds_master->UpdateFlag, true);
	Sds->Set( Sds->addr->UpdateFlag, true);

    initGuiVectors( Sds->addr );

	initStateButtons();
    initOscillatorDisplay();
    initFreqSlider();
    initScrollbars();
    initComboBoxes();

    initUiConnectors();
    initTimer();
}

MainWindow::~MainWindow()
{
	DESTRUCTOR( className );
	printf( "%p\n\n", OscWidget_item );
	if ( OscWidget_item ) delete ( OscWidget_item );
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	QString 		QStr	{};
	Kbd_base::
	key_union_t 	key 	{};
					key.Int = Keymap.Qt_key( event->key() ) ;
	switch ( key.Int )
	{
		case Qt::Key_Escape :
		{
			GUI_Exit();
			break;
		}
		default :
		{
			sds->Kbd_state.key = key.Int;
			QMainWindow::keyPressEvent( event );
			break;
		}
	} // switch  key
	QStr = "";
	QStr.push_back( (QChar)key.Arr[0] );
//	QStr.assign(1, (QChar)key.Arr[0] );
	Keyboard_Dialog_p->keyboard_key = QStr;
	Keyboard_Dialog_p->Setup_Widget();
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
	Sds->Set( Sds->addr->noteline_prefix.chord_delay, value );
}
void MainWindow::set_wdrole( RoleId_e roleid )
{
	if( roleid == ROLE_SIZE ) return;
	ui->pB_Wavedisplay->setText( Qwd_role_names[ roleid ] );

    Sds->Set( sds->WD_status.roleId, roleid );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );
}
void MainWindow::wavfile_selected( const QString &arg)
{
    string str = arg.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( WAVFILESTR_KEY, str );
        Eventlog.add( SDS_ID, READ_EXTERNAL_WAVFILE);
        set_wdrole( EXTERNALROLE );
    }
}

void MainWindow::Capture( QString str )
{
	Sds->capture_flag = Eventlog.capture( SDS_ID, not Sds->capture_flag );
    (Sds->capture_flag) ? 	ui->cB_Capture->setCurrentText( QCapture_str_lst[Eventlog.CAPTURING] ):
    						ui->cB_Capture->setCurrentText( QCapture_str_lst[Eventlog.SPOOLING ] );
}

void MainWindow::adsr_hall( )
{
	uint8_t value = ui->hs_hall_effect->value();
    Sds->Set(Sds->addr->adsr_arr[OSCID].hall , value);
    Eventlog.add( SDS_ID, ADSR_KEY);
}

void MainWindow::cB_Beat_per_sec( int bps_id )
{
	uint8_t bps = bps_struct().Bps_vec[bps_id];
    Sds->Set( Sds->addr->adsr_arr[OSCID].bps, bps  );
    Sds->Set( Sds->addr->adsr_arr[VCOID].bps, bps  );
    Sds->Set( Sds->addr->adsr_arr[FMOID].bps, bps  );
	Eventlog.add( SDS_ID, ADSRALL_KEY );

	if ( Spectrum_Dialog_p )
	{
	    QString Qstr{ int2char( Sds->addr->adsr_arr[OSCID].bps ) };
	    Spectrum_Dialog_p->ui->cb_bps->setCurrentText( Qstr );
	}
	MainWindow::setFocus();
}

void MainWindow::slideFrq( int value )
{
    Sds->Set(Sds->addr->features[OSCID].glide_effect , (uint8_t)value);
    Eventlog.add( SDS_ID, SOFTFREQUENCYKEY);

};


void MainWindow::dial_PMW_value_changed()
{
    uint8_t value = ui->hs_pmw->value();
    Sds->Set(Sds->addr->features[VCOID].PMW_dial , value );
    Eventlog.add( SDS_ID,PWMDIALKEY);
}

template< class Dialog>
auto switch_dialog( Dialog* p )
{
	if( p->isVisible() )
		p->hide();
	else
		p->show();
}
void MainWindow::Rtsp_Dialog()
{
	switch_dialog( this->Rtsp_Dialog_p );
    Rtsp_Dialog_p->Proc_table_update_all( );
}
void MainWindow::SDS_Dialog()
{
	if( Appstate->IsRunning( sds_master, SDSVIEWID ) )
	{
		Appstate->SetExitserver( sds_master, SDSVIEWID );
		Sem_p->Lock( SEMAPHORE_EXIT);
	}
	else
	{
		string Start_Comstack = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs->comstack_bin, "" );
		System_execute( Start_Comstack );
	}
	return;

}


void MainWindow::ADSR_Dialog()
{
    switch_dialog( this->Spectrum_Dialog_p );
    set_wdrole( ADSRROLE );

	Spectrum_Dialog_p->setGeometry(Spectrum_Dialog_Rect );
	Spectrum_Dialog_p->Set_adsr_flag( true );
}

void MainWindow::File_Director()
{
	switch_dialog( this->File_Dialog_p );
    File_Dialog_p->Setup_widgets();
}

void MainWindow::Spectrum_Dialog()
{
    switch_dialog( this->Spectrum_Dialog_p );
    set_wdrole( INSTRROLE );

    Spectrum_Dialog_p->setGeometry(Spectrum_Dialog_Rect );
    this->Spectrum_Dialog_p->Set_adsr_flag( false );
}


void MainWindow::waveform_slot(	uint8_t* wf_addr,
								uint8_t wfid,
								int ID,
								EVENTKEY_e wf_key,
								QLabel* label  )
{
	Sds->Set( *wf_addr, wfid );
	Eventlog.add( SDS_ID, wf_key);
	label->setText( QWaveform_vec[ wfid ] );
	MainWindow::setFocus();

}
void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->spectrum_arr[OSCID].wfid[0], _wfid, OSCID, SETWAVEFORMMAINKEY, ui->wf_OSC );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &Sds->addr->spectrum_arr[FMOID].wfid[0], _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->spectrum_arr[VCOID].wfid[0], _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}

void MainWindow::select_Sds( Id_t sdsid )
{
	Sds->Set( this->sds_master->config, sdsid );
	Sds->Set( this->sds_master->UpdateFlag	, true);
	Sds->Set( this->Sds->addr->UpdateFlag	, true); // update old sds

	this->Sds = DaTA.SDS.GetSds( sdsid ); // set new sds
	Sds->Set( this->SDS_ID, sdsid );
	this->sds = Sds->addr;

	initGuiVectors( this->sds );
	initStateButtons();

    Info( "Activate SDS ", to_string( (int) Sds->addr->SDS_Id ));

	File_Dialog_p->SetSds( this->Sds, sdsid );
	Spectrum_Dialog_p->SetSds( this->Sds, sdsid );

	Rtsp_Dialog_p->Proc_table_update( this->sds, SYNTHID );

   	rb_S_vec[ sds_master->config ]->setChecked( true );

	setwidgetvalues();
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

void MainWindow::CombineFreq()
{
	if ( ui->cB_Combine->isChecked() )
		Sds->Set( Sds->addr->frq_slidermode,  COMBINE );
	else
		Sds->Set( Sds->addr->frq_slidermode, SLIDE );
}

void MainWindow::setStaPlay( StAId_e staid )
{
    Sds->Set( Sds->addr->MIX_Id , staid );

    bool	play= not Sds->addr->StA_state_arr[staid].play;
    Sds->Set( Sds->addr->StA_state_arr[staid].play, play);
    if( play )
   	{
    	RoleId_e role = StaRole_map.GetRoleid( staid );
        set_wdrole( role );
        Eventlog.add( SDS_ID, RESET_STA_SCANNER_KEY );
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
    Sds->Set( Sds->addr->MIX_Id , staId );
    bool filled = not Sds->addr->StA_state_arr[staId].filled;
    Sds->Set( Sds->addr->StA_state_arr[staId].filled, filled );
    Eventlog.add( SDS_ID, RESET_STA_SCANNER_KEY );
    Eventlog.add( SDS_ID, SETSTA_KEY );
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

void MainWindow::toggle_store_sta( StAId_e id )
{

	Sds->Set( Sds->addr->MIX_Id , id );
    if ( Sds->addr->StA_state_arr[id].store )
    {
    	Eventlog.add( SDS_ID, STARECORD_STOP_KEY);
    }
    else
    {
        Eventlog.add( SDS_ID, STARECORD_START_KEY);
    }
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


void MainWindow::memory_clear()
{
	for( uint id : StAMemIds )
	{
		*cb_filled_sta_vec[id].state = false;
	}
    Eventlog.add( SDS_ID, CLEAR_KEY );
}

void MainWindow::mixer_slider( sl_value_t map )
{
    Sds->Set( Sds->addr->MIX_Id , map.id );
    Sds->Set( *map.value, (uint8_t)map.sl->value() );
    Eventlog.add( SDS_ID, SETSTA_KEY);
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
    Sds->Set( sds_master->slide_duration, (uint8_t)value); // % of 4*max_seconds
    Eventlog.add( 0, MASTERAMP_KEY);
}

void MainWindow::Clear_Banks()
{
	for( cb_state_t map : cb_play_sta_vec )
		map.cb->setChecked(false);
    Eventlog.add( SDS_ID, MUTEMBKEY);
}
void MainWindow::toggle_Mute()
{
    bool mute_flag 	= not Sds->addr->mixer_state.mute ;
    Eventlog.add( SDS_ID, MASTERAMP_MUTE_KEY);
    QString Qstr = mute_flag ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
}


void MainWindow::sliderFreq( sl_lcd_t map, uint8_t value )
{
	float freq 		= Spectrum.GetFrq( value );
	map.lcd->display(  freq  );
	Sds->Set( *map.value, value );

	Eventlog.add( SDS_ID, map.event );
};
void MainWindow::Slider_OSC_Freq( int value )
{
	sliderFreq( sl_frqidx_vec[OSCID], value );
}
void MainWindow::Slider_VCO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_VCO_LFO->show() : ui->lb_VCO_LFO->hide();

	sliderFreq( sl_frqidx_vec[VCOID], value );
	Eventlog.add( SDS_ID, CONNECTOSC_KEY );

}
void MainWindow::Slider_FMO_Freq( int value )
{
	( (uint)value < C0 ) ? ui->lb_FMO_LFO->show() : ui->lb_FMO_LFO->hide();

	sliderFreq( sl_frqidx_vec[FMOID], value );
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
	Sds->Set( Sds->addr->features[VCOID].adjust, (uint8_t) value);
	Eventlog.add( SDS_ID, ADJUST_KEY);
}
void MainWindow::Slider_FMO_Adjust( int value )
{
	Sds->Set( Sds->addr->features[FMOID].adjust, (uint8_t) value);
	Eventlog.add( SDS_ID, ADJUST_KEY );
}

void MainWindow::start_composer()
{

    string Start_Composer = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs->composer_bin, "" );
	System_execute( Start_Composer );
}


void MainWindow::start_audio_srv()
{
	if( Appstate->IsRunning( sds_master, RTSPID ) )
		return;
	if( Appstate->IsRunning( sds_master, AUDIOID))
	{
		Appstate->SetExitserver( sds_master, AUDIOID) ;
		return;
	}

    string Start_Audio_Srv = Cfg_p->Server_cmd( Cfg_p->Config.Nohup, fs->audio_bin,
			" > " + fs->nohup_file);

	System_execute( Start_Audio_Srv.data() );

}

auto start_synth = [  ]( MainWindow* M, string cmd )
{
	if( M->Appstate->IsRunning( M->sds_master, RTSPID ) )
	{
	    if ( M->Sem_p->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
	    	M->Sem_p->Release( SYNTHESIZER_START );
	    return M->SDS_ID;
	}

	Id_t sdsid = M->DaTA.Appstate.GetNextSdsId( );
	if ( sdsid < 0 ) return M->SDS_ID;

    System_execute( cmd.data() );
    return sdsid;

};
void MainWindow::start_synthesizer()
{
	if( Appstate->IsRunning( Sds->addr, SYNTHID ) )
	{
		exit_synthesizer( SYNTHID );
	}
	else
	{
		string cmd = Cfg_p->Server_cmd( Cfg_p->Config.Nohup, fs->synth_bin," >> " + fs->nohup_file );
		uint8_t sdsid = start_synth( this, cmd );
		select_Sds(sdsid);
	}

}
void MainWindow::start_keyboard()
{
	if( Appstate->IsRunning( Sds->addr, KEYBOARDID ))
	{
		exit_synthesizer( KEYBOARDID );
	}
	else
	{
		string cmd = Cfg_p->Server_cmd( Cfg_p->Config.Term, fs->Keyboard_bin, "" );
		int sdsid = start_synth( this, cmd );
		select_Sds(sdsid);
	}
}
void MainWindow::read_polygon_data()
{
    OscWidget_item->read_polygon_data();
};

void MainWindow::exit_synthesizer( APPID appid )
{
	if ( Appstate->IsRunning( sds_master, RTSPID ) )
	{
		if ( Sem_p->Getval( SEMAPHORE_EXIT, GETVAL ) > 0 )
			Sem_p->Release( SEMAPHORE_EXIT );
		return;
	}
	Appstate->SetExitserver( Sds->addr, appid );
    DaTA.Sem_p->Lock( SEMAPHORE_EXIT, 2 );
	setwidgetvalues();
}

void MainWindow::Save_Config()
{
    Sds->Write_str( INSTRUMENTSTR_KEY, "default");
	Eventlog.add( SDS_ID, SAVEINSTRUMENTKEY);
}

auto connect_secundary = [  ]( MainWindow* M, OSCID_e oscid, OSCID_e secid, char mode, bool val )
{
	if ( mode == CONF )
	{
		if( val )
			M->Sds->Set( M->sds->connect_arr[oscid].frq, secid );
		else
			M->Sds->Set( M->sds->connect_arr[oscid].frq, oscid );
	}
	if ( mode == CONV )
	{
		if( val )
			M->Sds->Set( M->sds->connect_arr[oscid].vol, secid );
		else
			M->Sds->Set( M->sds->connect_arr[oscid].vol, oscid );
	}
	M->Eventlog.add( M->SDS_ID, CONNECTOSC_KEY );
};

void MainWindow::connect_oscf( bool val )
{
	connect_secundary( this, OSCID, FMOID, CONF, val );
}
void MainWindow::connect_oscv( bool val )
{
	connect_secundary( this, OSCID, VCOID, CONV, val );
}
void MainWindow::connect_fmov( bool val )
{
	connect_secundary( this, FMOID, VCOID, CONV, val );
}
void MainWindow::connect_vcov( bool val )
{
	connect_secundary( this, VCOID, FMOID, CONV, val );
}

void MainWindow::get_record_status( )
{
    int pb_value = sds_master->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}

void MainWindow::SaveRecord() // TODO no yet ready
{

	if( ui->cb_overwrite->isChecked() )
		Sds->Set( sds_master->FileNo, 0_uint ); // take filename from the Other-cstring
	else
		Sds->Set( sds_master->FileNo ,1_uint ); // 1=automatic numbering
	// see also: Config.MAXWAVFILES, fs->counter_file

    if ( sds_master->Record_state == sdsstate_struct::RECORDING )
    {
        Eventlog.add( SDS_ID, STOPRECORD_KEY );
    }
    if ( sds_master->Record_state == sdsstate_struct::INACTIVE )
    {
        Eventlog.add( SDS_ID, STARTRECORD_KEY );
    }
}


void MainWindow::pB_Debug_clicked()
{
    uint8_t counter = Sds->addr->WD_status.wd_mode;
	counter = ( counter + 1 ) % WD_MODE_SIZE;
    Sds->Set( Sds->addr->WD_status.wd_mode , (WdModeID_t)counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );

    ui->pB_wd_mode->setText( Qwd_wdmode_names[ (int)counter ] );
}

void MainWindow::pB_oscgroup_clicked()
{
    uint8_t counter = Sds->addr->WD_status.oscId ;
	counter = ( counter + 1 ) % WD_OSC_SIZE;
    Sds->Set( Sds->addr->WD_status.oscId, (OSCID_e)counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY);

    ui->pB_oscgroup->setText( Qwd_osc_names[ (int)counter ] );
}

void MainWindow::pB_Wavedisplay_clicked()
{
	uint8_t counter = Sds->addr->WD_status.roleId ;
	counter = ( counter + 1 ) % WD_ROLES_SIZE;
    Sds->Set( Sds->addr->WD_status.roleId , (RoleId_e) counter );
    Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY);

    ui->pB_Wavedisplay->setText( Qwd_role_names[ counter ] );
};

void MainWindow::pB_fftmode_clicked()
{
	bool fft_mode = not (Sds->addr->WD_status.fftmode );
	Sds->Set( Sds->addr->WD_status.fftmode, fft_mode );
	Eventlog.add( SDS_ID, SETWAVEDISPLAYKEY );

	ui->pb_fftmode->setText( Qwd_fftmodes[ (int)fft_mode ] );
}

void MainWindow::update_CB_external()
{
	Info( "update recording info");
	CB_external->clear();
	CB_external->addItems( Qread_filenames( EventWAV.path ));
	QString Qfile	= QReadStr( Sds, EventWAV.event );
	CB_external->setCurrentText( Qfile );
}

void MainWindow::setwidgetvalues()
{
	if( Appstate->IsExitserver( App.sds, GUI_ID ) )
		GUI_Exit();
	else
		App.Appstate->SetRunning(  );

	for ( int oscid : oscIds )
	{
		sl_volume_vec[oscid].sl->setValue( *sl_volume_vec[oscid].value );
		sl_volume_vec[oscid].lcd->display( *sl_volume_vec[oscid].value );

		sl_frqidx_vec[oscid].sl->setValue( *sl_frqidx_vec[oscid].value );
		sl_frqidx_vec[oscid].lcd->display( Spectrum.GetFrq( *sl_frqidx_vec[oscid].value ));

		sB_lbl_vec[oscid].lbl->setText( QWaveform_vec[ *sB_lbl_vec[oscid].value] );
		sB_lbl_vec[oscid].sb->setValue( *sB_lbl_vec[oscid].value );
	};
    ui->hs_pmw->setValue			( (int) Sds->addr->features[VCOID].PMW_dial  );
    ui->hs_hall_effect->setValue	( (int) Sds->addr->adsr_arr[OSCID].hall );
    ui->Slider_slideFrq->setValue	( (int) Sds->addr->features[OSCID].glide_effect );
    ui->Slider_slideVol->setValue	( sds_master->slide_duration);//Master_Amp);
    ui->hs_balance->setValue		( sds_master->mixer_balance );

    get_record_status();


    QString
	Qstr = Sds->addr->mixer_state.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    Qstr	= int2char( Sds->addr->adsr_arr[OSCID].bps );
    ui->cb_bps->setCurrentText( Qstr );


	set_cb_psta_value( this );
	set_cb_ssta_value( this );
	set_sl_sta_value( this );
	set_cb_filled_value( this );

	ui->Pbar_telapsed->setValue( Sds->addr->time_elapsed );

	if ( Sds->addr->WD_status.roleId == AUDIOROLE )
		OscWidget_item->sds = sds_master;
	else
		OscWidget_item->sds = this->Sds->addr;

	if( Spectrum_Dialog_p->isVisible( ))
		Spectrum_Dialog_p->Update_instrument();
    ui->pB_Wavedisplay->setText( Qwd_role_names[ Sds->addr->WD_status.roleId ] );
    ui->pB_oscgroup->setText( Qwd_osc_names[ Sds->addr->WD_status.oscId ] );
	if( Rtsp_Dialog_p->isVisible() )
		Rtsp_Dialog_p->Proc_table_update_all();


   	ui->cB_Capture->setCurrentText( QCapture_str_lst[ Eventlog.capture_state ] );

   	bool combine = ( Sds->addr->frq_slidermode == COMBINE );
   	ui->cB_Combine->setChecked( combine );
   	//    rb_S_vec[ Sds->addr->SDS_Id ]->setChecked( true );

	ui->cb_connect_fmov->setChecked ( (bool)(Sds->addr->connect_arr[FMOID].vol != FMOID));
	ui->cb_connect_vcov->setChecked ( (bool)(Sds->addr->connect_arr[VCOID].vol != VCOID) );
	ui->cb_connect_oscv->setChecked( (bool)(Sds->addr->connect_arr[OSCID].vol != OSCID) );
	ui->cb_connect_oscf->setChecked( (bool)(Sds->addr->connect_arr[OSCID].frq != OSCID) );

    double frq_slide_duration = Sds->addr->features[OSCID].glide_effect * max_sec * percent;
    ui->lbl_frqglide_sec->setText( QString::number( frq_slide_duration ) + "\n[sec[");
    updateColorButtons();

    bool kbd_up=Appstate->IsRunning( sds, APPID::KEYBOARDID );
	if( kbd_up  )
	{
		if( not Keyboard_Dialog_p->isVisible() )
			Keyboard_Dialog_p->show();
		Keyboard_Dialog_p->Setup_Widget();

	}
	else
	{
		if( Keyboard_Dialog_p->isVisible() )
			Keyboard_Dialog_p->hide();
	}


}

void MainWindow::updateColorButtons()
{

	auto isRunning = [ this ]( APPID appid, interface_t* sds )
	{
		return Appstate->IsRunning( sds , appid );
	};

	uint audio_state		=	isRunning( AUDIOID, sds_master ) ;
	uint synthesizer_state	= 	isRunning( SYNTHID, sds )*2 + // red or yellow
								audio_state;   // red or yellow or green
	bool keyboard_up		= 	isRunning( APPID::KEYBOARDID, sds );
	uint keyboard_state 	= 	keyboard_up*2 + audio_state;
    setButton( ui->pBSynthesizer, synthesizer_state );
    setButton( ui->pb_Keyboard	, keyboard_state );
    setButton( ui->pBAudioServer, audio_state*3 ); // red or green
    setButton( ui->pb_SDSview	, isRunning( SDSVIEWID, sds_master )*2+1 );
    setButton( ui->pBComposer	, isRunning( COMPID, sds_master )*2+1 );


    // TODO complete
    bool record 	= ( sds_master->Record_state == sdsstate_struct::RECORDING );
    bool stopping	= ( sds_master->Record_state == sdsstate_struct::STOPPING );
    bool starting	= ( sds_master->Record_state == sdsstate_struct::STARTING );
    if( stopping or starting )
        setButton( ui->pBtoggleRecord, 2 ); // yellow
    else
    	setButton( ui->pBtoggleRecord, not record );
    ( record ) ? 	ui->pBtoggleRecord->setText( "Stop Rec") :
    				ui->pBtoggleRecord->setText( "Record");

    if( Rtsp_Dialog_p->isVisible() )
    	setButton( ui->pB_Rtsp, 3 );
    else
        setButton( ui->pB_Rtsp, 1 );

    if( File_Dialog_p->isVisible() )
        setButton( ui->pB_play_notes, 3 );
    else
        setButton( ui->pB_play_notes, 1 );

    if( Spectrum_Dialog_p->isVisible() )
        setButton( ui->pB_Specrum, 3 );
    else
        setButton( ui->pB_Specrum, 1 );


}
