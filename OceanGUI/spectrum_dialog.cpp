#include <Spectrum_dialog_class.h>

//Ui::Spectrum_Dialog_class	UI_obj;

Spectrum_Dialog_class::Spectrum_Dialog_class(QWidget *parent,
                                             Interface_class* 	gui,
											 EventLog_class*	_log) :
    Logfacility_class("Spectrum"),
	Spectrum_class(),
	QDialog(parent),
	ui(new Ui::Spectrum_Dialog_class)
{
//	ui = &UI_obj;

    ui->setupUi(this);
    Waveform_vec = Vstringvector( Get_waveform_vec() );
    waveform_vec_len = Waveform_vec.size();

    connect( ui->fS_1, SIGNAL(valueChanged(int)), this, SLOT(fS1( int )));
    connect( ui->vS_2, SIGNAL(valueChanged(int)), this, SLOT(vS2( int )));
    connect( ui->fS_3, SIGNAL(valueChanged(int)), this, SLOT(fS3( int )));
    connect( ui->vS_4, SIGNAL(valueChanged(int)), this, SLOT(vS4( int )));
    connect( ui->fS_5, SIGNAL(valueChanged(int)), this, SLOT(fS5( int )));
    connect( ui->vS_6, SIGNAL(valueChanged(int)), this, SLOT(vS6( int )));
    connect( ui->fS_7, SIGNAL(valueChanged(int)), this, SLOT(fS7( int )));
    connect( ui->vS_8, SIGNAL(valueChanged(int)), this, SLOT(vS8( int )));

    connect( ui->sb_spwf1, SIGNAL(valueChanged(int)), this, SLOT( sb_wf1(int) ));
    connect( ui->sb_spwf2, SIGNAL(valueChanged(int)), this, SLOT( sb_wf2(int) ));
    connect( ui->sb_spwf3, SIGNAL(valueChanged(int)), this, SLOT( sb_wf3(int) ));
    connect( ui->sb_spwf4, SIGNAL(valueChanged(int)), this, SLOT( sb_wf4(int) ));
    connect( ui->sb_adsrwf,SIGNAL(valueChanged(int)), this, SLOT( adsrwf(int) ));

    connect( ui->rb_spec_fmo,  SIGNAL( clicked()), this, SLOT( select_spec_fmo() ) );
    connect( ui->rb_spec_vco,  SIGNAL( clicked()), this, SLOT( select_spec_vco() ) );
    connect( ui->rb_spec_main, SIGNAL( clicked()), this, SLOT( select_spec_main() ));

    connect( ui->pB_save_spectrum, 	SIGNAL(clicked()), this, SLOT(save( )));
    connect( ui->rb_reset, 			SIGNAL(clicked()), this, SLOT(reset( )));

    connect( ui->hs_attack,	SIGNAL(valueChanged(int)), 	this, SLOT(attack(int) ));
    connect( ui->hs_decay,	SIGNAL(valueChanged(int)), 	this, SLOT(decay(int) ));

    this->Sds   		    = gui;
    sds_p 					= Sds->addr;
    Eventlog_p				= _log;

    sds_spectrum_vec 		= { &sds_p->VCO_spectrum,
								&sds_p->FMO_spectrum,
								&sds_p->OSC_spectrum};
    sds_adsr_vec			= { &sds_p->VCO_adsr,
    							&sds_p->FMO_adsr,
								&sds_p->OSC_adsr };

    this->instrument   		= this->Sds->Read_str( INSTRUMENTSTR_KEY );

    this->SetSds( Sds, 0 );
    select_spec( sds_p->Spectrum_type );

    Setup_widgets(  this->spectrum );
}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
//   if( ui) delete(ui);
}

void Spectrum_Dialog_class::select_spec( char oscid )
{
	sds_p->Spectrum_type 	= oscid;
	OscId					= oscid;
	if ( ADSR_flag )
	{
	    Sds->Set( sds_p->WD_status.oscId, (uint8_t) oscid );
	    Sds->Set( sds_p->WD_status.roleId, (uint8_t) osc_struct::ADSRID );
		spectrum				= sds_adsr_vec[ oscid ]->spec;
	}
	else
	{
	    Sds->Set( sds_p->WD_status.oscId, (uint8_t) oscid );
	    Sds->Set( sds_p->WD_status.roleId, (uint8_t) osc_struct::INSTRID );
		spectrum				= *sds_spectrum_vec[ oscid ];
	}
	Eventlog_p->add( SDS_ID, SETWAVEDISPLAYKEY );

	Setup_widgets( spectrum );
};

void Spectrum_Dialog_class::select_spec_fmo()
{
	select_spec( osc_struct::FMOID );
}
void Spectrum_Dialog_class::select_spec_vco()
{
	select_spec( osc_struct::VCOID );
}
void Spectrum_Dialog_class::select_spec_main()
{
	select_spec( osc_struct::OSCID );
}
void Spectrum_Dialog_class::Set_adsr_flag( bool flag )
{
	ADSR_flag 				= flag;
//	ui->cb_adsr->setChecked( ADSR_flag );
	select_spec( sds_p->Spectrum_type  );

}

void Spectrum_Dialog_class::attack(int value )
{
	Sds->Set( sds_adsr_vec[ OscId ]->attack, (uint8_t) value );
	Eventlog_p->add( SDS_ID, ADSR_KEY );
}
void Spectrum_Dialog_class::decay (int value )
{
	Sds->Set( sds_adsr_vec[ OscId ]->decay, (uint8_t) value );
	Eventlog_p->add( SDS_ID, ADSR_KEY );
}

void Spectrum_Dialog_class::reset()
{
	spectrum.volidx	= spec_struct().volidx;
	spectrum.frqidx = spec_struct().frqidx;
	Setup_widgets( spectrum );
	ui->rb_reset->setChecked( false );
}



void Spectrum_Dialog_class::Update_spectrum()
{
    string  newinstrument      = this->Sds->Read_str( INSTRUMENTSTR_KEY );

    if ( newinstrument.compare( instrument ) != 0 )
    {
        instrument 	= newinstrument;
        spectrum 	= *sds_spectrum_vec[ sds_p->Spectrum_type ];
        Setup_widgets( spectrum );
        return;
    }
}

auto spec_frq_slider = []( Spectrum_Dialog_class& C, int channel, int value )
{
    char oscid = C.sds_p->Spectrum_type;
	C.spectrum = *C.sds_spectrum_vec[ oscid ];
	C.spectrum.frqidx[ channel ] = check_range(  frqarr_range, value, "spec_frq_slider");
	float frqadj =  C.Spectrum.Frqadj(channel, value); // see osc.cpp
	C.spectrum.frqadj[ channel ] = frqadj;
    *C.sds_spectrum_vec[ oscid ] = C.spectrum;  // the active GUI spectrum is updated
    C.Eventlog_p->add( C.SDS_ID, UPDATESPECTRUM_KEY );	// the synthesizer is notified
    C.ui->lcd_spectrumDisplay->display( frqadj );
    C.ui->lbl_spectrumDisplay->setText( "[Hz]");
};

auto spec_vol_slider = []( Spectrum_Dialog_class& C, int channel, int value )
{
    char oscid = C.sds_p->Spectrum_type;
	C.spectrum = *C.sds_spectrum_vec[ oscid ];
	C.spectrum.volidx[ channel ] = value;
    C.Sum( C.spectrum ); // fill vol
    *C.sds_spectrum_vec[ oscid ] = C.spectrum;  // the active GUI spectrum is updated
    C.Eventlog_p->add( C.SDS_ID, UPDATESPECTRUM_KEY );	// the synthesizer is notified
    C.ui->lcd_spectrumDisplay->display( C.spectrum.vol[ channel ] * 100 );
    C.ui->lbl_spectrumDisplay->setText( "Amp [%]");
};

void Spectrum_Dialog_class::fS1( int value )
{
	spec_frq_slider( *this, 1, value );
}
void Spectrum_Dialog_class::vS2( int value )
{
	spec_vol_slider( *this, 1, value );
}
void Spectrum_Dialog_class::fS3( int value )
{
	spec_frq_slider( *this, 2, value );
}
void Spectrum_Dialog_class::vS4( int value )
{
	spec_vol_slider( *this, 2, value );
}
void Spectrum_Dialog_class::fS5( int value )
{
	spec_frq_slider( *this, 3, value );
}
void Spectrum_Dialog_class::vS6( int value )
{
	spec_vol_slider( *this, 3, value );
}
void Spectrum_Dialog_class::fS7( int value )
{
	spec_frq_slider( *this, 4, value );
}
void Spectrum_Dialog_class::vS8( int value )
{
	spec_vol_slider( *this, 4, value );
}

auto ScrollBar_Wafeform = [  ]( Spectrum_Dialog_class* S, uint id, int value  )
{
	S->spectrum = *S->sds_spectrum_vec[ S->OscId ];
	S->spectrum.wfid[id] = value;
	*S->sds_spectrum_vec[ S->OscId ] = S->spectrum;
	S->Eventlog_p->add( S->SDS_ID, UPDATESPECTRUM_KEY );

	QString QStr = S->Waveform_vec[ value ];
	S->ui->lbl_waveform->setText( QStr );
};

void Spectrum_Dialog_class::adsrwf( int value )
{
	uint8_t wfid = value % adsrwf_range.max;
//	ui->sb_spwf1->setValue( wfid );
	ui->lbl_adsrwf->setText( Qstring( adsrwf_str_vec[wfid]) );
	Sds->Set( sds_adsr_vec[ OscId ]->spec.wfid[0], wfid );
	Eventlog_p->add( SDS_ID, ADSR_KEY );
	coutf << "oscid " << OscId << " wfid " << wfid << endl;
}
void Spectrum_Dialog_class::sb_wf1(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf1->setValue( wfid );
	ScrollBar_Wafeform( this, 1, value );
}
void Spectrum_Dialog_class::sb_wf2(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf2->setValue( wfid );
	ScrollBar_Wafeform( this,2, value );
}
void Spectrum_Dialog_class::sb_wf3(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf3->setValue( wfid );
	ScrollBar_Wafeform( this,3, value );
}
void Spectrum_Dialog_class::sb_wf4(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf4->setValue( wfid );
	ScrollBar_Wafeform( this,4, value );
}

void Spectrum_Dialog_class::save()
{
	Eventlog_p->add( SDS_ID, SAVEINSTRUMENTKEY );
}

void Spectrum_Dialog_class::SetLabelWaveform( const QString& wf )
{
	ui->lbl_waveform->setText( wf );
}
void Spectrum_Dialog_class::SetLabelInstrument( const QString& instr )
{
	ui->lbl_instrument->setText( instr );
}
void Spectrum_Dialog_class::SetSds( Interface_class* Sds, int8_t sdsid )
{
	this->Sds 		= Sds;
	this->sds_p 	= Sds->addr;
	this->SDS_ID	= sdsid;
    Comment( INFO," File_Dialog set to SDS Id: " + to_string( (int) sdsid ));
    sds_spectrum_vec 		= { &sds_p->VCO_spectrum,
								&sds_p->FMO_spectrum,
								&sds_p->OSC_spectrum};
    sds_adsr_vec			= { &sds_p->VCO_adsr,
    							&sds_p->FMO_adsr,
								&sds_p->OSC_adsr };
    OscId					= sds_p->Spectrum_type;
    switch ( OscId )
    {
		case osc_struct::OSCID	: ui->rb_spec_main->click(); 	break;
		case osc_struct::VCOID  : ui->rb_spec_vco->click(); 	break;
		case osc_struct::FMOID  : ui->rb_spec_fmo->click(); 	break;
		default 				: 								break;
	}
    select_spec( OscId );

}
void Spectrum_Dialog_class::Setup_widgets(  spectrum_t spectrum)
{
    ui->fS_1->setValue( spectrum.frqidx[1] );
    ui->vS_2->setValue( spectrum.volidx[1] );
    ui->fS_3->setValue( spectrum.frqidx[2] );
    ui->vS_4->setValue( spectrum.volidx[2] );
    ui->fS_5->setValue( spectrum.frqidx[3] );
    ui->vS_6->setValue( spectrum.volidx[3] );
    ui->fS_7->setValue( spectrum.frqidx[4] );
    ui->vS_8->setValue( spectrum.volidx[4] );

    ui->sb_spwf1->setValue( spectrum.wfid[1] );
    ui->sb_spwf2->setValue( spectrum.wfid[2] );
    ui->sb_spwf3->setValue( spectrum.wfid[3] );
    ui->sb_spwf4->setValue( spectrum.wfid[4] );

    ui->hs_attack->setValue( sds_p->FMO_adsr.attack );
    ui->hs_decay ->setValue( sds_p->FMO_adsr.decay  );

    ui->lbl_instrument->setText( Qstring( instrument ) );

    ui->lbl_waveform->setText( Waveform_vec[ spectrum.wfid[1]] );
}
