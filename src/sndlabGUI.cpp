
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

int set_slider( float f )
{
	return ( f < LFO_limit ) ? f * LFO_count : f + (float) LFO_count;
}

MainWindow::MainWindow(QWidget *parent) :
	Logfacility_class( Module ),
	Config_class( Module ),
	ui(new Ui::MainWindow)

{

    auto Qstringlist = [ ]( const vector<string> str_vec )
		{
			QStringList Qlist{};
			for ( string str : str_vec )
			{
                Qlist.push_back( ( QString::fromStdString( str ) ) );
			}
			return Qlist;

		};

    auto fromstringvector = [ ]( const vector<string> str_vec )
		{
			vector<QString> Qvec{};
			for ( string str : str_vec )
			{
                Qvec.push_back( ( QString::fromStdString( str ) ) );
			}
			return Qvec;

		};
	QWaveform_vec = fromstringvector( Spectrum.Get_waveform_vec() );

	Qwavedisplay_type_str_vec = fromstringvector( wavedisplay_type_str_vec );

	Qbps_str_list = Qstringlist( bps_struct().Bps_str_vec );

	SDS.Announce( App.client_id, &sds->UserInterface );


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

    connect( ui->pBSynthesizerExit, SIGNAL(clicked()), this, SLOT( Controller_Exit() ));
    connect( ui->pBAudioServer, 	SIGNAL(clicked()), this, SLOT( start_audio_srv() ));

    connect( ui->pBComposer, SIGNAL(clicked()), this, SLOT( start_composer() ));
    connect( ui->pBtoggleRecord, SIGNAL(clicked(bool)), this, SLOT(toggle_Record() ));
    connect( ui->pB_Mute, SIGNAL(clicked()), this, SLOT(toggle_Mute()));
    connect( ui->pb_clear, SIGNAL(clicked()), this, SLOT(memory_clear()));
    connect( ui->Slider_VCO_Hz, SIGNAL(valueChanged(int)), this, SLOT(Slider_VCO_Hz_changed(int)));
    connect( ui->Slider_FMO_Hz, SIGNAL(valueChanged(int)), this, SLOT(Slider_FMO_Hz_changed(int)));

    connect(ui->rb_melody_1, SIGNAL(clicked()), this, SLOT(melody_connect() ));
    connect(ui->rb_melody_2, SIGNAL(clicked()), this, SLOT(melody_connect() ));
    connect(ui->rb_melody_3, SIGNAL(clicked()), this, SLOT(melody_connect() ));
    connect(ui->rb_melody_4, SIGNAL(clicked()), this, SLOT(melody_connect() ));

    connect(ui->pB_play_notes, SIGNAL(clicked()), this, SLOT(File_Director() ));
    connect(ui->pB_Specrum, SIGNAL(clicked()), this, SLOT(Spectrum_Dialog() ));

    connect(ui->dial_soft_freq, SIGNAL(valueChanged(int)), this, SLOT(dial_soft_freq_value_changed() ));
    connect(ui->dial_PMW      , SIGNAL(valueChanged(int)), this, SLOT(dial_PMW_value_changed() ));
    connect(ui->dial_ramp_up_down, SIGNAL( valueChanged(int)), this, SLOT(slot_dial_ramp_up_down()) );
    connect(ui->hs_adsr_attack , SIGNAL(valueChanged(int)), this, SLOT(dial_decay_value_changed() ));

    connect(ui->cb_bps		, SIGNAL(activated(int)), this, SLOT(cB_Beat_per_sec(int) ));
    connect(ui->hs_adsr_sustain, SIGNAL(valueChanged(int)), this, SLOT(main_adsr_sustain() ));
    connect(ui->pB_Wavedisplay , SIGNAL(clicked()), this, SLOT(pB_Wavedisplay_clicked()));

    sl_sta_vec = {
    	ui->Slider_mix_vol0, ui->Slider_mix_vol1, ui->Slider_mix_vol2, ui->Slider_mix_vol3,
		ui->Slider_mix_vol4, ui->Slider_mix_vol5, ui->Slider_mix_vol6, ui->Slider_mix_vol7
    };

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
			ui->cb_sta4, ui->cb_sta5, ui->cb_sta6, ui->cb_sta7,
    };
    connect(ui->cb_sta0, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute0(int)) );
    connect(ui->cb_sta1, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute1(int)) );
    connect(ui->cb_sta2, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute2(int)) );
    connect(ui->cb_sta3, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute3(int)) );
    connect(ui->cb_sta4, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute4(int)) );
    connect(ui->cb_sta5, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute5(int)) );
    connect(ui->cb_sta6, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute6(int)) );
    connect(ui->cb_sta7, SIGNAL(stateChanged(int)), this, SLOT( toggle_mute7(int)) );

    connect(ui->pB_Debug, SIGNAL(clicked()), this, SLOT(pB_Debug_clicked()));
    connect(ui->cb_external, SIGNAL(activated(QString)), this, SLOT(wavfile_selected(QString)));

    connect(ui->hs_hall_effect, SIGNAL(valueChanged(int)), this, SLOT(hs_hall_effect_value_changed(int)));

    connect( ui->sB_FMO , SIGNAL( valueChanged(int)), this, SLOT(FMO_Waveform_slot( int ))) ;
    connect( ui->sB_VCO , SIGNAL( valueChanged(int)), this, SLOT(VCO_Waveform_slot( int ))) ;
    connect( ui->sB_Main, SIGNAL( valueChanged(int)), this, SLOT(Main_Waveform_slot( int ))) ;


    record_timer = new QTimer( this );
    connect(record_timer, &QTimer::timeout, this, &MainWindow::get_record_status);
    record_timer->start(1000);

    status_timer = new QTimer( this );
    connect(status_timer, &QTimer::timeout, this, &MainWindow::Updatewidgets);
    status_timer->start(1000);

    osc_timer = new QTimer( this );
    connect(osc_timer, &QTimer::timeout, this, &MainWindow::read_polygon_data );
    osc_timer->start(20); // 50 Hz

    uint sb_max = QWaveform_vec.size()-1;
    ui->sB_Main->setMaximum(sb_max);
    ui->sB_FMO->setMaximum(sb_max);
    ui->sB_VCO->setMaximum(sb_max);

    CB_external         = ui->cb_external;
    string wavfile_path = file_structure().Dir.musicdir;
    Qread_filelist( CB_external,
    				wavfile_path, file_structure().wav_file_type );

    ui->cb_bps->addItems( Qbps_str_list );
    setwidgetvalues();

    scene               = new QGraphicsScene(this);
    ui->oscilloscope_view->setScene( scene );
    QRectF rect         = ui->oscilloscope_view->geometry();
    item                = new OszilloscopeWidget( sds, rect );
    scene->addItem( item );

    this->Spectrum_Dialog_Obj = new Spectrum_Dialog_class( this, &SDS );
    this->File_Dialog_obj = new File_Dialog_class( this, ui->Slider_Main_Hz );


}

MainWindow::~MainWindow()
{
	delete item;
	delete scene;
	delete osc_timer;
	delete status_timer;
	delete record_timer;
	delete ui;
	delete Spectrum_Dialog_Obj;
	delete File_Dialog_obj;
}


bool record=false;
QPalette record_color = QPalette();
QPalette status_color = QPalette();


QString get_bps_qstring( int id )
{
	string bps_str = bps_struct().getbps_str(id);
	QString QStr = QString::fromStdString( bps_str );
    return QStr;
}


void MainWindow::wavfile_selected( const QString &arg)
{
    qDebug() << "WAV file " << arg ;
    QString QStr = arg;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        SDS.Write_str( WAVEFILESTR_KEY, str );
        SDS.Set(sds->KEY , READ_EXTERNALWAVEFILE);
    }
}


void MainWindow::hs_hall_effect_value_changed(int value)
{
    SDS.Set(sds->Main_adsr.hall , value);
    SDS.Set( sds->KEY , ADSR_KEY);
}

void MainWindow::pB_Wavedisplay_clicked()
{
    int wd_counter = (sds->Wavedisplay_Id + 1) % wavedisplay_str_vec.size();
    QString QStr = QString::fromStdString(wavedisplay_str_vec[ wd_counter]);
    ui->pB_Wavedisplay->setText( QStr );
    SDS.Set(sds->Wavedisplay_Id , wd_counter);
    SDS.Set( sds->KEY , SETWAVEDISPLAYKEY);
};

void MainWindow::dial_soft_freq_value_changed()
{
    int value = ui->dial_soft_freq->value();
    SDS.Set(sds->Soft_freq , value);
    SDS.Set( sds->KEY , SOFTFREQUENCYKEY);

};

void MainWindow::dial_decay_value_changed()
{
    int dial = ui->hs_adsr_attack->value();
    SDS.Set(sds->Main_adsr.attack , dial);
    SDS.Set(sds->KEY, ADSR_KEY);

}
void MainWindow::dial_PMW_value_changed()
{
    int dial = ui->dial_PMW->value();
    SDS.Set(sds->PMW_dial , dial);
    SDS.Set( sds->KEY ,PMWDIALKEY);
}

void MainWindow::File_Director()
{
/*    if ( this->File_Dialog_obj == nullptr )
    {
        this->File_Dialog_obj = new File_Dialog_class( this, ui->Slider_Main_Hz );
    }
    */
    if ( this->File_Dialog_obj->isVisible()   )
        this->File_Dialog_obj->hide();
    else
        this->File_Dialog_obj->show();
}

void MainWindow::Spectrum_Dialog()
{
	/*
    if ( this->Spectrum_Dialog_Obj == nullptr )
    {
        this->Spectrum_Dialog_Obj = new Spectrum_Dialog_class( this, &SDS );
    }
    */
    if ( this->Spectrum_Dialog_Obj->isVisible()   )
        this->Spectrum_Dialog_Obj->hide();
    else
        this->Spectrum_Dialog_Obj->show();
}

void MainWindow::waveform_slot(	uint8_t* wf_addr,
								uint8_t wfid,
								int ID,
								int wf_key,
								QLabel* label  )
{
	*wf_addr = wfid;
	SDS.Set( sds->KEY , wf_key);
	label->setText( QWaveform_vec[ wfid ] );
}
void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &sds->MAIN_spectrum.id, _wfid, MAINID, SETWAVEFORMMAINKEY, ui->wf_main );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &sds->FMO_spectrum.id, _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &sds->VCO_spectrum.id, _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}

auto toggle_mute( MainWindow* C, uint id,  int state )
{
	C->sds->MIX_Id = id;
	C->sds->KEY = TOGGLEMBPLAYKEY;
}

void MainWindow::toggle_mute0( int state )
{
	toggle_mute( this, 0, state );
}
void MainWindow::toggle_mute1( int state )
{
	toggle_mute( this, 1,state );
}
void MainWindow::toggle_mute2( int state )
{
	toggle_mute( this, 2,state );
}
void MainWindow::toggle_mute3( int state )
{
	toggle_mute( this, 3,state );
}
void MainWindow::toggle_mute4( int state )
{
	toggle_mute( this, MbIdInstrument,state );
}
void MainWindow::toggle_mute5( int state )
{
	toggle_mute( this, MbIdKeyboard,state );
}
void MainWindow::toggle_mute6( int state )
{
	toggle_mute( this, MbIdNotes, state );
}
void MainWindow::toggle_mute7( int state )
{
	toggle_mute( this, MbIdExternal,state );
}

auto toggle_store_sta( MainWindow* C, int ID )
{
    C->SDS.Set( C->sds->MIX_Id , ID );
    if ( C->sds->StA_state[ID].store )
    {
    	C->SDS.Set( C->sds->KEY , STOPRECORD_KEY);
    }
    else
    {
        C->SDS.Set( C->sds->KEY , STORESOUNDKEY);
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
    		SDS.Set( sds->MIX_Id, id);
    		rb->setChecked( false );
    	}
    	id++;
    }
    SDS.Set( sds->KEY, CLEAR_KEY );
}

auto mixer_slider( MainWindow* C, int ID, int value )
{
    C->SDS.Set( C->sds->MIX_Id , ID );
    C->SDS.Set( C->sds->StA_amp_arr[ID], value );
//    C->SDS.Set( C->sds->StA_status[ID].play, true);
//    C->cb_sta_vec[ID]->setChecked( true );
    C->SDS.Set( C->sds->KEY 	, SETMBAMPPLAYKEY);
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
    SDS.Set( sds->LOOP_end, value);
    SDS.Set( sds->LOOP_step, 1 );
    SDS.Set( sds->KEY, MASTERAMP_LOOP_KEY);
}

void MainWindow::Clear_Banks()
{
	for( QCheckBox* cb : cb_sta_vec )
		cb->setChecked(false);

    SDS.Set( sds->KEY , MUTEMBKEY);

}
void MainWindow::toggle_Mute()
{
    bool mute_flag 	= not sds->mixer_status.mute ;
    SDS.Set( sds->KEY , MASTERAMP_MUTE_KEY);
    QString Qstr = mute_flag ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
}

void MainWindow::cB_Beat_per_sec( int bps_id )
{
    SDS.Set( sds->Main_adsr.bps_id, bps_id  );
    SDS.Set( sds->KEY, ADSR_KEY );
}
void MainWindow::setwidgetvalues()
{

	ui->Pbar_telapsed->setValue( sds->time_elapsed );


    ui->Slider_FMO_Hz->setValue(    set_slider( sds->FMO_Freq) );
    ui->Slider_VCO_Hz->setValue(    set_slider( sds->VCO_Freq) );
    ui->Slider_Main_Hz->setValue(   sds->Main_Freq);

    ui->Slider_Main_Vol->setValue(  sds->Master_Amp);
    ui->Slider_FMO_vol->setValue(   sds->FMO_Amp);
    ui->Slider_VCO_vol->setValue(   sds->VCO_Amp);

    int ID = 0;
    for( QRadioButton* rb : rb_sta_vec )
    {
    	rb->setChecked( sds->StA_state[ID].store );
		ID++;
    }
    ID = 0;
    for( QCheckBox* cb : cb_sta_vec )
    {
    	cb->setChecked( sds->StA_state[ID].play );
		ID++;
    }
    ID = 0;
    for( QSlider* sl : sl_sta_vec )
    {
    	sl->setValue( sds->StA_amp_arr[ID] );
		ID++;
    }

    ui->labelVCO->setText("VCO");
    ui->labelFMO->setText("FMO");


    ui->wf_fmo->setText( QWaveform_vec[ sds->FMO_spectrum.id ] );
    ui->wf_vco->setText( QWaveform_vec[ sds->VCO_spectrum.id ] );
    ui->wf_main->setText( QWaveform_vec[ sds->MAIN_spectrum.id ] );

    ui->sB_Main->setValue( sds->MAIN_spectrum.id );
    ui->sB_FMO->setValue(  sds->FMO_spectrum.id  );
    ui->sB_VCO->setValue(  sds->VCO_spectrum.id  );

    ui->hs_adsr_sustain->setValue(  (int)sds->Main_adsr.decay );
    ui->hs_adsr_attack->setValue(  (int) sds->Main_adsr.attack);
    ui->dial_PMW->setValue( (int)sds->PMW_dial  );
    ui->dial_soft_freq->setValue( (int)  sds->Soft_freq );
    ui->hs_hall_effect->setValue( (int)  sds->Main_adsr.hall );
    ui->progressBar_record->setValue(0);
    int wd_counter              = sds->Wavedisplay_Id;
    QString Qstr = QString::fromStdString(wavedisplay_str_vec[wd_counter]);
    ui->pB_Wavedisplay->setText( Qstr );
    ui->pB_Debug->setText( Qwavedisplay_type_str_vec[ (int) sds->WD_type_ID % 3 ] );

    ui->dial_ramp_up_down->setValue( sds->Master_Amp);

    Qstr = sds->mixer_status.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    sds->UserInterface 	= RUNNING;
    sds->UpdateFlag 		= true;

    MainWindow::show();
}

void MainWindow::GUI_Exit()
{
    qDebug("%s", "Exit" );
    QApplication::quit();
}

auto Slider_Hz = []( Interface_class& IFC, float& fptr, float value, char key )
	{
		IFC.Set( fptr 			, value);
		IFC.Set( IFC.addr->KEY 	, key);
	};
void MainWindow::MAIN_slot_Hz()
{
	Slider_Hz( SDS, sds->Main_Freq, ui->Slider_Main_Hz->value(), MAINFREQUENCYKEY );
}

void MainWindow::Slider_VCO_Hz_changed(int value )
{
    float freq = ( value >= LFO_count ) 	?  value - LFO_count
    								:  (float) value / LFO_count;

    ui->VCOLCD_Hz->display( freq );

    Slider_Hz( SDS, sds->VCO_Freq, freq, VCOFREQUENCYKEY );
}


void MainWindow::Slider_FMO_Hz_changed(int value )
{
    float freq = ( value >= LFO_count ) 	?  value - LFO_count
    								:  (float) value / LFO_count;

    ui->FMOLCD_Hz->display( freq );

    Slider_Hz( SDS, sds->FMO_Freq, freq, FMOFREQUENCYKEY );
}

auto Slider_volume = []( Interface_class& IF, uint8_t& ch_ptr, char value, char key )
	{
		IF.Set( ch_ptr , value);
		IF.Set( IF.addr->KEY , key);
	};

void MainWindow::MAIN_slot_volume()
{
	Slider_volume( SDS, sds->Master_Amp, ui->Slider_Main_Vol->value(), MASTERAMP_KEY );
}
void MainWindow::VCO_slot_volume()
{
	Slider_volume( SDS, sds->VCO_Amp, ui->Slider_VCO_vol->value(), VCOAMPKEY );
}
void MainWindow::FMO_slot_volume()
{
	Slider_volume( SDS, sds->FMO_Amp, ui->Slider_FMO_vol->value(), FMOAMPKEY );
}


void MainWindow::start_composer()
{
    string Start_Composer = Server_cmd( Config.Term, file_structure().composer_bin, "" );
	system_execute( Start_Composer.data() );
    Comment( INFO, Start_Composer );
}

void MainWindow::start_audio_srv()
{
	if( sds->Rtsp == RUNNING ) return;
    string Start_Audio_Srv = Server_cmd( Config.Term, file_structure().audio_bin, "" );
	system_execute( Start_Audio_Srv.data() );
	Comment( INFO, Start_Audio_Srv );
}

void MainWindow::start_synthesizer()
{
	if( sds->Rtsp == RUNNING )
	{
	    if ( Sem.Getval( SEMAPHORE_START, GETVAL ) > 0 )
	    	Sem.Release( SEMAPHORE_START );
	    return;
	}
    string Start_Synthesizer = Server_cmd( Config.Term, file_structure().synth_bin, "" );
    system_execute( Start_Synthesizer.data() );
    Comment( INFO, Start_Synthesizer );
    Sem.Lock( SEMAPHORE_STARTED );
    MainWindow::setwidgetvalues(); // initData deploys the initial value the the QObjects-
    MainWindow::show(); // and the Mainwindow is updated.
}
void MainWindow::read_polygon_data()
{
    item->read_polygon_data();
};

void MainWindow::Controller_Exit()
{
	if ( sds->Rtsp == RUNNING )
	{
    if ( Sem.Getval( SEMAPHORE_EXIT, GETVAL ) > 0 )
    	Sem.Release( SEMAPHORE_EXIT );
    return;
	}
    sds->Synthesizer = EXITSERVER ;
}

void MainWindow::Audio_Exit()
{
    sds->AudioServer = EXITSERVER;
}

// button save config to default instrument
void MainWindow::Save_Config()
{
    SDS.Write_str( INSTRUMENTSTR_KEY, "default");
    SDS.Set( sds->KEY , SAVEINSTRUMENTKEY); //
}

void MainWindow::set_mode_f()
{
    SDS.Set( sds->KEY , RESETFMOKEY);
}

void MainWindow::set_mode_v()
{
    SDS.Set( sds->KEY , RESETVCOKEY); //
}
void MainWindow::set_mode_o()
{
    SDS.Set( sds->KEY , RESETMAINKEY); //
}

void MainWindow::connect_fmo()
{
    SDS.Set( sds->KEY , CONNECTFMOVCOKEY);
}

void MainWindow::connect_vco()
{
    SDS.Set( sds->KEY , CONNECTVCOFMOKEY);
}

int pb_value = 0;

void MainWindow::get_record_status( )
{
    pb_value = sds->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}
void MainWindow::toggle_Record()
{
    record	=	not sds->mixer_status.external;
    SDS.Set( sds->FileNo , 0); // automatic numbering
    SDS.Set( sds->KEY , RECORDWAVFILEKEY);

    if ( not record )
    {
        record_color.setColor(QPalette::Button, Qt::red);
        ui->pBtoggleRecord->setText("recording");
    }
    else
    {
        record_color.setColor(QPalette::Button, Qt::green);
        ui->pBtoggleRecord->setText("Record");
    }
    ui->pBtoggleRecord->setPalette(record_color);

}

void MainWindow::main_adsr_sustain()
{
    int value = ui->hs_adsr_sustain->value();
    SDS.Set( sds->Main_adsr.decay , value);
    SDS.Set( sds->KEY ,ADSR_KEY);

}


void MainWindow::pB_Debug_clicked()
{
    uint16_t counter = ( sds->WD_type_ID + 1 ) % 3;
    SDS.Set( sds->WD_type_ID , counter);
    SDS.Set( sds->KEY , WAVEDISPLAYTYPEKEY);

    ui->pB_Debug->setText( Qwavedisplay_type_str_vec[ counter ] );
}

void MainWindow::melody_connect()
{
};

void MainWindow::Updatewidgets()
{
    if (  sds->UserInterface == UPDATEGUI  )
    {
        if ( sds->KEY == INSTRUMENTSTR_KEY )
        {
            string str = SDS.Read_str( INSTRUMENTSTR_KEY );
            QString QStr ;
            if ( this->File_Dialog_obj != nullptr )
                this->File_Dialog_obj->CB_instruments->textActivated(QStr);
        }
        if ( not sds->Composer )
        {
			switch( sds->FLAG )
			{
				case RECORDWAVFILEFLAG :
				{
					Qread_filelist( CB_external,
									file_structure().Dir.musicdir, file_structure().wav_file_type);
					break;
				}
				case NEWINSTRUMENTFLAG :
				{
					Qread_filelist( this->File_Dialog_obj->CB_instruments,
									file_structure().Dir.instrumentdir, file_structure().file_type);
					break;
				}
				case NEWNOTESLINEFLAG :
				{
					Qread_filelist( this->File_Dialog_obj->CB_notes,
									file_structure().Dir.notesdir, file_structure().file_type);
					break;
				}
			}
        }

        setwidgetvalues();

    }
    if (sds->AudioServer == RUNNING )
        status_color.setColor(QPalette::Button, Qt::green);
    else
        status_color.setColor(QPalette::Button, Qt::red);
    ui->pBAudioServer->setPalette(status_color);


    if (sds->Synthesizer == RUNNING )
        status_color.setColor(QPalette::Button, Qt::green);
    else
        status_color.setColor(QPalette::Button, Qt::red);
    ui->pBSynthesizer->setPalette(status_color);


}



