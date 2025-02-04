#include "Spectrum_dialog_class.h"
#include "qtimer.h"
#include "ui_spectrum_dialog_class.h"


Ui::Spectrum_Dialog_class	UI_obj;

Spectrum_Dialog_class::Spectrum_Dialog_class(QWidget *parent,
                                             Interface_class *gui) :
    Logfacility_class("Spectrum"),
	Spectrum_base(),
	QDialog(parent)
//    ui(new Ui::Spectrum_Dialog_class )
{
	ui = &UI_obj;
    ui->setupUi(this);
    connect( ui->fS_1, SIGNAL(valueChanged(int)), this, SLOT(fS1( int )));
    connect( ui->vS_2, SIGNAL(valueChanged(int)), this, SLOT(vS2( int )));
    connect( ui->fS_3, SIGNAL(valueChanged(int)), this, SLOT(fS3( int )));
    connect( ui->vS_4, SIGNAL(valueChanged(int)), this, SLOT(vS4( int )));
    connect( ui->fS_5, SIGNAL(valueChanged(int)), this, SLOT(fS5( int )));
    connect( ui->vS_6, SIGNAL(valueChanged(int)), this, SLOT(vS6( int )));
    connect( ui->fS_7, SIGNAL(valueChanged(int)), this, SLOT(fS7( int )));
    connect( ui->vS_8, SIGNAL(valueChanged(int)), this, SLOT(vS8( int )));

    connect( ui->rb_spec_fmo,  SIGNAL( clicked()), this, SLOT( select_spec_fmo() ) );
    connect( ui->rb_spec_vco,  SIGNAL( clicked()), this, SLOT( select_spec_vco() ) );
    connect( ui->rb_spec_main, SIGNAL( clicked()), this, SLOT( select_spec_main() ));

    connect( ui->pB_save_spectrum, 	SIGNAL(clicked()), this, SLOT(save( )));
    connect( ui->rb_reset, 			SIGNAL(clicked()), this, SLOT(reset( )));

    this->Sds   		    = gui;
    ifd 					= Sds->addr;
    ifd_spectrum_vec 		= { &ifd->VCO_spectrum,
								&ifd->FMO_spectrum,
								&ifd->OSC_spectrum};


    this->instrument   		= this->Sds->Read_str( INSTRUMENTSTR_KEY );
    size_t wfid            	= this->ifd->OSC_spectrum.id;
    assert( ( wfid >= 0 ) and ( wfid < Spectrum.Get_waveform_vec().size() ) );
    this->spectrum 			= ifd->OSC_spectrum;

    setup_Widgets(  this->spectrum );
    switch ( ifd->Spectrum_type )
    {
		case osc_struct::OSCID	: ui->rb_spec_main->click(); 	break;
		case osc_struct::VCOID  : ui->rb_spec_vco->click(); 	break;
		case osc_struct::FMOID  : ui->rb_spec_fmo->click(); 	break;
		default 				: 								break;
	}

    connect(status_timer, &QTimer::timeout, this, &Spectrum_Dialog_class::Update_spectrum);
    status_timer->start(200);

}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
//   if( ui) delete(ui);
}

auto select_spec = []( Spectrum_Dialog_class* C, Spectrum_base::spectrum_t& spec, char id )
{
	C->ifd->Spectrum_type = id;
	C->spectrum			= spec;
	C->setup_Widgets( spec );
};

void Spectrum_Dialog_class::select_spec_fmo()
{
	select_spec( this, ifd->FMO_spectrum, osc_struct::FMOID );
}
void Spectrum_Dialog_class::select_spec_vco()
{
	select_spec( this, ifd->VCO_spectrum, osc_struct::VCOID );
}
void Spectrum_Dialog_class::select_spec_main()
{
	select_spec( this, ifd->OSC_spectrum, osc_struct::OSCID );
}

void Spectrum_Dialog_class::reset()
{
	spectrum.frq = spec_struct().frq;
	spectrum.vol = spec_struct().vol;
	spectrum.phi = spec_struct().phi;
	setup_Widgets( spectrum );
	ui->rb_reset->setChecked( false );
}

void Spectrum_Dialog_class::Update_spectrum()
{

    string  newinstrument      = this->Sds->Read_str( INSTRUMENTSTR_KEY );

    if ( newinstrument.compare( instrument ) != 0 )
    {
        instrument = newinstrument;
        this->spectrum = *ifd_spectrum_vec[ ifd->Spectrum_type ];
        setup_Widgets( spectrum );
        return;
    }
}

auto specFreq = []( spectrum_t spec, int channel )
{
	return ( spec.base * (1+channel) * ( 1.0 + spec.frq[ channel ]) );
};

auto value2frq = []( auto value )
{
	return (  (spec_dta_t) value - 50.0 ) / 100.0;
};
auto frq2value = []( auto frq )
{
	return rint( 100+frq*100 );
};

auto spec_frq_slider = []( Spectrum_Dialog_class& C, int channel, int value )
{
    char id = C.ifd->Spectrum_type;
	C.spectrum = *C.ifd_spectrum_vec[ id ];
	C.spectrum.frq[ channel ] = value2frq( value );
    *C.ifd_spectrum_vec[ id ] = C.spectrum;  // the active GUI spectrum is updated
    C.ifd->KEY = UPDATESPECTRUM_KEY;	// the synthesizer is notified
    C.ui->lcd_spectrumDisplay->display( specFreq( C.spectrum, channel ));
    C.ui->lbl_spectrumDisplay->setText( "[Hz]");
};

auto spec_vol_slider = []( Spectrum_Dialog_class& C, int channel, int value )
{
    char id = C.ifd->Spectrum_type;
	C.spectrum = *C.ifd_spectrum_vec[ id ];
	C.spectrum.vol[ channel ] = (spec_dta_t)  value / 100.0;
    C.Spectrum_base::Sum( C.spectrum );
    *C.ifd_spectrum_vec[ id ] = C.spectrum;  // the active GUI spectrum is updated
    C.ifd->KEY = UPDATESPECTRUM_KEY;	// emit synthesizer event
    C.ui->lcd_spectrumDisplay->display(value);
    C.ui->lbl_spectrumDisplay->setText( "Aml [%]");
};

void Spectrum_Dialog_class::fS1( int value )
{
	spec_frq_slider( *this, 0, value );
}
void Spectrum_Dialog_class::vS2( int value )
{
	spec_vol_slider( *this, 0, value );
}
void Spectrum_Dialog_class::fS3( int value )
{
	spec_frq_slider( *this, 1, value );
}
void Spectrum_Dialog_class::vS4( int value )
{
	spec_vol_slider( *this, 1, value );
}
void Spectrum_Dialog_class::fS5( int value )
{
	spec_frq_slider( *this, 2, value );
}
void Spectrum_Dialog_class::vS6( int value )
{
	spec_vol_slider( *this, 2, value );
}
void Spectrum_Dialog_class::fS7( int value )
{
	spec_frq_slider( *this, 3, value );
}
void Spectrum_Dialog_class::vS8( int value )
{
	spec_vol_slider( *this, 3, value );
}

void Spectrum_Dialog_class::save()
{
	ifd->KEY = SAVEINSTRUMENTKEY;
}

void Spectrum_Dialog_class::SetLabelWaveform( const QString& wf )
{
	ui->lbl_waveform->setText( wf );
}
void Spectrum_Dialog_class::SetLabelInstrument( const QString& instr )
{
	ui->lbl_instrument->setText( instr );
}
void Spectrum_Dialog_class::setup_Widgets(  Spectrum_base::spectrum_t spectrum)
{
    ui->fS_1->setValue( frq2value( spectrum.frq[0] ) );
    ui->vS_2->setValue( rint(spectrum.vol[0]*100) );
    ui->fS_3->setValue( frq2value( spectrum.frq[1] ) );
    ui->vS_4->setValue( rint(spectrum.vol[1]*100) );
    ui->fS_5->setValue( frq2value( spectrum.frq[2] ) );
    ui->vS_6->setValue( rint(spectrum.vol[2]*100) );
    ui->fS_7->setValue( frq2value( spectrum.frq[3] ) );
    ui->vS_8->setValue( rint(spectrum.vol[3]*100));

    QString Qinstrument = QString::fromStdString( instrument );
    ui->lbl_instrument->setText( Qinstrument );

    QString Qwaveform = QString::fromStdString( Get_waveform_str( spectrum.id ));
    ui->lbl_waveform->setText( Qwaveform );
}
