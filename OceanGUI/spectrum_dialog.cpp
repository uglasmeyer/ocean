#include <Spectrum_dialog_class.h>
#include <Common.h>
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
    connect( ui->cb_bps	, SIGNAL(activated(int) )	,this, SLOT( cb_bps_slot(int) ));

    connect( ui->rb_spec_fmo,  SIGNAL( clicked()), this, SLOT( select_spec_fmo() ) );
    connect( ui->rb_spec_vco,  SIGNAL( clicked()), this, SLOT( select_spec_vco() ) );
    connect( ui->rb_spec_main, SIGNAL( clicked()), this, SLOT( select_spec_main() ));

    connect( ui->pB_save_spectrum, 	SIGNAL(clicked()), this, SLOT(save( )));
    connect( ui->rb_reset, 			SIGNAL(clicked()), this, SLOT(reset( )));

    connect( ui->hs_attack	, SIGNAL(valueChanged(int)), 	this, SLOT(attack(int) ));
    connect( ui->hs_decay	, SIGNAL(valueChanged(int)), 	this, SLOT(decay(int) ));

    connect( ui->cb_adsr	, SIGNAL( clicked(bool)), 	this, SLOT( adsr_slot(bool) ));

    this->Sds   		    = gui;
    sds_p 					= Sds->addr;
    Eventlog_p				= _log;

    set_spectrum_vec( sds_p );

    QStringList				Qbps_str_lst		= Qstringlist( bps_struct().Bps_lst );
    ui->cb_bps->addItems	( Qbps_str_lst );

    this->instrument   		= this->Sds->Read_str( INSTRUMENTSTR_KEY );

    this->SetSds( Sds, 0 );

    select_spec( sds_p->Spectrum_type );
}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
//   if( ui) delete(ui);
}
void Spectrum_Dialog_class::cb_bps_slot( int bps_id )
{
	uint8_t bps = bps_struct().Bps_vec[bps_id];
    Sds->Set( sds_adsr_vec[ OscId ]->bps, bps  );

    Eventlog_p->add( SDS_ID, ADSR_KEY );

}
void Spectrum_Dialog_class::set_spectrum_vec( interface_t* sds )
{
    sds_spectrum_vec 		= { &sds->VCO_spectrum,
								&sds->FMO_spectrum,
								&sds->OSC_spectrum};
    sds_adsr_vec			= { &sds->VCO_adsr,
    							&sds->FMO_adsr,
								&sds->OSC_adsr };
}
void Spectrum_Dialog_class::set_waveform_vec( vector<string> wf_vec )
{
	size_t max = wf_vec.size() - 1;
    ui->sb_spwf1->setMaximum( max );
    ui->sb_spwf2->setMaximum( max );
    ui->sb_spwf3->setMaximum( max );
    ui->sb_spwf4->setMaximum( max );
    Waveform_vec= Vstringvector( wf_vec );
}

void Spectrum_Dialog_class::set_spectrum_view()
{
	auto set_wavedisplay = [ this ]( Id_t roleid  )
	{
	    Sds->Set( sds_p->WD_status.oscId	, (uint8_t) OscId );
	    Sds->Set( sds_p->WD_status.roleId	, (uint8_t) roleid );
		Eventlog_p->add( SDS_ID, SETWAVEDISPLAYKEY );

	};

	if ( ADSR_flag )
	{
		set_wavedisplay( osc_struct::ADSRID );
		spectrum			= sds_adsr_vec[ OscId ]->spec;
		spectrum.adsr		= true;
		set_waveform_vec( adsrwf_str_vec );
	}
	else
	{
		set_wavedisplay( osc_struct::INSTRID );
		spectrum			= *sds_spectrum_vec[ OscId ];
		spectrum.adsr		= false;
		set_waveform_vec( waveform_str_vec );
	}
	Sds->Set( sds_p->Spectrum_type, (uint8_t) OscId );

	ui->lbl_waveform->setText( Waveform_vec[ WfSlot ] );
}
void Spectrum_Dialog_class::set_spectrum_data()
{
	if ( ADSR_flag )
	{
		sds_adsr_vec[ OscId ]->spec = spectrum;
		Eventlog_p->add( SDS_ID, ADSR_KEY );
	}
	else
	{
		*sds_spectrum_vec[ OscId ]	= spectrum;
		Eventlog_p->add( SDS_ID, UPDATESPECTRUM_KEY );
	}
}
void Spectrum_Dialog_class::select_spec( char oscid )
{
	OscId				= oscid;
	set_spectrum_view	();

	Setup_widgets		( spectrum );
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
	ADSR_flag 	= flag;
	select_spec	( OscId );
	ui->cb_adsr->setChecked( flag );
}
void Spectrum_Dialog_class::adsr_slot( bool flag )
{
	Set_adsr_flag( flag );
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
	spectrum.volidx	= default_spectrum.volidx;
	spectrum.frqidx = default_spectrum.frqidx;
	Setup_widgets( spectrum );
	ui->rb_reset->setChecked( false );
}

void Spectrum_Dialog_class::Update_instrument()
{
    string  newinstrument      = this->Sds->Read_str( INSTRUMENTSTR_KEY );

    if ( newinstrument.compare( instrument ) != 0 )
    {
        instrument 	= newinstrument;
        set_spectrum_view();
        return;
    }
}

void Spectrum_Dialog_class::spec_frq_slider( int channel, int value )
{
	spectrum.frqidx[ channel ] = check_range(  frqarr_range, value, "spec_frq_slider");
	float frqadj =  Spectrum.Frqadj(channel, value); // see osc.cpp
	spectrum.frqadj[ channel ] = frqadj;
	set_spectrum_data();
    ui->lcd_spectrumDisplay->display( frqadj );
    ui->lbl_spectrumDisplay->setText( "[Hz]");
};

void Spectrum_Dialog_class::spec_vol_slider( int channel, int value )
{
	spectrum.volidx[ channel ] = value;
    Sum( spectrum ); // fill vol
    set_spectrum_data();
    ui->lcd_spectrumDisplay->display( spectrum.vol[ channel ] * 100 );
    ui->lbl_spectrumDisplay->setText( "Amp [%]");
};

void Spectrum_Dialog_class::fS1( int value )
{
	spec_frq_slider( 1, value );
}
void Spectrum_Dialog_class::vS2( int value )
{
	spec_vol_slider( 1, value );
}
void Spectrum_Dialog_class::fS3( int value )
{
	spec_frq_slider( 2, value );
}
void Spectrum_Dialog_class::vS4( int value )
{
	spec_vol_slider( 2, value );
}
void Spectrum_Dialog_class::fS5( int value )
{
	spec_frq_slider( 3, value );
}
void Spectrum_Dialog_class::vS6( int value )
{
	spec_vol_slider( 3, value );
}
void Spectrum_Dialog_class::fS7( int value )
{
	spec_frq_slider( 4, value );
}
void Spectrum_Dialog_class::vS8( int value )
{
	spec_vol_slider(  4, value );
}

void Spectrum_Dialog_class::scrollBar_wafeform( uint id, int value  )
{
	spectrum.wfid[id] = value;
	WfSlot = id;
	set_spectrum_data();
	ui->lbl_waveform->setText( Waveform_vec[ WfSlot ] );

};


void Spectrum_Dialog_class::sb_wf1(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf1->setValue( wfid );
	scrollBar_wafeform( 1, value );
}
void Spectrum_Dialog_class::sb_wf2(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf2->setValue( wfid );
	scrollBar_wafeform( 2, value );
}
void Spectrum_Dialog_class::sb_wf3(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf3->setValue( wfid );
	scrollBar_wafeform( 3, value );
}
void Spectrum_Dialog_class::sb_wf4(int value )
{
	uint wfid = value % waveform_vec_len;
	ui->sb_spwf4->setValue( wfid );
	scrollBar_wafeform( 4, value );
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
    set_spectrum_vec( sds_p );

    select_spec( sds_p->Spectrum_type );

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

    ui->hs_attack->setValue( sds_adsr_vec[OscId]->attack );
    ui->hs_decay ->setValue( sds_adsr_vec[OscId]->decay  );

    ui->lbl_instrument->setText( Qstring( instrument ) );
    QString QStr	{ int2char( sds_adsr_vec[ OscId ]->bps ) };
    ui->cb_bps->setCurrentText( QStr );
}
