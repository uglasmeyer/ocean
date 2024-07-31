#include "spectrum_dialog_class.h"
#include "qtimer.h"
#include "ui_spectrum_dialog_class.h"



Spectrum_Dialog_class::Spectrum_Dialog_class(QWidget *parent,
                                             GUI_interface_class *gui) :
    Logfacility_class("Spectrum"),
	Spectrum_base(),
	QDialog(parent),

    ui(new Ui::Spectrum_Dialog_class)
{

    ui->setupUi(this);
    connect( ui->vS_1, SIGNAL(valueChanged(int)), this, SLOT(vS1( int )));
    connect( ui->vS_2, SIGNAL(valueChanged(int)), this, SLOT(vS2( int )));
    connect( ui->vS_3, SIGNAL(valueChanged(int)), this, SLOT(vS3( int )));
    connect( ui->vS_4, SIGNAL(valueChanged(int)), this, SLOT(vS4( int )));
    connect( ui->vS_5, SIGNAL(valueChanged(int)), this, SLOT(vS5( int )));
    connect( ui->vS_6, SIGNAL(valueChanged(int)), this, SLOT(vS6( int )));
    connect( ui->vS_7, SIGNAL(valueChanged(int)), this, SLOT(vS7( int )));
    connect( ui->vS_8, SIGNAL(valueChanged(int)), this, SLOT(vS8( int )));

    connect( ui->rb_spec_fmo,  SIGNAL( clicked()), this, SLOT( select_spec_fmo() ) );
    connect( ui->rb_spec_vco,  SIGNAL( clicked()), this, SLOT( select_spec_vco() ) );
    connect( ui->rb_spec_main, SIGNAL( clicked()), this, SLOT( select_spec_main() ));

    connect( ui->pB_save_spectrum, SIGNAL(clicked()), this, SLOT(save( )));

    this->GUI   		    = gui;
    ifd 					= GUI->addr;
    ifd_spectrum_vec 		= { &ifd->MAIN_spectrum,
    							&ifd->VCO_spectrum,
								&ifd->FMO_spectrum};
    osc_type				= ifd->Spectrum_type;

    this->instrument   		= this->GUI->read_str('i');
//    Spectrum.Set_Spec_Name( this->instrument );
    size_t wfid            	= this->ifd->MAIN_spectrum.id;
    cout << "wfid: " << dec << (int) wfid << endl;
    assert( ( wfid >= 0 ) and ( wfid < Spectrum.Get_waveform_vec().size() ) );
    this->spectrum 			= ifd->MAIN_spectrum;

    setup_Widgets(  this->spectrum );

    if ( ifd->Spectrum_type == MAINID ) ui->rb_spec_main->click();
    if ( ifd->Spectrum_type == VCOID  ) ui->rb_spec_vco->click(  );
    if ( ifd->Spectrum_type == FMOID  ) ui->rb_spec_fmo->click( );

    QTimer* status_timer = new QTimer( this );
    connect(status_timer, &QTimer::timeout, this, &Spectrum_Dialog_class::Update_spectrum);
    status_timer->start(200);

}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
    delete ui;
}

auto select_spec = []( Spectrum_Dialog_class* C, spec_struct_t& spec, char id )
{
	C->ifd->Spectrum_type = id;
	C->spectrum			= spec;
	C->setup_Widgets( C->spectrum );
};

void Spectrum_Dialog_class::select_spec_fmo()
{
	select_spec( this, ifd->FMO_spectrum, FMOID );
}
void Spectrum_Dialog_class::select_spec_vco()
{
	select_spec( this, ifd->VCO_spectrum, VCOID );
}
void Spectrum_Dialog_class::select_spec_main()
{
	select_spec( this, ifd->MAIN_spectrum, MAINID );
}

void Spectrum_Dialog_class::Update_spectrum()
{

    string  newinstrument      = this->GUI->read_str('i');

    if ( newinstrument.compare( instrument ) != 0 )
    {
        instrument = newinstrument;
        this->spectrum = *ifd_spectrum_vec[ ifd->Spectrum_type ];
        setup_Widgets( spectrum );
        return;
    }
}

auto spec_slider = []( Spectrum_Dialog_class& C, int channel, int value )
{
    char id = C.ifd->Spectrum_type;
	C.spectrum = *C.ifd_spectrum_vec[ id ];
	C.spectrum.dta[ channel ] = value;
    C.Spectrum_base::Sum( C.spectrum );
    *C.ifd_spectrum_vec[ id ] = C.spectrum;  // the active GUI spectrum is updated
    C.ifd->KEY = UPDATESPECTRUM_KEY;	// the synthesizer is notified
};

void Spectrum_Dialog_class::vS1( int value )
{
	spec_slider( *this, 0, value );
}
void Spectrum_Dialog_class::vS2( int value )
{
	spec_slider( *this, 1, value );
}
void Spectrum_Dialog_class::vS3( int value )
{
	spec_slider( *this, 2, value );
}
void Spectrum_Dialog_class::vS4( int value )
{
	spec_slider( *this, 3, value );
}
void Spectrum_Dialog_class::vS5( int value )
{
	spec_slider( *this, 4, value );
}
void Spectrum_Dialog_class::vS6( int value )
{
	spec_slider( *this, 5, value );
}
void Spectrum_Dialog_class::vS7( int value )
{
	spec_slider( *this, 6, value );
}
void Spectrum_Dialog_class::vS8( int value )
{
	spec_slider( *this, 7, value );
}

void Spectrum_Dialog_class::save()
{
	ifd->KEY = SAVEINSTRUMENTKEY;
}

void Spectrum_Dialog_class::setup_Widgets(  spec_struct_t spectrum)
{
    ui->vS_1->setValue( spectrum.dta[0] );
    ui->vS_2->setValue( spectrum.dta[1] );
    ui->vS_3->setValue( spectrum.dta[2] );
    ui->vS_4->setValue( spectrum.dta[3] );
    ui->vS_5->setValue( spectrum.dta[4] );
    ui->vS_6->setValue( spectrum.dta[5] );
    ui->vS_7->setValue( spectrum.dta[6] );
    ui->vS_8->setValue( spectrum.dta[7] );

    QString Qinstrument = QString::fromStdString( instrument );
    ui->lbl_instrument->setText( Qinstrument );

    QString Qwaveform = QString::fromStdString( Get_waveform_str( spectrum.id ));
    ui->lbl_waveform->setText( Qwaveform );
}
