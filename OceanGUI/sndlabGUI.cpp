
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

int set_slider( float f )
{
	return ( f < LFO_limit ) ? f * LFO_count : f + (float) LFO_count;
}

Ui::MainWindow	Ui_Mainwindow_obj{};

MainWindow::MainWindow(	QWidget *parent ) :
	Logfacility_class( Module )
//	ui(new Ui::MainWindow)

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
    connect( ui->pBSynthesizerExit, SIGNAL(clicked()), this, SLOT( Controller_Exit() ));
    connect( ui->pBAudioServer, 	SIGNAL(clicked()), this, SLOT( start_audio_srv() ));

    connect( ui->pBComposer, SIGNAL(clicked()), this, SLOT( start_composer() ));
    connect( ui->pBtoggleRecord, SIGNAL(clicked(bool)), this, SLOT(SaveRecord() ));
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


//    record_timer = new QTimer( this );
    connect(record_timer, &QTimer::timeout, this, &MainWindow::get_record_status);
    record_timer->start(1000);

//    status_timer = new QTimer( this );
    connect(status_timer, &QTimer::timeout, this, &MainWindow::Updatewidgets);
    status_timer->start(1000);

//    osc_timer = new QTimer( this );
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

//    scene               = new QGraphicsScene(this);
    ui->oscilloscope_view->setScene( scene );
    QRectF rect         = ui->oscilloscope_view->geometry();
    OszilloscopeWidget	OscWidg( DaTA->Sds.addr, rect );
    item = new OszilloscopeWidget( DaTA->Sds.addr, rect );
    scene->addItem( item );

//   File_Dialog_class File_Dialog_obj( this, ui->Slider_Main_Hz );
//   this->File_Dialog_p = new File_Dialog_class( this, ui->Slider_Main_Hz );


}

MainWindow::~MainWindow()
{
	delete item;
//	if ( ui ) delete ui;
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
        DaTA->Sds.Write_str( WAVEFILESTR_KEY, str );
        DaTA->Sds.Set(DaTA->Sds.addr->KEY , READ_EXTERNALWAVEFILE);
    }
}


void MainWindow::hs_hall_effect_value_changed(int value)
{
    DaTA->Sds.Set(DaTA->Sds.addr->Main_adsr.hall , value);
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , ADSR_KEY);
}

void MainWindow::pB_Wavedisplay_clicked()
{
    int wd_counter = (DaTA->Sds.addr->Wavedisplay_Id + 1) % wavedisplay_str_vec.size();
    QString QStr = QString::fromStdString(wavedisplay_str_vec[ wd_counter]);
    ui->pB_Wavedisplay->setText( QStr );
    DaTA->Sds.Set(DaTA->Sds.addr->Wavedisplay_Id , wd_counter);
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , SETWAVEDISPLAYKEY);
};

void MainWindow::dial_soft_freq_value_changed()
{
    int value = ui->dial_soft_freq->value();
    DaTA->Sds.Set(DaTA->Sds.addr->Soft_freq , value);
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , SOFTFREQUENCYKEY);

};

void MainWindow::dial_decay_value_changed()
{
    int dial = ui->hs_adsr_attack->value();
    DaTA->Sds.Set(DaTA->Sds.addr->Main_adsr.attack , dial);
    DaTA->Sds.Set(DaTA->Sds.addr->KEY, ADSR_KEY);

}
void MainWindow::dial_PMW_value_changed()
{
    int dial = ui->dial_PMW->value();
    DaTA->Sds.Set(DaTA->Sds.addr->PMW_dial , dial);
    DaTA->Sds.Set( DaTA->Sds.addr->KEY ,PMWDIALKEY);
}

void MainWindow::Rtsp_Dialog()
{
    if ( this->Rtsp_Dialog_p->isVisible()   )
        this->Rtsp_Dialog_p->hide();
    else
        this->Rtsp_Dialog_p->show();
}

void MainWindow::File_Director()
{
    if ( this->File_Dialog_p->isVisible()   )
        this->File_Dialog_p->hide();
    else
        this->File_Dialog_p->show();
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
	DaTA->Sds.Set( DaTA->Sds.addr->KEY , wf_key);
	label->setText( QWaveform_vec[ wfid ] );
}
void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &DaTA->Sds.addr->MAIN_spectrum.id, _wfid, MAINID, SETWAVEFORMMAINKEY, ui->wf_main );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &DaTA->Sds.addr->FMO_spectrum.id, _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &DaTA->Sds.addr->VCO_spectrum.id, _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}

auto toggle_mute( MainWindow* C, uint id,  int state )
{
	C->DaTA->Sds.addr->MIX_Id = id;
	C->DaTA->Sds.addr->KEY = TOGGLEMBPLAYKEY;
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
    C->DaTA->Sds.Set( C->DaTA->Sds.addr->MIX_Id , ID );
    if ( C->DaTA->Sds.addr->StA_state[ID].store )
    {
    	C->DaTA->Sds.Set( C->DaTA->Sds.addr->KEY , STOPRECORD_KEY);
    }
    else
    {
        C->DaTA->Sds.Set( C->DaTA->Sds.addr->KEY , STORESOUNDKEY);
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
    		DaTA->Sds.Set( DaTA->Sds.addr->MIX_Id, id);
    		rb->setChecked( false );
    	}
    	id++;
    }
    DaTA->Sds.Set( DaTA->Sds.addr->KEY, CLEAR_KEY );
}

auto mixer_slider( MainWindow* C, int ID, int value )
{
    C->DaTA->Sds.Set( C->DaTA->Sds.addr->MIX_Id , ID );
    C->DaTA->Sds.Set( C->DaTA->Sds.addr->StA_amp_arr[ID], value );
    C->DaTA->Sds.Set( C->DaTA->Sds.addr->KEY 	, SETMBAMPPLAYKEY);
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
    DaTA->Sds.Set( DaTA->Sds.addr->LOOP_end, value);
    DaTA->Sds.Set( DaTA->Sds.addr->LOOP_step, 1 );
    DaTA->Sds.Set( DaTA->Sds.addr->KEY, MASTERAMP_LOOP_KEY);
}

void MainWindow::Clear_Banks()
{
	for( QCheckBox* cb : cb_sta_vec )
		cb->setChecked(false);

    DaTA->Sds.Set( DaTA->Sds.addr->KEY , MUTEMBKEY);

}
void MainWindow::toggle_Mute()
{
    bool mute_flag 	= not DaTA->Sds.addr->mixer_status.mute ;
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , MASTERAMP_MUTE_KEY);
    QString Qstr = mute_flag ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
}

void MainWindow::cB_Beat_per_sec( int bps_id )
{
    DaTA->Sds.Set( DaTA->Sds.addr->Main_adsr.bps_id, bps_id  );
    DaTA->Sds.Set( DaTA->Sds.addr->KEY, ADSR_KEY );
}
void MainWindow::setwidgetvalues()
{

	ui->Pbar_telapsed->setValue( DaTA->Sds.addr->time_elapsed );


    ui->Slider_FMO_Hz->setValue(    set_slider( DaTA->Sds.addr->FMO_Freq) );
    ui->Slider_VCO_Hz->setValue(    set_slider( DaTA->Sds.addr->VCO_Freq) );
    ui->Slider_Main_Hz->setValue(   DaTA->Sds.addr->Main_Freq);

    ui->Slider_Main_Vol->setValue(  DaTA->Sds.addr->Master_Amp);
    ui->Slider_FMO_vol->setValue(   DaTA->Sds.addr->FMO_Amp);
    ui->Slider_VCO_vol->setValue(   DaTA->Sds.addr->VCO_Amp);

    int ID = 0;
    for( QRadioButton* rb : rb_sta_vec )
    {
    	rb->setChecked( DaTA->Sds.addr->StA_state[ID].store );
		ID++;
    }
    ID = 0;
    for( QCheckBox* cb : cb_sta_vec )
    {
    	cb->setChecked( DaTA->Sds.addr->StA_state[ID].play );
		ID++;
    }
    ID = 0;
    for( QSlider* sl : sl_sta_vec )
    {
    	sl->setValue( DaTA->Sds.addr->StA_amp_arr[ID] );
		ID++;
    }

    ui->labelVCO->setText("VCO");
    ui->labelFMO->setText("FMO");


    ui->wf_fmo->setText( QWaveform_vec[ DaTA->Sds.addr->FMO_spectrum.id ] );
    ui->wf_vco->setText( QWaveform_vec[ DaTA->Sds.addr->VCO_spectrum.id ] );
    ui->wf_main->setText( QWaveform_vec[ DaTA->Sds.addr->MAIN_spectrum.id ] );

    ui->sB_Main->setValue( DaTA->Sds.addr->MAIN_spectrum.id );
    ui->sB_FMO->setValue(  DaTA->Sds.addr->FMO_spectrum.id  );
    ui->sB_VCO->setValue(  DaTA->Sds.addr->VCO_spectrum.id  );

    ui->hs_adsr_sustain->setValue(  (int)DaTA->Sds.addr->Main_adsr.decay );
    ui->hs_adsr_attack->setValue(  (int) DaTA->Sds.addr->Main_adsr.attack);
    ui->dial_PMW->setValue( (int)DaTA->Sds.addr->PMW_dial  );
    ui->dial_soft_freq->setValue( (int)  DaTA->Sds.addr->Soft_freq );
    ui->hs_hall_effect->setValue( (int)  DaTA->Sds.addr->Main_adsr.hall );
    ui->progressBar_record->setValue(0);
    int wd_counter              = DaTA->Sds.addr->Wavedisplay_Id;
    QString Qstr = QString::fromStdString(wavedisplay_str_vec[wd_counter]);
    ui->pB_Wavedisplay->setText( Qstr );
    ui->pB_Debug->setText( Qwavedisplay_type_str_vec[ (int) DaTA->Sds.addr->WD_type_ID % 3 ] );

    ui->dial_ramp_up_down->setValue( DaTA->Sds.addr->Master_Amp);

    Qstr = DaTA->Sds.addr->mixer_status.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );

    DaTA->Sds.addr->UserInterface 	= RUNNING;
    DaTA->Sds.addr->UpdateFlag 		= true;

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
	Slider_Hz( DaTA->Sds, DaTA->Sds.addr->Main_Freq, ui->Slider_Main_Hz->value(), MAINFREQUENCYKEY );
}

void MainWindow::Slider_VCO_Hz_changed(int value )
{
    float freq = ( value >= LFO_count ) 	?  value - LFO_count
    								:  (float) value / LFO_count;

    ui->VCOLCD_Hz->display( freq );

    Slider_Hz( DaTA->Sds, DaTA->Sds.addr->VCO_Freq, freq, VCOFREQUENCYKEY );
}


void MainWindow::Slider_FMO_Hz_changed(int value )
{
    float freq = ( value >= LFO_count ) 	?  value - LFO_count
    								:  (float) value / LFO_count;

    ui->FMOLCD_Hz->display( freq );

    Slider_Hz( DaTA->Sds, DaTA->Sds.addr->FMO_Freq, freq, FMOFREQUENCYKEY );
}

auto Slider_volume = []( Interface_class& IF, uint8_t& ch_ptr, char value, char key )
	{
		IF.Set( ch_ptr , value);
		IF.Set( IF.addr->KEY , key);
	};

void MainWindow::MAIN_slot_volume()
{
	Slider_volume( DaTA->Sds, DaTA->Sds.addr->Master_Amp, ui->Slider_Main_Vol->value(), MASTERAMP_KEY );
}
void MainWindow::VCO_slot_volume()
{
	Slider_volume( DaTA->Sds, DaTA->Sds.addr->VCO_Amp, ui->Slider_VCO_vol->value(), VCOAMPKEY );
}
void MainWindow::FMO_slot_volume()
{
	Slider_volume( DaTA->Sds, DaTA->Sds.addr->FMO_Amp, ui->Slider_FMO_vol->value(), FMOAMPKEY );
}


void MainWindow::start_composer()
{
    string Start_Composer = Cfg->Server_cmd( Cfg->Config.Term, file_structure().composer_bin, "" );
	system_execute( Start_Composer.data() );
}

void MainWindow::start_audio_srv()
{
	if( DaTA->Sds.addr->Rtsp == RUNNING ) return;
    string Start_Audio_Srv = Cfg->Server_cmd( Cfg->Config.Term, file_structure().audio_bin, "" );
	system_execute( Start_Audio_Srv.data() );
}

void MainWindow::start_synthesizer()
{
	if( DaTA->Sds.addr->Rtsp == RUNNING )
	{
	    if ( Sem->Getval( SYNTHESIZER_START, GETVAL ) > 0 )
	    	Sem->Release( SYNTHESIZER_START );
	    return;
	}
    string Start_Synthesizer = Cfg->Server_cmd( Cfg->Config.Term, file_structure().synth_bin, "" );
    system_execute( Start_Synthesizer.data() );
    Sem->Lock( SEMAPHORE_STARTED );
    MainWindow::setwidgetvalues(); // initData deploys the initial value the the QObjects-
    MainWindow::show(); // and the Mainwindow is updated.
}
void MainWindow::read_polygon_data()
{
    item->read_polygon_data();
};

void MainWindow::Controller_Exit()
{
	if ( DaTA->Sds.addr->Rtsp == RUNNING )
	{
    if ( Sem->Getval( SEMAPHORE_EXIT, GETVAL ) > 0 )
    	Sem->Release( SEMAPHORE_EXIT );
    return;
	}
    DaTA->Sds.addr->Synthesizer = EXITSERVER ;
}

void MainWindow::Audio_Exit()
{
    DaTA->Sds.addr->AudioServer = EXITSERVER;
}

// button save Cfg->Config to default instrument
void MainWindow::Save_Config()
{
    DaTA->Sds.Write_str( INSTRUMENTSTR_KEY, "default");
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , SAVEINSTRUMENTKEY); //
}

void MainWindow::set_mode_f()
{
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , RESETFMOKEY);
}

void MainWindow::set_mode_v()
{
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , RESETVCOKEY); //
}
void MainWindow::set_mode_o()
{
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , RESETMAINKEY); //
}

void MainWindow::connect_fmo()
{
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , CONNECTFMOVCOKEY);
}

void MainWindow::connect_vco()
{
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , CONNECTVCOFMOKEY);
}

int pb_value = 0;

void MainWindow::get_record_status( )
{
    pb_value = DaTA->Sds.addr->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}
void MainWindow::SaveRecord()
{
    DaTA->Sds.Set( DaTA->Sds.addr->FileNo , 0); // automatic numbering
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , SAVE_EXTERNALWAVFILEKEY);
}

void MainWindow::main_adsr_sustain()
{
    int value = ui->hs_adsr_sustain->value();
    DaTA->Sds.Set( DaTA->Sds.addr->Main_adsr.decay , value);
    DaTA->Sds.Set( DaTA->Sds.addr->KEY ,ADSR_KEY);

}


void MainWindow::pB_Debug_clicked()
{
    uint16_t counter = ( DaTA->Sds.addr->WD_type_ID + 1 ) % 3;
    DaTA->Sds.Set( DaTA->Sds.addr->WD_type_ID , counter);
    DaTA->Sds.Set( DaTA->Sds.addr->KEY , WAVEDISPLAYTYPEKEY);

    ui->pB_Debug->setText( Qwavedisplay_type_str_vec[ counter ] );
}

void MainWindow::melody_connect()
{
};

void MainWindow::Updatewidgets()
{
    if (  DaTA->Sds.addr->UserInterface == UPDATEGUI  )
    {
        if ( DaTA->Sds.addr->KEY == INSTRUMENTSTR_KEY )
        {
            string str = DaTA->Sds.Read_str( INSTRUMENTSTR_KEY );
            QString QStr ;
            if ( this->File_Dialog_p != nullptr )
                this->File_Dialog_p->CB_instruments->textActivated(QStr);
        }
        if ( not DaTA->Sds.addr->Composer )
        {
			switch( DaTA->Sds.addr->FLAG )
			{
				case RECORDWAVFILEFLAG :
				{
					Qread_filelist( CB_external,
									file_structure().Dir.musicdir, file_structure().wav_file_type);
					break;
				}
				case NEWINSTRUMENTFLAG :
				{
					Qread_filelist( this->File_Dialog_p->CB_instruments,
									file_structure().Dir.instrumentdir, file_structure().file_type);
					break;
				}
				case NEWNOTESLINEFLAG :
				{
					Qread_filelist( this->File_Dialog_p->CB_notes,
									file_structure().Dir.notesdir, file_structure().file_type);
					break;
				}
			}
        }

    	DaTA->Sds.addr->UserInterface = OFFLINE ;
    	cout << hex << DaTA->Sds.addr << endl;
    	DaTA->Sds.addr = DaTA->SetSds( Rtsp_Dialog_obj.SDS_ID );
    	cout << hex << DaTA->Sds.addr << endl;
    	Rtsp_Dialog_obj.sds = DaTA->Sds.addr;

        setwidgetvalues();

    }

    if (DaTA->Sds.addr->AudioServer == RUNNING )
        status_color.setColor(QPalette::Button, Qt::green);
    else
        status_color.setColor(QPalette::Button, Qt::red);
    ui->pBAudioServer->setPalette(status_color);


    if (DaTA->Sds.addr->Synthesizer == RUNNING )
        status_color.setColor(QPalette::Button, Qt::green);
    else
        status_color.setColor(QPalette::Button, Qt::red);
    ui->pBSynthesizer->setPalette(status_color);


}



