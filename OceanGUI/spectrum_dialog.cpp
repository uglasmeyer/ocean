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
 * keyboard_dialog.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <include/Common.h>
#include <include/Spectrum_dialog.h>

Spectrum_Dialog_class::Spectrum_Dialog_class(QWidget *parent,
                                             Interface_class* 	_Sds,
											 EventLog_class*	_log) :
    Logfacility_class("Spectrum"),
	Spectrum_class(),
	QDialog(parent),
	ui(new Ui::Spectrum_Dialog_class)
{
	className 			= Logfacility_class::className;
    Waveform_vec 		= Vstringvector( Get_waveform_vec() );
    waveform_vec_len 	= Waveform_vec.size();

    ui->setupUi(this);
    ui->rb_spec_main->setChecked(true);
    QStringList				Qbps_str_lst		= Qstringlist( bps_struct().Bps_lst );
    ui->cb_bps->addItems	( Qbps_str_lst );

    connect( ui->fS_1, SIGNAL(valueChanged(int)), this, SLOT(fS1( int )));
    connect( ui->vS_2, SIGNAL(valueChanged(int)), this, SLOT(vS2( int )));
    connect( ui->fS_3, SIGNAL(valueChanged(int)), this, SLOT(fS3( int )));
    connect( ui->vS_4, SIGNAL(valueChanged(int)), this, SLOT(vS4( int )));
    connect( ui->fS_5, SIGNAL(valueChanged(int)), this, SLOT(fS5( int )));
    connect( ui->vS_6, SIGNAL(valueChanged(int)), this, SLOT(vS6( int )));
    connect( ui->fS_7, SIGNAL(valueChanged(int)), this, SLOT(fS7( int )));
    connect( ui->vS_8, SIGNAL(valueChanged(int)), this, SLOT(vS8( int )));
    fS_vec = { ui->fS_1, ui->fS_3, ui->fS_5, ui->fS_7 };
    vS_vec = { ui->vS_2, ui->vS_4, ui->vS_6, ui->vS_8 };
    for( QSlider* slider : fS_vec )
   	{
   		slider->setMaximum( harmonic_range.max );
   		slider->setMinimum( harmonic_range.min );
   	}
    connect( ui->sb_spwf1, SIGNAL(valueChanged(int)), this, SLOT( sb_wf1(int) ));
    connect( ui->sb_spwf2, SIGNAL(valueChanged(int)), this, SLOT( sb_wf2(int) ));
    connect( ui->sb_spwf3, SIGNAL(valueChanged(int)), this, SLOT( sb_wf3(int) ));
    connect( ui->sb_spwf4, SIGNAL(valueChanged(int)), this, SLOT( sb_wf4(int) ));
    sb_vec = { nullptr, ui->sb_spwf1, ui->sb_spwf2, ui->sb_spwf3, ui->sb_spwf4 };

    connect( ui->cb_bps	, SIGNAL(activated(int) )	,this, SLOT( cb_bps_slot(int) ));

    connect( ui->rb_spec_fmo,  SIGNAL( clicked()), this, SLOT( select_oscid_fmo() ) );
    connect( ui->rb_spec_vco,  SIGNAL( clicked()), this, SLOT( select_oscid_vco() ) );
    connect( ui->rb_spec_main, SIGNAL( clicked()), this, SLOT( select_oscid_main() ));
    connect( ui->rb_reset, 		SIGNAL(clicked()), this, SLOT(reset( )));
    rb_vec = { ui->rb_spec_vco, ui->rb_spec_fmo, ui->rb_spec_main };

    connect( ui->pB_save_spectrum, 	SIGNAL(clicked()), this, SLOT(save( )));

    connect( ui->hs_attack	, SIGNAL(valueChanged(int)), 	this, SLOT(attack(int) ));
    connect( ui->hs_decay	, SIGNAL(valueChanged(int)), 	this, SLOT(decay(int) ));

    connect( ui->cb_adsr	, SIGNAL( clicked(bool)), 	this, SLOT( adsr_slot(bool) ));

    Eventlog_p				= _log;
    this->SetSds( _Sds );

}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
	DESTRUCTOR( className );
}
void Spectrum_Dialog_class::cb_bps_slot( int bps_id )
{
	uint8_t bps = bps_struct().Bps_vec[bps_id];
    Sds->Set( sds_p->adsr_arr[ OscId ].bps, bps  );

    Eventlog_p->add( SDS_ID, ADSR_KEY );

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
	auto set_wavedisplay = [ this ]( RoleId_e roleid  )
	{
	    Sds->Set( sds_p->WD_state.oscId	, OscId );
	    Sds->Set( sds_p->WD_state.roleId	, roleid );
		Eventlog_p->add( SDS_ID, SETWAVEDISPLAYKEY );
	};

	if ( ADSR_flag )
	{
		set_wavedisplay		( ADSRROLE );
		spectrum			= sds_p->adsr_arr[ OscId ].spec;
		spectrum.adsr		= true;
		set_waveform_vec	( adsrwf_str_vec );
		this->setWindowTitle( "ADSR" );
	}
	else
	{
		set_wavedisplay		( INSTRROLE );
		spectrum			= sds_p->spectrum_arr[ OscId ];
		spectrum.adsr		= false;
		set_waveform_vec	( waveform_str_vec );
		this->setWindowTitle( "Spectrum" );
	}
}
void Spectrum_Dialog_class::save_spectrum_data()
{
	if ( ADSR_flag )
	{
		sds_p->adsr_arr[ OscId ].spec 	= spectrum;
		Eventlog_p->add( SDS_ID, ADSR_KEY );
	}
	else
	{
		// persist the setting from the main window control of spectrum
		sds_p->spectrum_arr[ OscId ]	= spectrum;
		Eventlog_p->add( SDS_ID, UPDATESPECTRUM_KEY );
	}
}
void Spectrum_Dialog_class::select_oscid( OSCID_e oscid )
{
	OscId				= oscid;
	Sds->Set			( sds_p->SpectrumTypeId, oscid );

    Channel 			= 1;
	set_spectrum_view	();

	Setup_widgets		();
};

void Spectrum_Dialog_class::select_oscid_fmo()
{
	select_oscid( FMOID );
}
void Spectrum_Dialog_class::select_oscid_vco()
{
	select_oscid( VCOID );
}
void Spectrum_Dialog_class::select_oscid_main()
{
	select_oscid( OSCID );
}
void Spectrum_Dialog_class::Set_adsr_flag( bool flag )
{
	ADSR_flag 	= flag;
	select_oscid( OscId );
	ui->cb_adsr->setChecked( flag );
}
void Spectrum_Dialog_class::adsr_slot( bool flag )
{
	Set_adsr_flag( flag );
}
void Spectrum_Dialog_class::attack(int value )
{
	Sds->Set( sds_p->adsr_arr[ OscId ].attack, (uint8_t) value );
	Eventlog_p->add( SDS_ID, ADSR_KEY );
}
void Spectrum_Dialog_class::decay (int value )
{
	Sds->Set( sds_p->adsr_arr[ OscId ].decay, (uint8_t) value );
	Eventlog_p->add( SDS_ID, ADSR_KEY );
}

void Spectrum_Dialog_class::reset()
{
	if ( ADSR_flag )
	{
		spectrum		= default_adsr_spec;
		spectrum.osc	= OscId;
	}
	else
	{
		spectrum			= default_spectrum;
		spectrum.osc		= OscId;
		// in case there were any change in between;
		spectrum.vol[0]		= sds_p->spectrum_arr[OscId].vol[0];
		spectrum.frqadj[0]	= sds_p->spectrum_arr[OscId].frqadj[0];
		spectrum.frqidx[0]	= sds_p->spectrum_arr[OscId].frqidx[0];
		spectrum.volidx[0]	= sds_p->spectrum_arr[OscId].volidx[0];
		spectrum.wfid[0]	= sds_p->spectrum_arr[OscId].wfid[0];
	}
	save_spectrum_data	();
	Setup_widgets		();
	rb_vec[OscId]->setChecked( true ); // unset rb_reset
}

void Spectrum_Dialog_class::spec_frq_slider( int channel, int value )
{
	//modify the harmonics of the base frequency
	uint8_t idx					= check_range(  harmonic_range, (uint8_t)value, "spec_frq_slider");
    Channel 					= channel;
	spectrum.frqidx[ channel ] 	= idx;
	float frqadj 				= Frqadj(channel, idx ); // see osc.cpp
	spectrum.frqadj[ channel ] 	= frqadj;
    ui->lcd_spectrumDisplay->display( frqadj );
    ui->lbl_spectrumDisplay->setText( "[Hz]");
	save_spectrum_data();
};

void Spectrum_Dialog_class::spec_vol_slider( int channel, int value )
{
    Channel 					= channel;
	spectrum.volidx[ channel ] 	= value;
    Sum( spectrum ); // fill vol
    save_spectrum_data();
    ui->lcd_spectrumDisplay->display( spectrum.volidx[ channel ] );
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

void Spectrum_Dialog_class::waveform_spinbox( uint id, int value  )
{
	Channel 			= id;
	spectrum.wfid[id] 	= value;
	save_spectrum_data();
	SetLabelWaveform();
};


void Spectrum_Dialog_class::sb_wf1(int value )
{
	waveform_spinbox( 1, value );
}
void Spectrum_Dialog_class::sb_wf2(int value )
{
	waveform_spinbox( 2, value );
}
void Spectrum_Dialog_class::sb_wf3(int value )
{
	waveform_spinbox( 3, value );
}
void Spectrum_Dialog_class::sb_wf4(int value )
{
	waveform_spinbox( 4, value );
}

void Spectrum_Dialog_class::save()
{
	Eventlog_p->add( SDS_ID, SAVEINSTRUMENTKEY );
}

void Spectrum_Dialog_class::SetLabelWaveform( )
{
	int wfid = sb_vec[ Channel ]->value();
	ui->lbl_waveform->setText( Waveform_vec[ wfid ] );
}


void Spectrum_Dialog_class::SetInstrument()
{
    string instrument= this->Sds->Read_str( INSTRUMENTSTR_KEY );
	ui->lbl_instrument->setText( Qstring( instrument ) );
    select_oscid( sds_p->SpectrumTypeId );
    Setup_widgets();
}
void Spectrum_Dialog_class::SetSds( Interface_class* Sds )
{
	this->Sds 		= Sds;
	this->sds_p 	= Sds->addr;
	this->SDS_ID	= sds_p->SDS_Id;

    SetInstrument();
}

void Spectrum_Dialog_class::Setup_widgets()
{
	int i = 1;
	for( QSlider* sl : fS_vec )
	{
		sl->setValue( check_range( harmonic_range, spectrum.frqidx[i] ) );;
		i++;
	}

	i = 1;
	for( QSlider* sl : vS_vec )
	{
		sl->setValue( spectrum.volidx[i] );;
		i++;
	}

    ui->sb_spwf1->setValue( spectrum.wfid[1] );
    ui->sb_spwf2->setValue( spectrum.wfid[2] );
    ui->sb_spwf3->setValue( spectrum.wfid[3] );
    ui->sb_spwf4->setValue( spectrum.wfid[4] );

    ui->hs_attack->setValue( sds_p->adsr_arr[OscId].attack );
    ui->hs_decay ->setValue( sds_p->adsr_arr[OscId].decay  );

	SetLabelWaveform();
//	SetInstrument();
    QString QStr	{ int2char( sds_p->adsr_arr[ OscId ].bps ) };
    ui->cb_bps->setCurrentText( QStr );
}
