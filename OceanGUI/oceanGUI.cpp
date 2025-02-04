
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



// System
#include <unistd.h> //sleep

const string Module = "OceanGUI";

auto set_rb_sta_value = []( MainWindow* C )
{
    int ID = 0;
    for( QRadioButton* rb : C->rb_sta_vec )
    {
    	rb->setChecked( C->Sds->addr->StA_state[ID].store );
		ID++;
    }

};
auto set_cb_sta_value = []( MainWindow* C )
{
    uint ID = 0;
    for( QCheckBox* cb : C->cb_sta_vec )
    {
    	cb->setChecked( C->Sds->addr->StA_state[ID].play );
		ID++;
    }
};
auto set_sl_sta_value = []( MainWindow* C )
{
    uint ID = 0;
    for( QSlider* sl : C->sl_sta_vec )
    {
    	sl->setValue( C->Sds->addr->StA_amp_arr[ID] );
		ID++;
    }
};


Ui::MainWindow	Ui_Mainwindow_obj{};

MainWindow::MainWindow(	QWidget *parent ) :
	Logfacility_class( Module )
//	ui(new Ui::MainWindow)

{



	QWaveform_vec = Vstringvector( Spectrum.Get_waveform_vec() );


	Qwd_wdmode_names 	= Vstringvector( wavedisplay_struct().types );
	Qwd_osc_names 		= Vstringvector( osc_struct().types );
	Qwd_display_names 	= Vstringvector( osc_struct().roles );
	Qwd_fftmodes		= Vstringvector( wavedisplay_struct().fftmodes );

	Qbps_str_lst = Qstringlist( bps_struct().Bps_lst );

	ui = &Ui_Mainwindow_obj;
    ui->setupUi(this);

    // https://stackoverflow.com/questions/17095957/qt-creator-and-main-window-background-image
    QString Ocean_png = QString::fromStdString( file_structure().Dir.libdir + "Ocean.png" );
    QPixmap bkgnd( Ocean_png );
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);

    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Button, QColor(0,179,255) );
    this->setPalette(palette);

    connect( ui->pB_Rtsp, SIGNAL(clicked()), this, SLOT( Rtsp_Dialog() ));
    connect( ui->pb_SDSview, SIGNAL(clicked()), this, SLOT( SDS_Dialog() ));
    connect( ui->pBSynthesizerExit, SIGNAL(clicked()), this, SLOT( Controller_Exit() ));
    connect( ui->pBAudioServer, 	SIGNAL(clicked()), this, SLOT( start_audio_srv() ));

    connect( ui->pBComposer, SIGNAL(clicked()), this, SLOT( start_composer() ));
    connect( ui->pBtoggleRecord, SIGNAL(clicked(bool)), this, SLOT(SaveRecord() ));
    connect( ui->pB_Mute, SIGNAL(clicked()), this, SLOT(toggle_Mute()));
    connect( ui->pb_clear, SIGNAL(clicked()), this, SLOT(memory_clear()));
    connect( ui->Slider_VCO_Hz, SIGNAL(valueChanged(int)), this, SLOT(Slider_VCO_Hz_changed(int)));
    connect( ui->Slider_FMO_Hz, SIGNAL(valueChanged(int)), this, SLOT(Slider_FMO_Hz_changed(int)));

    connect(ui->pB_play_notes, SIGNAL(clicked()), this, SLOT(File_Director() ));
    connect(ui->pB_Specrum, SIGNAL(clicked()), this, SLOT(Spectrum_Dialog() ));

    connect(ui->dial_soft_freq, SIGNAL(valueChanged(int)), this, SLOT(dial_soft_freq_value_changed() ));
    connect(ui->dial_PMW      , SIGNAL(valueChanged(int)), this, SLOT(dial_PMW_value_changed() ));
    connect(ui->dial_ramp_up_down, SIGNAL( valueChanged(int)), this, SLOT(slot_dial_ramp_up_down()) );
    connect(ui->hs_adsr_attack , SIGNAL(valueChanged(int)), this, SLOT(dial_decay_value_changed() ));

    connect(ui->cb_bps		, SIGNAL(activated(int)), this, SLOT(cB_Beat_per_sec(int) ));
    connect(ui->hs_adsr_sustain, SIGNAL(valueChanged(int)), this, SLOT(main_adsr_sustain() ));
    connect(ui->pB_Wavedisplay , SIGNAL(clicked()), this, SLOT(pB_Wavedisplay_clicked()));
    connect(ui->pB_wd_mode, SIGNAL(clicked()), this, SLOT(pB_Debug_clicked()));
    connect(ui->pB_oscgroup, SIGNAL(clicked()), this, SLOT(pB_oscgroup_clicked()));
    connect(ui->pb_fftmode, SIGNAL(clicked()), this, SLOT(pB_fftmode_clicked()));

    sl_sta_vec = {
    	ui->Slider_mix_vol0, ui->Slider_mix_vol1, ui->Slider_mix_vol2, ui->Slider_mix_vol3,
		ui->Slider_mix_vol4, ui->Slider_mix_vol5, ui->Slider_mix_vol6, ui->Slider_mix_vol7
    };
	set_sl_sta_value( this );
    connect(ui->Slider_mix_vol0, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix0(int) ));
    connect(ui->Slider_mix_vol1, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix1(int) ));
    connect(ui->Slider_mix_vol2, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix2(int) ));
    connect(ui->Slider_mix_vol3, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix3(int) ));
    connect(ui->Slider_mix_vol4, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix4(int) ));
    connect(ui->Slider_mix_vol5, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix5(int) ));
    connect(ui->Slider_mix_vol6, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix6(int) ));
    connect(ui->Slider_mix_vol7, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix7(int) ));

    rb_sta_vec =
    {
    	ui->rb_sta0, ui->rb_sta1, ui->rb_sta2, ui->rb_sta3,
		ui->rb_sta4, ui->rb_sta5, ui->rb_sta6, ui->rb_sta7
    };
    set_rb_sta_value( this );
    connect(ui->rb_sta0, SIGNAL(clicked()), this, SLOT( toggle_store_sta0()) );
    connect(ui->rb_sta1, SIGNAL(clicked()), this, SLOT( toggle_store_sta1()) );
    connect(ui->rb_sta2, SIGNAL(clicked()), this, SLOT( toggle_store_sta2()) );
    connect(ui->rb_sta3, SIGNAL(clicked()), this, SLOT( toggle_store_sta3()) );
    connect(ui->rb_sta4, SIGNAL(clicked()), this, SLOT( toggle_store_sta4()) );
    connect(ui->rb_sta5, SIGNAL(clicked()), this, SLOT( toggle_store_sta5()) );
    connect(ui->rb_sta6, SIGNAL(clicked()), this, SLOT( toggle_store_sta6()) );
    connect(ui->rb_sta7, SIGNAL(clicked()), this, SLOT( toggle_store_sta7()) );

    cb_sta_vec =
    {
    		ui->cb_sta0, ui->cb_sta1, ui->cb_sta2, ui->cb_sta3,
			ui->cb_sta4, ui->cb_sta5, ui->cb_sta6, ui->cb_sta7
    };
	set_cb_sta_value( this );
    connect(ui->cb_sta0, SIGNAL(clicked()), this, SLOT( toggle_mute0()) );
    connect(ui->cb_sta1, SIGNAL(clicked()), this, SLOT( toggle_mute1()) );
    connect(ui->cb_sta2, SIGNAL(clicked()), this, SLOT( toggle_mute2()) );
    connect(ui->cb_sta3, SIGNAL(clicked()), this, SLOT( toggle_mute3()) );
    connect(ui->cb_sta4, SIGNAL(clicked()), this, SLOT( toggle_mute4()) );
    connect(ui->cb_sta5, SIGNAL(clicked()), this, SLOT( toggle_mute5()) );
    connect(ui->cb_sta6, SIGNAL(clicked()), this, SLOT( toggle_mute6()) );
    connect(ui->cb_sta7, SIGNAL(clicked()), this, SLOT( toggle_mute7()) );

    rb_S_vec =
    {
    	ui->rb_S0, ui->rb_S0, ui->rb_S0, ui->rb_S0
    };
    rb_S_vec[ this->Sds->addr->config ]->setChecked( true );
    connect(ui->rb_S0, SIGNAL(clicked()), this, SLOT( select_Sds0()) );
    connect(ui->rb_S1, SIGNAL(clicked()), this, SLOT( select_Sds1()) );
    connect(ui->rb_S2, SIGNAL(clicked()), this, SLOT( select_Sds2()) );
    connect(ui->rb_S3, SIGNAL(clicked()), this, SLOT( select_Sds3()) );

    connect(ui->cb_external, SIGNAL(activated(QString)), this, SLOT(wavfile_selected(QString)));

    connect(ui->hs_hall_effect, SIGNAL(valueChanged(int)), this, SLOT(hs_hall_effect_value_changed(int)));

    connect( ui->sB_FMO , SIGNAL( valueChanged(int)), this, SLOT(FMO_Waveform_slot( int ))) ;
    connect( ui->sB_VCO , SIGNAL( valueChanged(int)), this, SLOT(VCO_Waveform_slot( int ))) ;
    connect( ui->sB_Main, SIGNAL( valueChanged(int)), this, SLOT(Main_Waveform_slot( int ))) ;

    connect(status_timer, &QTimer::timeout, this, &MainWindow::Updatewidgets);
    status_timer->start(1000);

    connect(osc_timer, &QTimer::timeout, this, &MainWindow::read_polygon_data );
    osc_timer->start(20); // 50 Hz

    uint sb_max = QWaveform_vec.size()-1;
    ui->sB_Main->setMaximum(sb_max);
    ui->sB_FMO->setMaximum(sb_max);
    ui->sB_VCO->setMaximum(sb_max);

    CB_external         = ui->cb_external;
    string wavfile_path = file_structure().Dir.musicdir;
    Qread_filelist( CB_external,
    				wavfile_path, file_structure().wav_type );
    string file		= Sds->Read_str( OTHERSTR_KEY );
    QString Qfile	= QString::fromStdString( file );
    CB_external->setCurrentText( Qfile );

    ui->cb_bps->addItems( Qbps_str_lst );

    ui->pB_oscgroup->setText( Qwd_osc_names[ Sds->addr->WD_status.oscId ]);
    ui->pb_fftmode->setText( Qwd_fftmodes [ Sds->addr->WD_status.fftmode ]);
    ui->pB_wd_mode->setText( Qwd_wdmode_names[ Sds->addr->WD_status.wd_mode ]);
    ui->pB_Wavedisplay->setText( Qwd_display_names[ Sds->addr->WD_status.roleId ]);

    setwidgetvalues();


    ui->oscilloscope_view->setScene( scene );
    QRectF rect         = ui->oscilloscope_view->geometry();
    OszilloscopeWidget	OscWidg( Sds->addr, rect );
    OscW_item = new OszilloscopeWidget( Sds->addr, rect );
    scene->addItem( OscW_item );

    Sds_master->Record = false;
}

MainWindow::~MainWindow()
{
	delete OscW_item;
//	if ( ui ) delete ui;
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
		Qread_filelist( this->File_Dialog_p->CB_notes,
						file_structure().Notesdirs[ notetype],
						file_structure().Notestypes[notetype]);
		this->File_Dialog_p->show();
    }
}

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
	waveform_slot( &Sds->addr->OSC_spectrum.id, _wfid, osc_struct::OSCID, SETWAVEFORMMAINKEY, ui->wf_main );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &Sds->addr->FMO_spectrum.id, _wfid, osc_struct::FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &Sds->addr->VCO_spectrum.id, _wfid, osc_struct::VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}


void MainWindow::select_Sds( uint sdsid ) // TODO working
{

	this->Sds = DaTA->GetSds( sdsid );
	DaTA->Reg.Show_proc_register( sdsid );
//    Comment( INFO," Ocean GUI set to SDS Id: " + to_string( (int)sdsid));

    File_Dialog_obj.SetSds( this->Sds, sdsid );
	Spectrum_Dialog_Obj.ifd = this->Sds->addr;
	if ( Sds->addr->WD_status.roleId == osc_struct::AUDIOID )
		OscW_item->sds = Sds_master;
	else
		OscW_item->sds = this->Sds->addr;

	Rtsp_Dialog_obj.proc_table_update_row( sdsid + 1 );
	Rtsp_Dialog_obj.SDS_ID = sdsid;

	DaTA->sds_master->config = sdsid;
	DaTA->sds_master->UpdateFlag = true;

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

auto setStaPlay( MainWindow* C, uint id )
{
	cout.flush() << "setStaPlay " << id << endl;
    C->Sds->Set( C->Sds->addr->MIX_Id , id );
    bool play = not C->Sds->addr->StA_state[id].play;
    C->Sds->Set( C->Sds->addr->StA_state[id].play, play);
    C->Sds->Set( C->Sds->addr->KEY, SETSTAPLAY_KEY );
    C->cb_sta_vec[ id ]->setChecked( play ) ;
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

auto toggle_store_sta( MainWindow* C, int ID )
{
    C->Sds->Set( C->Sds->addr->MIX_Id , ID );
    if ( C->Sds->addr->StA_state[ID].store )
    {
    	C->Sds->Set( C->Sds->addr->KEY , STOPRECORD_KEY);
    }
    else
    {
        C->Sds->Set( C->Sds->addr->KEY , STORESOUNDKEY);
    }
    C->rb_sta_vec[ ID ]->setChecked( false );
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
    for ( QRadioButton* rb : rb_sta_vec )
    {
    	if ( rb->isChecked() )
    	{
    		Sds->Set( Sds->addr->MIX_Id, id);
    		rb->setChecked( false );
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
	for( QCheckBox* cb : cb_sta_vec )
		cb->setChecked(false);

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
	auto set_slider = []( float f )
	{
		return ( f < LFO_limit ) ? f * LFO_count : f + (float) LFO_count;
	};

    ui->Slider_FMO_Hz->setValue(    set_slider( Sds->addr->FMO_wp.frequency) );
    ui->Slider_VCO_Hz->setValue(    set_slider( Sds->addr->VCO_wp.frequency) );
    ui->Slider_Main_Hz->setValue(   Sds->addr->OSC_wp.frequency);

    ui->Slider_Main_Vol->setValue(  Sds->addr->Master_Amp);
    ui->Slider_FMO_vol->setValue(   Sds->addr->FMO_wp.volume);
    ui->Slider_VCO_vol->setValue(   Sds->addr->VCO_wp.volume);


    ui->labelVCO->setText("VCO");
    ui->labelFMO->setText("FMO");


    ui->wf_fmo->setText( QWaveform_vec[ Sds->addr->FMO_spectrum.id ] );
    ui->wf_vco->setText( QWaveform_vec[ Sds->addr->VCO_spectrum.id ] );
    ui->wf_main->setText( QWaveform_vec[ Sds->addr->OSC_spectrum.id ] );

    ui->sB_Main->setValue( Sds->addr->OSC_spectrum.id );
    ui->sB_FMO->setValue(  Sds->addr->FMO_spectrum.id  );
    ui->sB_VCO->setValue(  Sds->addr->VCO_spectrum.id  );

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


    Sds->Set( Sds->addr->UserInterface 	, RUNNING );
    Sds->Set( Sds->addr->UpdateFlag 		, true );
    MainWindow::show();
}

void MainWindow::GUI_Exit()
{
    qDebug("%s", "Exit" );
    QApplication::exit();
}

auto Slider_Hz = []( Interface_class* IFC, float& fptr, float value, char key )
	{
		IFC->Set( fptr 			, value);
		IFC->Set( IFC->addr->KEY 	, key);
	};

void MainWindow::MAIN_slot_Hz()
{
	int value = ui->Slider_Main_Hz->value();
	Slider_Hz( this->Sds, this->Sds->addr->OSC_wp.frequency, value, OSCFREQUENCYKEY );
}

void MainWindow::Slider_VCO_Hz_changed(int value )
{
    float freq = ( value >= LFO_count ) 	?  value - LFO_count
    								:  (float) value / LFO_count;

    if ( value < LFO_count )
    	ui->lb_VCO_LFO->show();
    else
    	ui->lb_VCO_LFO->hide();

    ui->VCOLCD_Hz->display( freq );

    Slider_Hz( this->Sds, this->Sds->addr->VCO_wp.frequency, freq, VCOFREQUENCYKEY );
}


void MainWindow::Slider_FMO_Hz_changed(int value )
{
    float freq = ( value >= LFO_count ) 	?  value - LFO_count
    								:  (float) value / LFO_count;
    if ( value < LFO_count )
    	ui->lb_FMO_LFO->show();
    else
    	ui->lb_FMO_LFO->hide();

    ui->FMOLCD_Hz->display( freq );

    Slider_Hz( this->Sds, this->Sds->addr->FMO_wp.frequency, freq, FMOFREQUENCYKEY );
}

auto Slider_volume = []( Interface_class* IF, uint8_t& ch_ptr, char value, char key )
	{
		IF->Set( ch_ptr , value);
		IF->Set( IF->addr->KEY , key);
	};

void MainWindow::MAIN_slot_volume()
{
	Slider_volume( this->Sds, Sds->addr->Master_Amp, ui->Slider_Main_Vol->value(), MASTERAMP_KEY );
}
void MainWindow::VCO_slot_volume()
{
	Slider_volume( this->Sds, Sds->addr->VCO_wp.volume, ui->Slider_VCO_vol->value(), VCOAMPKEY );
}
void MainWindow::FMO_slot_volume()
{
	Slider_volume( this->Sds, Sds->addr->FMO_wp.volume, ui->Slider_FMO_vol->value(), FMOAMPKEY );
}


void MainWindow::start_composer()
{
    string Start_Composer = Cfg->Server_cmd( Cfg->Config.Term, file_structure().composer_bin, "" );
	system_execute( Start_Composer.data() );
}

void MainWindow::start_audio_srv()
{
	if( Sds->addr->Rtsp == RUNNING ) return;
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
//    Rtsp_Dialog_obj.proc_table_update_row( sdsid + 1);
    setwidgetvalues();
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
    DaTA->Reg.Reset( idx );
    DaTA->Sem.Lock( SEMAPHORE_EXIT, 2 );
    Rtsp_Dialog_obj.proc_table_update_row(Rtsp_Dialog_obj.SDS_ID + 1);
}

void MainWindow::Audio_Exit()
{
    DaTA->sds_master->AudioServer = EXITSERVER;
    DaTA->Reg.Reset(  AUDIOID );
    DaTA->Sem.Lock( SEMAPHORE_EXIT, 2 );
    Rtsp_Dialog_obj.proc_table_update_row(0);
}

// button save Cfg->Config to default instrument
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

int pb_value = 0;

void MainWindow::get_record_status( )
{
    pb_value = Sds->addr->RecCounter;
//    cout << "Rec status value: " << pb_value << endl;
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

	Sds->Set( Sds->addr->Record, not Sds->addr->Record );
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
void MainWindow::Updatewidgets()
{

    if (  Sds_master->UserInterface == UPDATEGUI  )
    {
        if ( Sds->addr->KEY == INSTRUMENTSTR_KEY )
        {
            string str = Sds->Read_str( INSTRUMENTSTR_KEY );
            Instrument_name = QString::fromStdString( str );
            if ( this->File_Dialog_p != nullptr )
                this->File_Dialog_p->CB_instruments->textActivated( Instrument_name );
            if ( this->Spectrum_Dialog_p != nullptr )
            	Spectrum_Dialog_p->SetLabelInstrument( Instrument_name );
        }
        if ( not DaTA->sds_master->Composer )
        {
			switch( Sds->addr->FLAG )
			{
				case RECORDWAVFILEFLAG :
				{
					Info( "update recording info");
					Qread_filelist( CB_external,
									file_structure().Dir.musicdir, file_structure().wav_type);
					string file		= Sds->Read_str( OTHERSTR_KEY );
					QString Qfile	= QString::fromStdString( file );
					CB_external->setCurrentText( Qfile );
					break;
				}
				case NEWINSTRUMENTFLAG :
				{
					Qread_filelist( this->File_Dialog_p->CB_instruments,
									file_structure().Dir.instrumentdir, file_structure().snd_type);
					break;
				}
				case NEWNOTESLINEFLAG :
				{
					int notetype = Sds->addr->NotestypeId;
					Qread_filelist( this->File_Dialog_p->CB_notes,
									file_structure().Notesdirs[ notetype],
									file_structure().Notestypes[notetype]);
					break;
				}
			}
        }
    }

    Sds->addr->UserInterface = OFFLINE ;

	// init new SDS
    // SetSds(  );


    if (Sds_master->AudioServer == RUNNING )
        Set_button_color( ui->pBAudioServer, Qt::green);
    else
        Set_button_color( ui->pBAudioServer, Qt::red);


    if (Sds->addr->Synthesizer == RUNNING )
        Set_button_color( ui->pBSynthesizer, Qt::green);
    else
        Set_button_color( ui->pBSynthesizer, Qt::red);

    if( Sds_master->Record)
    {
    	ui->pBtoggleRecord->setText( "Stop Rec");
        Set_button_color( ui->pBtoggleRecord, Qt::red);
    }
    else
    {
    	ui->pBtoggleRecord->setText( "Record");
        Set_button_color( ui->pBtoggleRecord, Qt::green);
    }
    setwidgetvalues();

}



