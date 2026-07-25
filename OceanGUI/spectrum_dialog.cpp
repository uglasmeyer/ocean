/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * spectrum_dialog.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */
// Ocean
#include <Oscwaveform.h>

// OceanGUI
#include <Spectrum_dialog.h>
#include <DataGraphicClass.h>

Spectrum_Dialog_class::Spectrum_Dialog_class(QWidget*			parent,
											 Dataworld_class* 	data ,
											 EventLog_class*	_log) :
    Logfacility_class	( "Spectrum_Dlg" ),
	Spectrum_class		(),
	Interface_base		( data ),
	QDialog				( parent ),
	ui					( new Ui::Spectrum_Dialog_class )
{
    QWaveform_vec 		= Vstringvector( Get_waveform_vec() );
    Eventlog_p			= _log;

    ui->setupUi			( this );

    QStringList			qresolution_lst = { "8", "16", "32", "64" };
    ui->cB_resolution->addItems( qresolution_lst );
//    ui->cB_resolution->setCurrentIndex( sds_p->resolutionIdx );

    ui->rb_spec_main->setChecked( true );
    QStringList			Qbps_str_lst		= Qstringlist( bps_struct().Bps_lst );
    ui->cb_bps->addItems( Qbps_str_lst );

    connect( ui->fS_1	, SIGNAL(valueChanged(int)), this, SLOT(fS1( int )));
    connect( ui->vS_2	, SIGNAL(valueChanged(int)), this, SLOT(vS2( int )));
    connect( ui->fS_3	, SIGNAL(valueChanged(int)), this, SLOT(fS3( int )));
    connect( ui->vS_4	, SIGNAL(valueChanged(int)), this, SLOT(vS4( int )));
    connect( ui->fS_5	, SIGNAL(valueChanged(int)), this, SLOT(fS5( int )));
    connect( ui->vS_6	, SIGNAL(valueChanged(int)), this, SLOT(vS6( int )));
    connect( ui->fS_7	, SIGNAL(valueChanged(int)), this, SLOT(fS7( int )));
    connect( ui->vS_8	, SIGNAL(valueChanged(int)), this, SLOT(vS8( int )));
    fS_vec = { nullptr, ui->fS_1, ui->fS_3, ui->fS_5, ui->fS_7 };
    vS_vec = { nullptr, ui->vS_2, ui->vS_4, ui->vS_6, ui->vS_8 };
    for( size_t channel = 1; channel < SPEC_CHANNELS; channel++ )
   	{
    	fS_vec[channel]->setMaximum( harmonic_range.max );
    	fS_vec[channel]->setMinimum( harmonic_range.min );
   	}
    connect( ui->sb_spwf1, SIGNAL(valueChanged(int)), this, SLOT( sb_wf1(int) ));
    connect( ui->sb_spwf2, SIGNAL(valueChanged(int)), this, SLOT( sb_wf2(int) ));
    connect( ui->sb_spwf3, SIGNAL(valueChanged(int)), this, SLOT( sb_wf3(int) ));
    connect( ui->sb_spwf4, SIGNAL(valueChanged(int)), this, SLOT( sb_wf4(int) ));
    sb_vec = { nullptr, ui->sb_spwf1, ui->sb_spwf2, ui->sb_spwf3, ui->sb_spwf4 };

    connect( ui->rb_spec_fmo	, SIGNAL( clicked()), this, SLOT( select_oscid_fmo() ) );
    connect( ui->rb_spec_vco	, SIGNAL( clicked()), this, SLOT( select_oscid_vco() ) );
    connect( ui->rb_spec_main	, SIGNAL( clicked()), this, SLOT( select_oscid_main() ));
    rb_vec = { ui->rb_spec_vco, ui->rb_spec_fmo, ui->rb_spec_main };

    connect( ui->pB_save_spectrum		, SIGNAL(clicked()),
    		this						, SLOT( save( )));
    connect( ui->hs_attack				, SIGNAL(valueChanged(int)),
    		this						, SLOT( attack(int) ));
    connect( ui->hs_decay				, SIGNAL(valueChanged(int)),
    		this						, SLOT( decay(int) ));
    connect( ui->cb_adsr				, SIGNAL( clicked(bool)),
    		this						, SLOT( adsr_slot(bool) ));
    connect( ui->pB_Fourier				, SIGNAL( clicked())		,
    		this						, SLOT( Fourier_mode() ));
    connect( ui->cB_resolution			, SIGNAL( activated(int) ),
    		this						, SLOT( resolution_slot( int ) ));
    connect( ui->cb_bps					, SIGNAL( activated(int) )	,
    		this						, SLOT( cb_bps_slot(int) ));
    connect( ui->rb_reset				, SIGNAL( clicked())	,
    		this						, SLOT( reset() ) );


    initDataDisplay();
}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
	DESTRUCTOR( className );
	delete ui->vS_4;
	delete ui->vS_2;
	delete ui->vS_6;
	delete ui->vS_8;
	delete AdsrDataDisplay;
	delete adsrScene;
	delete FourierDataDisplay;
	delete fourierScene;
	delete ui;
}

void Spectrum_Dialog_class::initDataDisplay()
{
	Data_t					scale 	= 1.0;
    datagraphics_param_t 	param 	{ scale, false, false };

         		adsrScene 			= new QGraphicsScene( this );
    			AdsrDataDisplay		= new DataGraphic_class( Sds->addr, ui->adsr_dataview, param ) ;
    ui->adsr_dataview->setScene		( adsrScene );
    adsrScene->addItem				( AdsrDataDisplay );

				fourierScene		= new QGraphicsScene( this );
				FourierDataDisplay	= new DataGraphic_class( Sds->addr, ui->fft_dataview, param ) ;
	ui->fft_dataview->setScene		( fourierScene );
    fourierScene->addItem			( FourierDataDisplay );

    UpdateDataDisplay				();
}

void Spectrum_Dialog_class::UpdateDataDisplay()
{
    AdsrDataDisplay->ReadData	( &sds_p->adsrdata[0], adsrdisplay_len );
    adsrScene->update			();
    FourierDataDisplay->ReadData( &sds_p->fft_data[0], fft_display_len );
    fourierScene->update		();
}
void Spectrum_Dialog_class::add_event( EVENTKEY_e key )
{
	Eventlog_p->add( SDS_ID, key );
}
void Spectrum_Dialog_class::Dialog( bool adsr )
{
	if( this->isVisible() )
	{
		this->hide();
	}
	else
	{
	    Setup_widgets();
		this->show();
	}
    Set_adsr_flag( adsr );
}
void Spectrum_Dialog_class::cb_bps_slot( int bps_id )
{
	uint8_t bps = bps_struct().Bps_vec[bps_id];
    Sds->Set( sds_p->adsr_arr[ OscId ].bps, bps  );

    add_event( ADSR_KEY );
}

void Spectrum_Dialog_class::set_waveform_vec( vector<string> wf_vec )
{
	size_t max = wf_vec.size() - 1;
    ui->sb_spwf1->setMaximum( max );
    ui->sb_spwf2->setMaximum( max );
    ui->sb_spwf3->setMaximum( max );
    ui->sb_spwf4->setMaximum( max );
    QWaveform_vec= Vstringvector( wf_vec );

}

void Spectrum_Dialog_class::set_spectrum_view()
{
	auto set_wavedisplay = [ this ]( bool mode  )
	{
	    Sds->Set( sds_p->WD_state.oscId	, OscId );
	    add_event( SETWAVEDISPLAYKEY );
	};

	if ( ADSR_flag )
	{
		set_wavedisplay		( true );
		spectrum			= sds_p->adsr_arr[ OscId ].spec;
		spectrum.adsr		= true;
		set_waveform_vec	( adsrwf_str_vec );
		this->setWindowTitle( "ADSR" );
	}
	else
	{
		set_wavedisplay		( false );
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
		add_event( ADSR_KEY );
	}
	else
	{
		// persist the setting from the main window control of spectrum
		sds_p->spectrum_arr[ OscId ]	= spectrum;
		add_event( UPDATESPECTRUM_KEY );
	}

}
void Spectrum_Dialog_class::select_oscid( OSCID_e oscid )
{
	OscId				= oscid;
	Sds->Set			( sds_p->SpectrumTypeId, oscid );

    Channel 			= 1;
    if( this->isVisible() )
    {
    	set_spectrum_view	();
    	Setup_widgets		();
    }
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
{// manage ADSR parameter or spectrum parameter
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
//	Sds->Set( sds_p->WD_state.adsrmode, true );
}
void Spectrum_Dialog_class::decay (int value )
{
	Sds->Set( sds_p->adsr_arr[ OscId ].decay, (uint8_t) value );
//	Sds->Set( sds_p->WD_state.adsrmode, true );
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
    Sum							( spectrum ); // fill vol
    save_spectrum_data			();
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
	save_spectrum_data	();
	SetLabelWaveform	();
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
	add_event( SAVEINSTRUMENTKEY );
}

void Spectrum_Dialog_class::resolution_slot( int idx )
{
	Sds->Set	( sds_p->resolutionIdx, uint8_t(idx) );
	Sds->Set	( sds_p->WD_state.fftmode, true );
	add_event	( ADSR_KEY );
}

void Spectrum_Dialog_class::Fourier_mode()
{
	Sds->Set	( sds_p->WD_state.fftmode, true );
	Sds->Set	( sds_p->spectrum_arr[OSCID].wfid[0], uint8_t(FOURIER) );
	add_event	( ADSR_KEY );
}

void Spectrum_Dialog_class::SetLabelWaveform( )
{
	int wfid = sb_vec[ Channel ]->value();
	ui->lbl_waveform->setText( QWaveform_vec[ wfid ] );
}

void Spectrum_Dialog_class::SetSds()
{
	Interface_base::Set_Sds	();
    SetInstrument			();
}

void Spectrum_Dialog_class::SetInstrument()
{
    string instrument			= this->Sds->Read_str( INSTRUMENTSTR );
	ui->lbl_instrument->setText	( Qstring( instrument ) );
    select_oscid				( sds_p->SpectrumTypeId );
    Setup_widgets				();
}

void Spectrum_Dialog_class::Setup_widgets()
{
	for( size_t channel = 1; channel < SPEC_CHANNELS; channel++ )
	{
		fS_vec[channel]->setValue( check_range( harmonic_range, spectrum.frqidx[channel] ) );;
		vS_vec[channel]->setValue( spectrum.volidx[channel] );
		sb_vec[channel]->setValue( spectrum.wfid[channel] );
	}

    ui->hs_attack->setValue		( sds_p->adsr_arr[OscId].attack );
    ui->hs_decay ->setValue		( sds_p->adsr_arr[OscId].decay  );

	SetLabelWaveform			();
    QString QStr				{ int2char( sds_p->adsr_arr[ OscId ].bps ) };
    ui->cb_bps->setCurrentText	( QStr );
    ui->cB_resolution->setCurrentIndex( sds_p->resolutionIdx );


}
