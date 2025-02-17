
// qtcreator
#include <Mainwindow.h>
#include <ui_mainwindow.h>
#include <Oszilloscopewidget.h>
#include <Spectrum_dialog_class.h>

// Synhesizer
#include <Keys.h>
#include <Wavedisplay_base.h>
#include <Logfacility.h>




#include <Mixer.h>
#include <Ocean.h>

// Qt
#include <QLabel>
#include <QByteArray>
#include <QPolygon>
#include <QTimer>
#include <QRect>
#include <QEvent>




const string 		Module 		= "OceanGUI";

//Ui::MainWindow	Ui_Mainwindow_obj{};

auto set_rb_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::rb_state_t map : M->rb_sta_vec )
    	map.rb->setChecked( *map.state );
};
auto set_cb_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::cb_state_t map : M->cb_sta_vec )
    	map.cb->setChecked( *map.state );
};
auto set_sl_sta_value = [ ]( MainWindow* M )
{
    for( MainWindow::sl_value_t map : M->sl_sta_vec )
    	map.sl->setValue( *map.value );
};



MainWindow::MainWindow(	QWidget *parent ) :
	Logfacility_class( Module ),
	ui(new Ui::MainWindow() )

{
//	ui = &Ui_Mainwindow_obj;
    ui->setupUi(this);

    initPanel();

//	select_Sds(Sds_master->config );
    Sds_master->UpdateFlag = true;
    Sds->addr->UpdateFlag = true;
    Sds_master->Record = false;

    initWavedisplay();
    initOscWidget();
    initMixerVector();
    initFreqSlider();
    initScrollbars();
    initComboBoxes();



    initUiConnectors();
    initTimer();
}

MainWindow::~MainWindow()
{
	delete OscW_item;
//	if ( ui ) delete ui;
}

void MainWindow::GUI_Exit()
{
    qDebug("%s", "Exit" );
    QApplication::quit();
}

void MainWindow::wavfile_selected( const QString &arg)
{
    qDebug() << "WAV file " << arg ;
    QString QStr = arg;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        Sds->Write_str( WAVEFILESTR_KEY, str );
        Sds->Set(Sds->addr->KEY , READ_EXTERNALWAVEFILE);
    }
}


void MainWindow::hs_hall_effect_value_changed(int value)
{
    Sds->Set(Sds->addr->OSC_adsr.hall , value);
    Sds->Set( Sds->addr->KEY , ADSR_KEY);
}


void MainWindow::dial_soft_freq_value_changed()
{
    int value = ui->dial_soft_freq->value();
    Sds->Set(Sds->addr->OSC_wp.glide_effect , value);
    Sds->Set( Sds->addr->KEY , SOFTFREQUENCYKEY);

};

void MainWindow::dial_decay_value_changed()
{
    int dial = ui->hs_adsr_attack->value();
    Sds->Set(Sds->addr->OSC_adsr.attack , dial);
    Sds->Set(Sds->addr->KEY, ADSR_KEY);

}
void MainWindow::dial_PMW_value_changed()
{
    int dial = ui->dial_PMW->value();
    Sds->Set(Sds->addr->VCO_wp.PMW_dial , dial);
    Sds->Set( Sds->addr->KEY ,PMWDIALKEY);
}

void MainWindow::Rtsp_Dialog()
{
    if ( this->Rtsp_Dialog_p->isVisible()   )
    {
        this->Rtsp_Dialog_p->hide();
    }
    else
    {
        this->Rtsp_Dialog_p->show();
        Rtsp_Dialog_p->proc_table_update_all( );
    }
}
void MainWindow::SDS_Dialog()
{
    string Start_Comstack = Cfg->Server_cmd( Cfg->Config.Term, file_structure().comstack_bin, "" );
	system_execute( Start_Comstack.data() );
	return;

    if ( this->SDS_Dialog_p->isVisible()   )
    {
        this->SDS_Dialog_p->hide();
    }
    else
    {
        this->SDS_Dialog_p->show();
    }
}

void MainWindow::File_Director()
{
    if ( this->File_Dialog_p->isVisible()   )
        this->File_Dialog_p->hide();
    else
    {
    	int notetype = Sds->addr->NotestypeId;
    	File_Dialog_p->CB_notes->clear();
    	File_Dialog_p->CB_notes->addItems( Qread_filenames( file_structure().Notesdirs[ notetype],
															file_structure().Notestypes[notetype]) );
		this->File_Dialog_p->show();
    }
}
void MainWindow::setButtonColor( QPushButton* pb, QColor color  )
{
	QPalette status_color = QPalette();
	status_color.setColor(QPalette::Button, color);
	pb->setPalette( status_color );
};

void MainWindow::Spectrum_Dialog()
{
    if ( this->Spectrum_Dialog_p->isVisible()   )
        this->Spectrum_Dialog_p->hide();
    else
        this->Spectrum_Dialog_p->show();
}

void MainWindow::waveform_slot(	uint8_t* wf_addr,
								uint8_t wfid,
								int ID,
								int wf_key,
								QLabel* label  )
{
	*wf_addr = wfid;
	Sds->Set( Sds->addr->KEY , wf_key);
	label->setText( QWaveform_vec[ wfid ] );
	this->Spectrum_Dialog_p->SetLabelWaveform( QWaveform_vec[ wfid ] );
}
void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->OSC_spectrum.wfid, _wfid, OSCID, SETWAVEFORMMAINKEY, ui->wf_main );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &Sds->addr->FMO_spectrum.wfid, _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->VCO_spectrum.wfid, _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}


void MainWindow::select_Sds( uint sdsid ) // TODO working
{
	this->Sds_master->config = sdsid;
	this->Sds_master->UpdateFlag = true;
	this->Sds->addr->UpdateFlag = true;
	this->SDS_ID = sdsid;

	this->Sds = DaTA->GetSds( sdsid );
    Info( App.This_Application + " set to SDS Id: " + to_string( (int) Sds->addr->SDS_Id ));
	DaTA->Reg.Show_proc_register( sdsid );

	File_Dialog_p->SetSds( this->Sds, sdsid );
	Spectrum_Dialog_p->SetSds( this->Sds, sdsid );

	Rtsp_Dialog_p->proc_table_update_row( sdsid + 1 );
	Rtsp_Dialog_p->SDS_ID = sdsid;

	wp_vec = { &Sds->addr->VCO_wp, &Sds->addr->FMO_wp, &Sds->addr->OSC_wp };


};

void MainWindow::select_Sds0()
{
	select_Sds( 0);
	setwidgetvalues();
}
void MainWindow::select_Sds1()
{
	select_Sds( 1);
	setwidgetvalues();
}
void MainWindow::select_Sds2()
{
	select_Sds( 2);
	setwidgetvalues();
}
void MainWindow::select_Sds3()
{
	select_Sds( 3);
	setwidgetvalues();
}

auto setStaPlay( MainWindow* M, uint id )
{
    M->Sds->Set( M->Sds->addr->MIX_Id , id );
    bool play = not M->Sds->addr->StA_state[id].play;
    M->Sds->Set( M->Sds->addr->StA_state[id].play, play);
    M->Sds->Set( M->Sds->addr->KEY, SETSTAPLAY_KEY );
    M->cb_sta_vec[ id ].cb->setChecked( play ) ;
}

void MainWindow::toggle_mute0(  )
{
	setStaPlay( this, 0 );
}
void MainWindow::toggle_mute1(  )
{
	setStaPlay( this, 1 );
}
void MainWindow::toggle_mute2(  )
{
	setStaPlay( this, 2 );
}
void MainWindow::toggle_mute3(  )
{
	setStaPlay( this, 3 );
}
void MainWindow::toggle_mute4(  )
{
	setStaPlay( this, MbIdInstrument );
}
void MainWindow::toggle_mute5(  )
{
	setStaPlay( this, MbIdKeyboard );
}
void MainWindow::toggle_mute6(  )
{
	setStaPlay( this, MbIdNotes );
}
void MainWindow::toggle_mute7(  )
{
	setStaPlay( this, MbIdExternal );
}

auto toggle_store_sta( MainWindow* M, int ID )
{
    M->Sds->Set( M->Sds->addr->MIX_Id , ID );
    if ( M->Sds->addr->StA_state[ID].store )
    {
    	M->Sds->Set( M->Sds->addr->KEY , STOPRECORD_KEY);
    }
    else
    {
        M->Sds->Set( M->Sds->addr->KEY , STORESOUNDKEY);
    }
    M->rb_sta_vec[ ID ].rb->setChecked( false );
};

void MainWindow::toggle_store_sta0()
{
	toggle_store_sta( this, 0 );
}
void MainWindow::toggle_store_sta1()
{
	toggle_store_sta( this, 1 );
}
void MainWindow::toggle_store_sta2()
{
	toggle_store_sta( this, 2 );
}
void MainWindow::toggle_store_sta3()
{
	toggle_store_sta( this, 3 );
}
void MainWindow::toggle_store_sta4()
{
	toggle_store_sta( this, 4 );
}
void MainWindow::toggle_store_sta5()
{
	toggle_store_sta( this, 5 );
}
void MainWindow::toggle_store_sta6()
{
	toggle_store_sta( this, 6 );
}
void MainWindow::toggle_store_sta7()
{
	toggle_store_sta( this, MbIdExternal );
}


void MainWindow::memory_clear()
{
	uint id = 0;
    for ( rb_state_t map : rb_sta_vec )
    {
    	if ( map.rb->isChecked() )
    	{
    		Sds->Set( Sds->addr->MIX_Id, id);
    		map.rb->setChecked( false );
    	}
    	id++;
    }
    Sds->Set( Sds->addr->KEY, CLEAR_KEY );
}

auto mixer_slider( MainWindow* C, int ID, int value )
{
    C->Sds->Set( C->Sds->addr->MIX_Id , ID );
    C->Sds->Set( C->Sds->addr->StA_amp_arr[ID], value );
    C->Sds->Set( C->Sds->addr->KEY 	, SETMBAMPPLAYKEY);
};

void MainWindow::Sl_mix0( int value )
{
	mixer_slider( this, 0, value );
};
void MainWindow::Sl_mix1( int value )
{
	mixer_slider( this, 1, value );
};
void MainWindow::Sl_mix2( int value )
{
	mixer_slider( this, 2, value );
};
void MainWindow::Sl_mix3( int value )
{
	mixer_slider( this, 3, value );
};
void MainWindow::Sl_mix4( int value )
{
	mixer_slider( this, MbIdInstrument, value );
};
void MainWindow::Sl_mix5( int value )
{
	mixer_slider( this, MbIdKeyboard, value );
};
void MainWindow::Sl_mix6( int value )
{
    mixer_slider( this, MbIdNotes, value );
};
void MainWindow::Sl_mix7( int value )
{
    mixer_slider( this, MbIdExternal, value );
};

void MainWindow::slot_dial_ramp_up_down()
{
    float value = ui->dial_ramp_up_down->value();
    Sds->Set( Sds->addr->LOOP_end, value);
    Sds->Set( Sds->addr->LOOP_step, 1 );
    Sds->Set( Sds->addr->KEY, MASTERAMP_LOOP_KEY);
}

void MainWindow::Clear_Banks()
{
	for( cb_state_t map : cb_sta_vec )
		map.cb->setChecked(false);

    Sds->Set( Sds->addr->KEY , MUTEMBKEY);

}
void MainWindow::toggle_Mute()
{
    bool mute_flag 	= not Sds->addr->mixer_status.mute ;
    Sds->Set( Sds->addr->KEY , MASTERAMP_MUTE_KEY);
    QString Qstr = mute_flag ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
}

void MainWindow::cB_Beat_per_sec( int bps_id )
{
	uint8_t bps_val = ui->cb_bps->currentText().toInt();
    Sds->Set( Sds->addr->OSC_adsr.bps, bps_val  );
	Sds->Set( Sds->addr->KEY, ADSR_KEY );
}

void MainWindow::setwidgetvalues()
{
    ui->Slider_FMO_Hz->setValue( Sds->addr->FMO_wp.frqidx );
    ui->Slider_VCO_Hz->setValue( Sds->addr->VCO_wp.frqidx );;
    ui->Slider_OSC_Hz->setValue( Sds->addr->OSC_wp.frqidx );;

    ui->Slider_FMO_Hz->setValue( Sds->addr->FMO_wp.frqidx );
    ui->Slider_VCO_Hz->setValue( Sds->addr->VCO_wp.frqidx );;
    ui->Slider_OSC_Hz->setValue( Sds->addr->OSC_wp.frqidx );;

    ui->Slider_FMO_vol->setValue(   Sds->addr->FMO_wp.volume);
    ui->Slider_VCO_vol->setValue(   Sds->addr->VCO_wp.volume);
    ui->Slider_OSC_Vol->setValue(  Sds_master->Master_Amp);

    ui->FMOLCD_Hz->display( Sds->addr->FMO_wp.frequency );
    ui->VCOLCD_Hz->display( Sds->addr->VCO_wp.frequency );
    ui->OSCLCD_Hz->display( Sds->addr->OSC_wp.frequency );

    ui->FMOLCD_Amp->display( Sds->addr->FMO_wp.volume );
    ui->VCOLCD_Amp->display( Sds->addr->VCO_wp.volume );
    ui->OSCLCD_Amp->display( Sds_master->Master_Amp);

    ui->wf_fmo->setText( QWaveform_vec[ Sds->addr->FMO_spectrum.wfid ] );
    ui->wf_vco->setText( QWaveform_vec[ Sds->addr->VCO_spectrum.wfid ] );
    ui->wf_main->setText( QWaveform_vec[ Sds->addr->OSC_spectrum.wfid ] );

    ui->sB_Main->setValue( Sds->addr->OSC_spectrum.wfid );
    ui->sB_FMO->setValue(  Sds->addr->FMO_spectrum.wfid  );
    ui->sB_VCO->setValue(  Sds->addr->VCO_spectrum.wfid  );

    ui->hs_adsr_sustain->setValue(  (int)Sds->addr->OSC_adsr.decay );
    ui->hs_adsr_attack->setValue(  (int) Sds->addr->OSC_adsr.attack);
    ui->dial_PMW->setValue( (int)Sds->addr->VCO_wp.PMW_dial  );
    ui->dial_soft_freq->setValue( (int)  Sds->addr->OSC_wp.glide_effect );
    ui->hs_hall_effect->setValue( (int)  Sds->addr->OSC_adsr.hall );

    get_record_status();

    ui->dial_ramp_up_down->setValue( Sds->addr->Master_Amp);

    QString Qstr = Sds->addr->mixer_status.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    ui->cb_bps->setCurrentText( QString( int2char( Sds->addr->OSC_adsr.bps)));

	set_cb_sta_value( this );
	set_sl_sta_value( this );
	set_rb_sta_value( this );

	ui->Pbar_telapsed->setValue( Sds->addr->time_elapsed );

	if ( Sds->addr->WD_status.roleId == osc_struct::AUDIOID )
		OscW_item->sds = Sds_master;
	else
		OscW_item->sds = this->Sds->addr;
	if( Spectrum_Dialog_p->isVisible( ))
		Spectrum_Dialog_p->Update_spectrum();

    Sds->Set( Sds->addr->UserInterface 	, RUNNING );
    MainWindow::show();
}

void MainWindow::sliderFreq( uint8_t oscid,  QLCDNumber* lcd, int value, char key )
{
	float freq 		= Spectrum.Calc( value );
	lcd->display( freq );

	wp_vec[ oscid ]->frequency 	= freq;
	wp_vec[ oscid ]->frqidx 	= value;

	this->Sds->Set( this->Sds->addr->KEY, (char) key);
};
void MainWindow::Slider_OSC_Freq( int value )
{
	sliderFreq( OSCID, ui->OSCLCD_Hz, value, OSCFREQUENCYKEY );
}
void MainWindow::Slider_VCO_Freq( int value )
{
	( value < Spectrum.C0 ) ? ui->lb_VCO_LFO->show() : ui->lb_VCO_LFO->hide();

    sliderFreq( VCOID, ui->VCOLCD_Hz, value, VCOFREQUENCYKEY );
}
void MainWindow::Slider_FMO_Freq( int value )
{
	( value < Spectrum.C0 ) ? ui->lb_FMO_LFO->show() : ui->lb_FMO_LFO->hide();

    sliderFreq( FMOID, ui->FMOLCD_Hz, value, FMOFREQUENCYKEY );
}


void MainWindow::sliderVolume( uint8_t oscid, QLCDNumber* lcd, int value, char key )
{
	if ( oscid == OSCID )
	{
		Sds_master->Master_Amp 	= value;
	}
	else
	{
		wp_vec[ oscid ]->volume = value;
		lcd->display( value );
	}

	this->Sds->Set( this->Sds->addr->KEY, (char) key);

};

void MainWindow::MAIN_slot_volume()
{
	sliderVolume( OSCID, ui->OSCLCD_Amp, ui->Slider_OSC_Vol->value(), MASTERAMP_KEY );
}
void MainWindow::VCO_slot_volume()
{
	sliderVolume( VCOID, ui->VCOLCD_Amp, ui->Slider_VCO_vol->value(), VCOAMPKEY );
}
void MainWindow::FMO_slot_volume()
{
	sliderVolume( FMOID, ui->FMOLCD_Amp, ui->Slider_FMO_vol->value(), FMOAMPKEY );
}

void MainWindow::Slider_VCO_Adjust( int value )
{
	Sds->Set( Sds->addr->VCO_wp.adjust, value);
	Sds->Set( Sds->addr->KEY, ADJUST_KEY);
}
void MainWindow::Slider_FMO_Adjust( int value )
{
	Sds->Set( Sds->addr->FMO_wp.adjust, value);
	Sds->Set( Sds->addr->KEY, ADJUST_KEY);
}

void MainWindow::start_composer()
{
    string Start_Composer = Cfg->Server_cmd( Cfg->Config.Term, file_structure().composer_bin, "" );
	system_execute( Start_Composer.data() );
}

void MainWindow::start_audio_srv()
{
	if( Sds->addr->Rtsp == RUNNING )
		return;
    string Start_Audio_Srv = Cfg->Server_cmd( Cfg->Config.Term,
    		file_structure().audio_bin,
			"-S 0");
	system_execute( Start_Audio_Srv.data() );
    Sem->Lock( SEMAPHORE_STARTED );
    Rtsp_Dialog_obj.proc_table_update_row( 0 );
}

void MainWindow::start_synthesizer()
{
	if( Sds->addr->Rtsp == RUNNING )
	{
	    if ( Sem->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
	    	Sem->Release( SYNTHESIZER_START );
	    return;
	}

	int sdsid = DaTA->Reg.GetStartId( );
	if ( sdsid < 0 ) return;

	string Start_Synthesizer = Cfg->Server_cmd( Cfg->Config.Term,
    		file_structure().synth_bin,
			"-S " + to_string( sdsid ) );

    system_execute( Start_Synthesizer.data() );
    Sem->Lock( SEMAPHORE_STARTED );
    select_Sds(sdsid);
	setwidgetvalues();

//    Rtsp_Dialog_obj.proc_table_update_row( sdsid + 1);
}
void MainWindow::read_polygon_data()
{
    OscW_item->read_polygon_data();
};

void MainWindow::Controller_Exit()
{
	if ( Sds->addr->Rtsp == RUNNING )
	{
		if ( Sem->Getval( SEMAPHORE_EXIT, GETVAL ) > 0 )
			Sem->Release( SEMAPHORE_EXIT );
		return;
	}
	uint idx = Rtsp_Dialog_obj.SDS_ID + SYNTHID ;
    Sds->Set( Sds->addr->Synthesizer , EXITSERVER );
    DaTA->Sem.Lock( SEMAPHORE_EXIT, 2 );
    DaTA->Reg.Reset( idx );
    Rtsp_Dialog_obj.proc_table_update_row(Rtsp_Dialog_obj.SDS_ID + 1);
}

void MainWindow::Audio_Exit()
{
    Sds_master->AudioServer = EXITSERVER;
    DaTA->Reg.Reset(  AUDIOID );
    DaTA->Sem.Lock( SEMAPHORE_EXIT, 2 );
    Rtsp_Dialog_obj.proc_table_update_row(0);
}

void MainWindow::Save_Config()
{
    Sds->Write_str( INSTRUMENTSTR_KEY, "default");
    Sds->Set( Sds->addr->KEY , SAVEINSTRUMENTKEY); //
}

void MainWindow::set_mode_f()
{
    Sds->Set( Sds->addr->KEY , RESETFMOKEY);
}

void MainWindow::set_mode_v()
{
    Sds->Set( Sds->addr->KEY , RESETVCOKEY); //
}
void MainWindow::set_mode_o()
{
    Sds->Set( Sds->addr->KEY , RESETMAINKEY); //
}

void MainWindow::connect_fmo()
{
    Sds->Set( Sds->addr->KEY , CONNECTFMOVCOKEY);
}

void MainWindow::connect_vco()
{
    Sds->Set( Sds->addr->KEY , CONNECTVCOFMOKEY);
}

void MainWindow::get_record_status( )
{
    int pb_value = Sds->addr->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}
void MainWindow::SaveRecord()
{
    Sds->Set( Sds_master->FileNo , 0); // 0=automatic numbering

    if ( Sds_master->Record )
    	Sds_master->AudioServer = RECORDSTOP;
    else
    {
    	Sds_master->AudioServer = RECORDSTART;
    }

//	Sds->Set( Sds->addr->Record, not Sds->addr->Record );
}

void MainWindow::main_adsr_sustain()
{
    int value = ui->hs_adsr_sustain->value();
    Sds->Set( Sds->addr->OSC_adsr.decay , value);
    Sds->Set( Sds->addr->KEY ,ADSR_KEY);
}


void MainWindow::pB_Debug_clicked()
{
    uint8_t counter = ( Sds->addr->WD_status.wd_mode + 1 ) % WD_MODE_SIZE;
    Sds->Set( Sds->addr->WD_status.wd_mode , counter);
    Sds->Set( Sds->addr->KEY , SETWAVEDISPLAYKEY );

    ui->pB_wd_mode->setText( Qwd_wdmode_names[ (int)counter ] );
}

void MainWindow::pB_oscgroup_clicked()
{
    uint8_t counter = ( Sds->addr->WD_status.oscId + 1 ) % WD_OSC_SIZE;
    Sds->Set( Sds->addr->WD_status.oscId, counter );
    Sds->Set( Sds->addr->KEY , SETWAVEDISPLAYKEY);

    ui->pB_oscgroup->setText( Qwd_osc_names[ (int)counter ] );
}

void MainWindow::pB_Wavedisplay_clicked()
{
    uint8_t counter = (Sds->addr->WD_status.roleId + 1) % WD_ROLES_SIZE;
    Sds->Set( Sds->addr->WD_status.roleId , counter);
    Sds->Set( Sds->addr->KEY , SETWAVEDISPLAYKEY);

    ui->pB_Wavedisplay->setText( Qwd_display_names[ (int)counter ] );
};

void MainWindow::pB_fftmode_clicked()
{
	bool fft_mode = not (Sds->addr->WD_status.fftmode );
	Sds->Set( Sds->addr->WD_status.fftmode, fft_mode );
	Sds->Set( Sds->addr->KEY, SETWAVEDISPLAYKEY );
	cout << "pB_fftmode_clicked" << boolalpha << Sds->addr->WD_status.fftmode << endl;

	ui->pb_fftmode->setText( Qwd_fftmodes[ (int)fft_mode ] );

}
void MainWindow::updateWidgets()
{

    if (  Sds_master->UserInterface == UPDATEGUI  )
    {
        if ( Sds->addr->KEY == INSTRUMENTSTR_KEY )
        {
            string str = Sds->Read_str( INSTRUMENTSTR_KEY );
            Instrument_name = QString::fromStdString( str );
            File_Dialog_p->CB_instruments->textActivated( Instrument_name );
        }
        if ( not Sds_master->Composer )
        {
			switch( Sds->addr->FLAG )
			{
				case RECORDWAVFILEFLAG :
				{
					Info( "update recording info");
					CB_external->clear();
					CB_external->addItems( Qread_filenames( file_structure().Dir.musicdir, file_structure().wav_type) );
					string file		= Sds->Read_str( OTHERSTR_KEY );
					QString Qfile	= QString::fromStdString( file );
					CB_external->setCurrentText( Qfile );
					break;
				}
				case NEWINSTRUMENTFLAG :
				{
					File_Dialog_p->CB_instruments->clear();
					File_Dialog_p->CB_instruments->addItems( Qread_filenames( file_structure().Dir.instrumentdir, file_structure().snd_type));
					break;
				}
				case NEWNOTESLINEFLAG :
				{
					int notetype = Sds->addr->NotestypeId;
					File_Dialog_p->CB_notes->clear();
					File_Dialog_p->CB_notes->addItems( Qread_filenames(file_structure().Notesdirs[ notetype],
									file_structure().Notestypes[notetype]) );
					break;
				}
			}
        }
    }

    Sds->addr->UserInterface = OFFLINE ;


    if (Sds_master->AudioServer == RUNNING )
        setButtonColor( ui->pBAudioServer, Qt::green);
    else
        setButtonColor( ui->pBAudioServer, Qt::red);


    if (Sds->addr->Synthesizer == RUNNING )
        setButtonColor( ui->pBSynthesizer, Qt::green);
    else
        setButtonColor( ui->pBSynthesizer, Qt::red);

    if( Sds_master->Record)
    {
    	ui->pBtoggleRecord->setText( "Stop Rec");
        setButtonColor( ui->pBtoggleRecord, Qt::red);
    }
    else
    {
    	ui->pBtoggleRecord->setText( "Record");
        setButtonColor( ui->pBtoggleRecord, Qt::green);
    }
    setwidgetvalues();

}



