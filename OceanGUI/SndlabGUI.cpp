
// qtcreator
#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <oszilloscopewidget.h>
#include <spectrum_dialog_class.h>

// Synhesizer

#include <synthesizer.h>
#include <Wavedisplay.h>
#include <mixer.h>
#include <keys.h>
#include <Logfacility.h>

// Qt
#include <QLabel>
#include <QByteArray>
#include <QPolygon>
#include <QTimer>
#include <QRect>
#include <QEvent>



// System
#include <unistd.h> //sleep

MainWindow::MainWindow(QWidget *parent)
    :
     QMainWindow(parent),
	 ui(new Ui::MainWindow)
{


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

    ui->setupUi(this);

    // https://stackoverflow.com/questions/17095957/qt-creator-and-main-window-background-image
    QString Ocean_png = QString::fromStdString( dir_struct().libdir + "Ocean.png" );
    QPixmap bkgnd( Ocean_png );
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Button, QColor(0,179,255) );
    this->setPalette(palette);

    connect( ui->pBtoggleRecord, SIGNAL(clicked(bool)), this, SLOT(toggle_Record() ));
    connect( ui->pB_Store, SIGNAL(clicked()), this, SLOT(Store()));
    connect( ui->pB_Mute, SIGNAL(clicked()), this, SLOT(toggle_Mute()));
    connect( ui->pb_clear, SIGNAL(clicked()), this, SLOT(memory_clear()));
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

    connect(ui->sB_Duration    , SIGNAL(valueChanged(int)), this, SLOT(sB_Duration(int) ));
    connect(ui->hs_adsr_sustain, SIGNAL(valueChanged(int)), this, SLOT(main_adsr_sustain() ));
    connect(ui->pB_Wavedisplay , SIGNAL(clicked()), this, SLOT(pB_Wavedisplay_clicked()));

    connect(ui->Slider_mix_vol1, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix1(int) ));
    connect(ui->Slider_mix_vol2, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix2(int) ));
    connect(ui->Slider_mix_vol3, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix3(int) ));
    connect(ui->Slider_mix_vol4, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix4(int) ));
    connect(ui->Slider_mix_vol5, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix5(int) ));
    connect(ui->Slider_mix_vol6, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix6(int) ));
    connect(ui->Slider_mix_vol7, SIGNAL(valueChanged(int)), this, SLOT(Sl_mix7(int) ));

    connect(ui->pB_Debug, SIGNAL(clicked()), this, SLOT(pB_Debug_clicked()));
    connect(ui->cb_external, SIGNAL(activated(QString)), this, SLOT(wavfile_selected(QString)));

    connect(ui->hs_hall_effect, SIGNAL(valueChanged(int)), this, SLOT(hs_hall_effect_value_changed(int)));

    connect( ui->sB_FMO , SIGNAL( valueChanged(int)), this, SLOT(FMO_Waveform_slot( int ))) ;
    connect( ui->sB_VCO , SIGNAL( valueChanged(int)), this, SLOT(VCO_Waveform_slot( int ))) ;
    connect( ui->sB_Main, SIGNAL( valueChanged(int)), this, SLOT(Main_Waveform_slot( int ))) ;


    QTimer* record_timer = new QTimer( this );
    connect(record_timer, &QTimer::timeout, this, &MainWindow::get_record_status);
    record_timer->start(1000);

    QTimer* status_timer = new QTimer( this );
    connect(status_timer, &QTimer::timeout, this, &MainWindow::Updatewidgets);
    status_timer->start(1000);

    QTimer* osc_timer = new QTimer( this );
    connect(osc_timer, &QTimer::timeout, this, &MainWindow::read_polygon_data );
    osc_timer->start(20); // 50 Hz

    uint sb_max = QWaveform_vec.size()-1;
    ui->sB_Main->setMaximum(sb_max);
    ui->sB_FMO->setMaximum(sb_max);
    ui->sB_VCO->setMaximum(sb_max);

    CB_external         = ui->cb_external;
    string wavfile_path = dir_struct().musicdir;
    read_filelist( CB_external, wavfile_path, "wav");

    setwidgetvalues();

    scene               = new QGraphicsScene(this);
    ui->oscilloscope_view->setScene( scene );
    QRectF rect         = ui->oscilloscope_view->geometry();
    item                = new OszilloscopeWidget( GUI.addr, rect );
    scene->addItem( item );


    GUI.Announce( "SndlabGUI", true );
}

MainWindow::~MainWindow()
{
    delete ui;
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

//int duration_counter = 0;

void MainWindow::wavfile_selected( const QString &arg)
{
    qDebug() << "WAV file " << arg ;
    QString QStr = arg;
    string str = QStr.toStdString();
    if ( str.length() > 0 )
    {
        GUI.Write_str( WAVEFILESTR_KEY, str );
        GUI.Set(GUI.addr->KEY , SETEXTERNALWAVEFILE);
    }
}


void MainWindow::hs_hall_effect_value_changed(int value)
{
    GUI.Set(GUI.addr->Main_adsr_hall , value);
    GUI.Set( GUI.addr->KEY , ADSRHALLKEY);
}

void MainWindow::pB_Wavedisplay_clicked()
{
    int wd_counter = (GUI.addr->Wavedisplay_Id + 1) % wavedisplay_str_vec.size();
    QString QStr = QString::fromStdString(wavedisplay_str_vec[ wd_counter]);
    ui->pB_Wavedisplay->setText( QStr );
    GUI.Set(GUI.addr->Wavedisplay_Id , wd_counter);
    GUI.Set( GUI.addr->KEY , SETWAVEDISPLAYKEY);
};

void MainWindow::dial_soft_freq_value_changed()
{
    int value = ui->dial_soft_freq->value();
    GUI.Set(GUI.addr->Soft_freq , value);
    GUI.Set( GUI.addr->KEY , SOFTFREQUENCYKEY);

};

void MainWindow::dial_decay_value_changed()
{
    int dial = ui->hs_adsr_attack->value();
    GUI.Set(GUI.addr->Main_adsr_attack , dial);
    GUI.Set(GUI.addr->KEY, ADSRDECAYKEY);

}
void MainWindow::dial_PMW_value_changed()
{
    int dial = ui->dial_PMW->value();
    GUI.Set(GUI.addr->PMW_dial , dial);
    GUI.Set( GUI.addr->KEY ,PMWDIALKEY);
}

void MainWindow::File_Director()
{
    if ( this->Dialog_File == nullptr )
    {
        this->Dialog_File = new File_Dialog_class( this );
    }
    if ( this->Dialog_File->isVisible()   )
        this->Dialog_File->hide();
    else
        this->Dialog_File->show();
}

void MainWindow::Spectrum_Dialog()
{
    if ( this->Spectrum_Dialog_Obj == nullptr )
    {
        this->Spectrum_Dialog_Obj = new Spectrum_Dialog_class( this, &GUI );
    }
    if ( this->Spectrum_Dialog_Obj->isVisible()   )
        this->Spectrum_Dialog_Obj->hide();
    else
        this->Spectrum_Dialog_Obj->show();
}

void MainWindow::waveform_slot(char* wf_addr, char wfid, int ID, int wf_key, QLabel* label  )
{
	*wf_addr = wfid;
	GUI.Set( GUI.addr->KEY , wf_key);
	label->setText( QWaveform_vec[ wfid ] );
}
void MainWindow::Main_Waveform_slot( int _wfid )
{
	waveform_slot( &GUI.addr->MAIN_spectrum.id, _wfid, MAINID, SETWAVEFORMMAINKEY, ui->wf_main );
}
void MainWindow::FMO_Waveform_slot(int _wfid)
{
	waveform_slot( &GUI.addr->FMO_spectrum.id, _wfid, FMOID, SETWAVEFORMFMOKEY, ui->wf_fmo );
}
void MainWindow::VCO_Waveform_slot( int _wfid )
{
	waveform_slot( &GUI.addr->VCO_spectrum.id, _wfid, VCOID, SETWAVEFORMVCOKEY, ui->wf_vco );
}

void MainWindow::change_status1()
{
    GUI.Set( GUI.addr->MIX_Id , 0);
    GUI.Set( GUI.addr->KEY , TOGGLEMBPLAYKEY); //

}
void MainWindow::change_status2()
{
    GUI.Set( GUI.addr->MIX_Id , 1);
    GUI.Set( GUI.addr->KEY , TOGGLEMBPLAYKEY); //

}
void MainWindow::change_status3()
{
    GUI.Set( GUI.addr->MIX_Id , 2);
    GUI.Set( GUI.addr->KEY , TOGGLEMBPLAYKEY); //

}

void MainWindow::change_status4()
{
    GUI.Set( GUI.addr->MIX_Id , MbIdExternal);
    GUI.Set( GUI.addr->KEY , TOGGLEMBPLAYKEY); //

}

void MainWindow::change_status5()
{
    GUI.Set( GUI.addr->MIX_Id , MbIdNotes);
    GUI.Set( GUI.addr->KEY , TOGGLEMBPLAYKEY); //

}

void MainWindow::Store()
{
    struct store_widgets
    {
        QRadioButton*   rb;
        QCheckBox*      cb;
    };
    vector< store_widgets> store_widget_v{
        { ui->rb_bank1,ui->cb_1},
        { ui->rb_bank2,ui->cb_2},
        { ui->rb_bank3,ui->cb_3},
        { ui->rb_bank4,ui->cb_4},
        { ui->rb_bank5,ui->cb_5},
        { ui->rb_bank7,ui->cb_7}
    };
    char banknr=0;
    int i = 0;
    for ( store_widgets stw : store_widget_v )
    {
        if ( stw.rb->isChecked() )
        {
            banknr = i;
            if ( ! stw.cb->isChecked())
                stw.cb->click();
        }
        i++;
    }
    GUI.Set( GUI.addr->MIX_Id , banknr); //
    bool status_store = not GUI.addr->ma_status[banknr].store;
    GUI.addr->ma_status[banknr].store = status_store;
    if ( status_store )
        GUI.Set( GUI.addr->KEY , STOPRECORD_KEY);
    else
        GUI.Set( GUI.addr->KEY , STORESOUNDKEY);
}

void MainWindow::memory_clear()
{
	vector<uint> rb_ids = {0,1,2,3,4,MbIdExternal };
    int Id = 0; // if no one is checked, the current Id is cleared,
                // that should be ok.
    for ( QRadioButton* rb : {  ui->rb_bank1,
                                ui->rb_bank2,
                                ui->rb_bank3,
                                ui->rb_bank4,
                                ui->rb_bank5,
								ui->rb_bank7
                            })
    {
        if( rb->isChecked() )
        {
            GUI.Set( GUI.addr->MIX_Id, rb_ids[Id] );
        }
        Id++;
    }
    GUI.Set( GUI.addr->KEY, CLEAR_KEY );
}

auto mixer_slider( MainWindow* C, int ID, int value )
{
    C->GUI.Set( C->GUI.addr->MIX_Amp, value);
    C->GUI.Set( C->GUI.addr->MIX_Id , ID );
    C->GUI.Set( C->GUI.addr->KEY 	, SETMBAMPPLAYKEY);
};

void MainWindow::Sl_mix1( int value )
{
	mixer_slider( this, 0, value );
};
void MainWindow::Sl_mix2( int value )
{
	mixer_slider( this, 1, value );
};
void MainWindow::Sl_mix3( int value )
{
	mixer_slider( this, 2, value );
};
void MainWindow::Sl_mix4( int value )
{
	mixer_slider( this, 3, value );
};
void MainWindow::Sl_mix5( int value )
{
	mixer_slider( this, 4, value );
};
void MainWindow::Sl_mix6( int value ) // Notes volume
{
	mixer_slider( this, 5, value );
};
void MainWindow::Sl_mix7( int value )
{
	mixer_slider( this, MbIdExternal, value );
};

void MainWindow::slot_dial_ramp_up_down()
{
    float value = ui->dial_ramp_up_down->value();
    GUI.Set( GUI.addr->LOOP_end, value);
    GUI.Set( GUI.addr->LOOP_step, 1 );
    GUI.Set( GUI.addr->KEY, MASTERAMP_LOOP_KEY);
}

void MainWindow::Clear_Banks()
{
    if ( ui->cb_1->isChecked())
        ui->cb_1->click();
    if ( ui->cb_3->isChecked())
        ui->cb_3->click();
    if ( ui->cb_2->isChecked())
        ui->cb_2->click();
    if ( ui->cb_4->isChecked())
        ui->cb_4->click();

    GUI.Set( GUI.addr->KEY , MUTEMBKEY);

}
void MainWindow::toggle_Mute()
{
    bool mute_flag 	= not GUI.addr->mi_status.mute;
    GUI.Set( GUI.addr->mi_status.mute , mute_flag );
    GUI.Set( GUI.addr->KEY , MASTERAMP_MUTE_KEY);
    QString Qstr = mute_flag ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
}

void MainWindow::sB_Duration( int bps_id  )
{
    QString Qstr = get_bps_qstring( bps_id );
    ui->Bps->setText( Qstr );
    GUI.Set( GUI.addr->Main_adsr_bps_id, bps_id);
    GUI.Set( GUI.addr->KEY, ADSRDURATIONKEY );
}
void MainWindow::setwidgetvalues()
{

    ui->dial_PMW->setValue( (int)GUI.addr->PMW_dial  );

    int slider_value = ( GUI.addr->FMO_Freq < LFO_limit ) ? GUI.addr->FMO_Freq * LFO_count:
    														GUI.addr->FMO_Freq + (float)LFO_count;
    ui->Slider_FMO_Hz->setValue(    slider_value );
    ui->Slider_VCO_Hz->setValue(    GUI.addr->VCO_Freq);
    ui->Slider_Main_Hz->setValue(   GUI.addr->Main_Freq);

    ui->Slider_Main_Vol->setValue(  GUI.addr->Master_Amp);
    ui->Slider_FMO_vol->setValue(   GUI.addr->FMO_Amp);
    ui->Slider_VCO_vol->setValue(   GUI.addr->VCO_Amp);

    ui->Slider_mix_vol1->setValue(75);
    ui->Slider_mix_vol2->setValue(75);
    ui->Slider_mix_vol3->setValue(75);
    ui->Slider_mix_vol4->setValue(75);
    ui->Slider_mix_vol6->setValue(75);
    ui->Slider_mix_vol7->setValue(100);

    ui->labelVCO->setText("VCO");
    ui->labelFMO->setText("FMO");


    ui->wf_fmo->setText( QWaveform_vec[ GUI.addr->FMO_spectrum.id ] );
    ui->wf_vco->setText( QWaveform_vec[ GUI.addr->VCO_spectrum.id ] );
    ui->wf_main->setText( QWaveform_vec[ GUI.addr->MAIN_spectrum.id ] );

    ui->sB_Main->setValue( GUI.addr->MAIN_spectrum.id );
    ui->sB_FMO->setValue(  GUI.addr->FMO_spectrum.id  );
    ui->sB_VCO->setValue(  GUI.addr->VCO_spectrum.id  );

    ui->sB_Duration->setValue( GUI.addr->Main_adsr_bps_id );
    QString Qstr = get_bps_qstring( GUI.addr->Main_adsr_bps_id );
    ui->Bps->setText( Qstr );
    ui->hs_adsr_sustain->setValue(  (int) GUI.addr->Main_adsr_decay );
    ui->hs_adsr_attack->setValue(  (int)     GUI.addr->Main_adsr_attack);
    ui->dial_soft_freq->setValue( (int)  GUI.addr->Soft_freq );
    ui->hs_hall_effect->setValue( (int)  GUI.addr->Main_adsr_hall );
    ui->progressBar_record->setValue(0);
    int wd_counter              = GUI.addr->Wavedisplay_Id;
    Qstr = QString::fromStdString(wavedisplay_str_vec[wd_counter]);
    ui->pB_Wavedisplay->setText( Qstr );
    ui->pB_Debug->setText( Qwavedisplay_type_str_vec[ (int) GUI.addr->WD_type_ID % 3 ] );

    ui->dial_ramp_up_down->setValue( GUI.addr->Master_Amp);

    Qstr = GUI.addr->mi_status.mute ? "UnMute" : "Mute";
    ui->pB_Mute->setText( Qstr );
    MainWindow::show();
}

void MainWindow::GUI_Exit()
{
    qDebug("%s", "Exit" );
    GUI.Announce( "SndlabGUI", false );

    shmdt( GUI.addr );
    QApplication::quit();
}

auto Slider_Hz = []( Interface_class& IF, float& fptr, float value, char key )
	{
		IF.Set( fptr , value);
		IF.Set( IF.addr->KEY , key);
	};
void MainWindow::MAIN_slot_Hz()
{
	Slider_Hz( GUI, GUI.addr->Main_Freq, ui->Slider_Main_Hz->value(), MAINFREQUENCYKEY );
}

void MainWindow::VCO_slot_Hz()
{
	Slider_Hz( GUI, GUI.addr->VCO_Freq, ui->Slider_VCO_Hz->value(), VCOFREQUENCYKEY );
}
void MainWindow::Slider_FMO_Hz_changed(int value )
{
    float freq = 0.0;
    if ( value >= LFO_count )
    {
    	freq = value - ( LFO_count );
    }
    else
    {
    	freq = (float) value / LFO_count;
    }
    ui->FMOLCD_Hz->display( freq );

    Slider_Hz( GUI, GUI.addr->FMO_Freq, freq, FMOFREQUENCYKEY );
}

auto Slider_volume = []( Interface_class& IF, char& ch_ptr, char value, char key )
	{
		IF.Set( ch_ptr , value);
		IF.Set( IF.addr->KEY , key);
	};

void MainWindow::MAIN_slot_volume()
{
	Slider_volume( GUI, GUI.addr->Master_Amp, ui->Slider_Main_Vol->value(), MASTERAMP_KEY );
}
void MainWindow::VCO_slot_volume()
{
	Slider_volume( GUI, GUI.addr->VCO_Amp, ui->Slider_VCO_vol->value(), VCOAMPKEY );
}
void MainWindow::FMO_slot_volume()
{
	Slider_volume( GUI, GUI.addr->FMO_Amp, ui->Slider_FMO_vol->value(), FMOAMPKEY );
}




void MainWindow::start_srv()
{
//    string cmd = "xterm -e '" + file_structure().audio_bin + " 2 44100' &";
    string Start_Audio_Srv = Server_struct().cmd( Audio_Srv, "" );
	system_execute( Start_Audio_Srv.data() );
    Log.Comment( Log.INFO, Start_Audio_Srv );
}

void MainWindow::start_synthesizer()
{
    string Start_Synthesizer = Server_struct().cmd( Synthesizer, "" );
    system_execute( Start_Synthesizer.data() );
    Log.Comment( Log.INFO, Start_Synthesizer );
    Wait( 2*SECOND ); 	// WAIT until the startup of the process finished.
    					// the synthesizer process will prepare the initial values
                        // from the keyboard file, that are stored into the GUI.ifd_data structure
    MainWindow::setwidgetvalues(); // initData deploys the initial value the the QObjects-
    MainWindow::show(); // and the Mainwindow is updated.
}
void MainWindow::read_polygon_data()
{
    item->read_polygon_data();
};

void MainWindow::Controller_Exit()
{
    GUI.Set( GUI.addr->Synthesizer , EXITSERVER ); //


}

void MainWindow::Audio_Exit()
{
    GUI.Set( GUI.addr->AudioServer , STOPSNDSRV);
}

// button save config to default instrument
void MainWindow::Save_Config()
{
    GUI.Write_str( INSTRUMENTSTR_KEY, "default");
    GUI.Set( GUI.addr->KEY , SAVEINSTRUMENTKEY); //
}

void MainWindow::set_mode_f()
{
    GUI.Set( GUI.addr->KEY , RESETFMOKEY);
}

void MainWindow::set_mode_v()
{
    GUI.Set( GUI.addr->KEY , RESETVCOKEY); //
}
void MainWindow::set_mode_o()
{
    GUI.Set( GUI.addr->KEY , RESETMAINKEY); //
}

void MainWindow::connect_fmo()
{
    GUI.Set( GUI.addr->KEY , CONNECTFMOVCOKEY);
}

void MainWindow::connect_vco()
{
    GUI.Set( GUI.addr->KEY , CONNECTVCOFMOKEY);
}

int pb_value = 0;

void MainWindow::get_record_status( )
{
    pb_value = GUI.addr->RecCounter;
    ui->progressBar_record->setValue( pb_value );
}
void MainWindow::toggle_Record()
{
    record=! record;
    if (record)
    {
        record_color.setColor(QPalette::Button, Qt::red);
        ui->pBtoggleRecord->setText("recording");
        GUI.Set( GUI.addr->FileNo , 0); // automatic numbering
        GUI.Set( GUI.addr->KEY , RECORDWAVFILEKEY);
    }
    else
    {
        record_color.setColor(QPalette::Button, Qt::green);
        ui->pBtoggleRecord->setText("Record");
        GUI.Set( GUI.addr->KEY , RECORDWAVFILEKEY);
    }
    ui->pBtoggleRecord->setPalette(record_color);

}

void MainWindow::main_adsr_sustain()
{
    int value = ui->hs_adsr_sustain->value();
    GUI.Set( GUI.addr->Main_adsr_decay , value);
    GUI.Set( GUI.addr->KEY ,ADSRSUSTAINKEY);

}


void MainWindow::pB_Debug_clicked()
{
    uint16_t counter = ( GUI.addr->WD_type_ID + 1 ) % 3;
    GUI.Set( GUI.addr->WD_type_ID , counter);
    GUI.Set( GUI.addr->KEY , WAVEDISPLAYTYPEKEY);

    ui->pB_Debug->setText( Qwavedisplay_type_str_vec[ counter ] );
}

void MainWindow::melody_connect()
{
};

void MainWindow::Updatewidgets()
{
    if (  GUI.addr->UserInterface == UPDATEGUI  )
    {
        if ( GUI.addr->KEY == INSTRUMENTSTR_KEY )
        {
            string str = GUI.Read_str( INSTRUMENTSTR_KEY );
            QString QStr ;
            if ( this->Dialog_File != nullptr )
                this->Dialog_File->CB_instruments->textActivated(QStr);
        }
        if ( not GUI.addr->Composer )
        {
        switch( GUI.addr->FLAG )
        {
            case RECORDWAVFILEFLAG :
            {
                read_filelist(CB_external, dir_struct().musicdir, "wav");
                break;
            }
            case NEWINSTRUMENTFLAG :
            {
                read_filelist(this->Dialog_File->CB_instruments, dir_struct().instrumentdir, "kbd");
                break;
            }
            case NEWNOTESLINEFLAG :
            {
                read_filelist(this->Dialog_File->CB_notes, dir_struct().notesdir, "kbd");
                break;
            }
        }
        }

        setwidgetvalues();

        GUI.Set( GUI.addr->UserInterface , RUNNING);
    }
    if (GUI.addr->AudioServer == RUNSNDSRV )
        status_color.setColor(QPalette::Button, Qt::green);
    else
        status_color.setColor(QPalette::Button, Qt::red);
    ui->pBAudioServer->setPalette(status_color);


    if (GUI.addr->Synthesizer == RUNNING )
        status_color.setColor(QPalette::Button, Qt::green);
    else
        status_color.setColor(QPalette::Button, Qt::red);
    ui->pBSynthesizer->setPalette(status_color);


}



