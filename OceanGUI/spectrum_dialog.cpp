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
#include <DataGraphic.h>
#include <Oscwaveform.h>

// OceanGUI
#include <Spectrum_dialog.h>

extern Frequency_class* frequency	;

/**************************************************
 * Mode_struct
 *************************************************/
Mode_struct::Mode_struct()
{
	at_mode_map =
	{	//														scale	, cursor	, center
		{ MODE_e::MANUAL,	{ WFID_e::ADSR		, "Manual"	, { 1.0		, false		, false }} },
		{ MODE_e::FOURIER, 	{ WFID_e::FOURIER	, "Fourier"	, { 1.0		, false		, true } } }
	};
	aw_mode_map =
	{
		{ MODE_e::MANUAL,	{ WFID_e::WFID_SIZE	, "Manual"	, { 1.0		, false		, false } } },
		{ MODE_e::FOURIER, 	{ WFID_e::FOURIER	, "Fourier"	, { 1.0		, false		, false } } }
	};

};

datagraphics_param_t Mode_struct::At_param( bool adsr )
{
	MODE_e mode	= adsr ? MODE_e::MANUAL : MODE_e::FOURIER;
	return at_mode_map[mode].param;
}
datagraphics_param_t Mode_struct::Aw_param( MODE_e mode )
{
	return aw_mode_map[mode].param;
}
WFID_e Mode_struct::Wfid( bool adsr, MODE_e mode )
{
	return adsr ? at_mode_map[mode].wfid : aw_mode_map[mode].wfid;
}
QString Mode_struct::Text( bool adsr, MODE_e mode )
{
	return adsr ? at_mode_map[mode].text : aw_mode_map[mode].text;
}

/**************************************************
 * Spectrum_Dialog_class
 *************************************************/
using namespace fourier_ns;
Spectrum_Dialog_class::Spectrum_Dialog_class(QWidget*			parent,
											 Dataworld_class* 	data ,
											 EventLog_class*	_log) :
    Logfacility_class	( "Spectrum_Dlg" ),
	Interface_base		( data ),
	QDialog				( parent ),
	Mode				(),
	ui					( new Ui::Spectrum_Dialog_class ),
	Resolution			( resolution_max )
{
	auto initcBs = [ this ](  )
	{
		QStringList			qresolution_lst = Qstringvector( Resolution.Str_vec );
		ui->cB_resolution->addItems			( qresolution_lst );

		QStringList			Qbps_str_lst	= Qstringlist( bps_struct().Bps_lst );
		ui->cb_bps->addItems				( Qbps_str_lst );

		Qfilter_str_lst						= Qstringvector( fourierfilter_txt );
		ui->cB_fourierfilter->addItems		( Qfilter_str_lst );
	};

	auto inituiVectors = [ this ](  )
	{
		fS_vec = { nullptr, ui->fS_1, ui->fS_2, ui->fS_3, ui->fS_4 };
		vS_vec = { nullptr, ui->vS_1, ui->vS_2, ui->vS_3, ui->vS_4 };
		sb_vec = { nullptr, ui->sb_spwf1, ui->sb_spwf2, ui->sb_spwf3, ui->sb_spwf4 };
		rb_vec = { ui->rb_spec_vco, ui->rb_spec_fmo, ui->rb_spec_main };
	};

	auto initValues = [ this ](  )
	{
		for( uint8_t channel : dialog_channel_vec )
		{
			fS_vec[channel]->setMaximum( harmonic_range.max );
			fS_vec[channel]->setMinimum( harmonic_range.min );
		}
		ui->hs_attack->setValue				( sds_p->adsr_arr[OscId].attack );
		ui->hs_decay ->setValue				( sds_p->adsr_arr[OscId].decay  );
		ui->rb_spec_main->setChecked		( true );
	};

	auto initConnections = [ this ](  )
	{
		connect( ui->fS_1	, SIGNAL(valueChanged(int)), this, SLOT(fS1( int )));
		connect( ui->vS_1	, SIGNAL(valueChanged(int)), this, SLOT(vS1( int )));
		connect( ui->fS_2	, SIGNAL(valueChanged(int)), this, SLOT(fS2( int )));
		connect( ui->vS_2	, SIGNAL(valueChanged(int)), this, SLOT(vS2( int )));
		connect( ui->fS_3	, SIGNAL(valueChanged(int)), this, SLOT(fS3( int )));
		connect( ui->vS_3	, SIGNAL(valueChanged(int)), this, SLOT(vS3( int )));
		connect( ui->fS_4	, SIGNAL(valueChanged(int)), this, SLOT(fS4( int )));
		connect( ui->vS_4	, SIGNAL(valueChanged(int)), this, SLOT(vS4( int )));
		connect( ui->sb_spwf1, SIGNAL(valueChanged(int)), this, SLOT( sb_wf1(int) ));
		connect( ui->sb_spwf2, SIGNAL(valueChanged(int)), this, SLOT( sb_wf2(int) ));
		connect( ui->sb_spwf3, SIGNAL(valueChanged(int)), this, SLOT( sb_wf3(int) ));
		connect( ui->sb_spwf4, SIGNAL(valueChanged(int)), this, SLOT( sb_wf4(int) ));
		connect( ui->rb_spec_fmo	, SIGNAL( clicked()), this, SLOT( select_oscid_fmo() ) );
		connect( ui->rb_spec_vco	, SIGNAL( clicked()), this, SLOT( select_oscid_vco() ) );
		connect( ui->rb_spec_main	, SIGNAL( clicked()), this, SLOT( select_oscid_main() ));
		connect( ui->rb_high_low	, SIGNAL( clicked()), this, SLOT( rb_high_low_slot() ));

		connect( ui->pB_save_spectrum		, SIGNAL(clicked()),
				this						, SLOT( saveInstrument( )));
		connect( ui->hs_attack				, SIGNAL(valueChanged(int)),
				this						, SLOT( attack(int) ));
		connect( ui->hs_decay				, SIGNAL(valueChanged(int)),
				this						, SLOT( decay(int) ));
		connect( ui->cb_adsr				, SIGNAL( clicked(bool)),
				this						, SLOT( adsr_slot(bool) ));
		connect( ui->pB_Fourier				, SIGNAL( clicked())		,
				this						, SLOT( pB_Fourier_slot() ));
		connect( ui->cB_resolution			, SIGNAL( activated(int) ),
				this						, SLOT( resolution_slot( int ) ));
		connect( ui->cb_bps					, SIGNAL( activated(int) )	,
				this						, SLOT( cb_bps_slot(int) ));
		connect( ui->rb_reset				, SIGNAL( clicked())	,
				this						, SLOT( reset() ) );
		connect( ui->cB_fourierfilter		, SIGNAL( activated(int)),
				this						, SLOT( fourierfilter_slot( int ) ));
		connect( ui->sl_shift_frequency		, SIGNAL( valueChanged( int )),
				this						, SLOT( shift_frequency( int ) ));
	};


	Eventlog_p				= _log;
    this->OscId				= NOOSCID;

	ui->setupUi				( this );
	move					( 0,300 );
	initcBs					();
	inituiVectors			();
	Interface_base::Set_Sds	();
    this->OscId				= sds_p->SpectrumOscId;
	initValues				();
	initDialog				();
	initDataDisplay			();
	initConnections			();

}

Spectrum_Dialog_class::~Spectrum_Dialog_class()
{
	DESTRUCTOR( className );
	delete ui->vS_4;
	delete ui->vS_2;
	delete ui->vS_3;
	delete ui->vS_1;
	delete AtDataDisplay;
	delete atScene;
	delete AwDataDisplay;
	delete awScene;
	delete ui;
}

void Spectrum_Dialog_class::initDialog()
{
	adsrmode			= sds_p->WD_state.adsrmode;
    Waveform			= adsrmode ? adsrform : waveform;
	Select_oscid		( sds_p->SpectrumOscId );
	Set_adsr_flag		( adsrmode );
	ui->rb_high_low->setChecked( false ); // set focus to low frequencies
	rb_high_low_slot	();
    SetInstrument		();
    set_fourier_mode	( MANUAL );
	ui->pB_fourier_input->setText( Qwd_role_names[ sds_p->WD_state.roleId ] );

}


void Spectrum_Dialog_class::initDataDisplay()
{
    datagraphics_param_t 	param 	= Mode.At_param( adsrmode );
    atScene 						= new QGraphicsScene( this );
    AtDataDisplay					= new DataGraphic_class( Sds->addr, ui->adsr_dataview, param ) ;
    ui->adsr_dataview->setScene		( atScene );
    atScene->addItem				( AtDataDisplay );

    param 							= Mode.Aw_param( sds_p->fourier.mode );
	awScene							= new QGraphicsScene( this );
	AwDataDisplay					= new DataGraphic_class( Sds->addr, ui->fft_dataview, param ) ;
	ui->fft_dataview->setScene		( awScene );
    awScene->addItem				( AwDataDisplay );

    UpdateDataDisplay				();
}


void Spectrum_Dialog_class::UpdateDataDisplay()
{
	AtDataDisplay->Set_parameter( Mode.At_param( adsrmode ));
	AtDataDisplay->ReadData		( &sds_p->adsrdata[0], adsrdisplay_len );
    atScene->update				();

   	AwDataDisplay->Set_parameter( Mode.Aw_param( sds_p->fourier.mode ) );
    AwDataDisplay->ReadData		( &sds_p->fft_data[0], fft_display_len );
    awScene->update				();
}

void Spectrum_Dialog_class::add_event( EVENTKEY_e key )
{
	Eventlog_p->add( SDS_ID, key );
}

void Spectrum_Dialog_class::Dialog( bool adsr )
{
	if( this->isVisible() )
	{
		this->hide		();
		Set_adsr_flag	( false );
	}
	else
	{
		Set_adsr_flag	( adsr );
		initDialog		();
		this->show		();
	}
}

void Spectrum_Dialog_class::cb_bps_slot( int bps_id )
{
	uint8_t bps = bps_struct().Bps_vec[bps_id];
    Sds->Set( sds_p->adsr_arr[ OscId ].bps, bps  );
}


void Spectrum_Dialog_class::read_spectrum()
{
	spectrum	= adsrmode ? sds_p->adsr_arr[ OscId ].spec :
							 sds_p->spectrum_arr[ OscId ];
	if( adsrmode )
		adsr_reset_spectrum = spectrum;
	else
		reset_spectrum		= spectrum;
}

void Spectrum_Dialog_class::change_spectrum_view()
{
	read_spectrum		();
	set_waveform_vec	();
	rb_high_low_slot	();
	Channel				= select_channel( channel_range.min );
	setWaveformLabel	( spectrum.wfid[Channel] );
	QString title 		= adsrmode ? "ADSR" : "Spectrum";
	this->setWindowTitle( title );
}


void Spectrum_Dialog_class::save_spectrum_data()
{
	spectrum_t& spec_r	= adsrmode	? sds_p->adsr_arr[OscId].spec
									: sds_p->spectrum_arr[OscId];
	Sds->Set( spec_r, spectrum);

	adsrmode	? assert( spectrum.type == spectrum_ns::ADSR )
				: assert( spectrum.type == spectrum_ns::FREQ );
	assert( spectrum.osc == OscId );
}

void Spectrum_Dialog_class::Select_oscid( OSCID_e oscid )
{
	OscId				= oscid;
	if( not rb_vec[oscid]->isChecked() )
		rb_vec[oscid]->setChecked( true );
	Sds->Set			( sds_p->SpectrumOscId, OscId );
	Sds->Set			( sds_p->WD_state.oscId, OscId );
    Channel 			= select_channel( channel_range.min );
    read_spectrum		();
   	Setup_widgets		();
};

void Spectrum_Dialog_class::select_oscid_fmo()
{
	Select_oscid( FMOID );
}

void Spectrum_Dialog_class::select_oscid_vco()
{
	Select_oscid( VCOID );
}

void Spectrum_Dialog_class::select_oscid_main()
{
	Select_oscid( OSCID );
}

void Spectrum_Dialog_class::Set_adsr_flag( bool flag )
{// manage ADSR parameter or spectrum parameter
	adsrmode 	= flag;
    Waveform	= adsrmode ? adsrform : waveform;
    Sds->Set	( sds_p->WD_state.adsrmode, flag );
    change_spectrum_view();
	ui->cb_adsr->setChecked( flag );
}

void Spectrum_Dialog_class::adsr_slot( bool flag )
{
	Set_adsr_flag	( flag );
	initDialog		();
}

void Spectrum_Dialog_class::attack(int value )
{
	Sds->Set( sds_p->adsr_arr[ OscId ].attack, (uint8_t) value );
}

void Spectrum_Dialog_class::decay (int value )
{
	Sds->Set( sds_p->adsr_arr[ OscId ].decay, (uint8_t) value );
}

void Spectrum_Dialog_class::reset()
{
	spectrum 			= adsrmode ? adsr_reset_spectrum : reset_spectrum;
	rb_vec[spectrum.osc]->setChecked( true ); // and at the same time unset rb_reset
	save_spectrum_data	();
	Setup_widgets		();
}

void Spectrum_Dialog_class::update_lcd( float value, QString label )
{
	ui->lcd_spectrumDisplay->display( value );
	ui->lbl_spectrumDisplay->setText( label );
};

void Spectrum_Dialog_class::spec_frq_slider( int channel, int value )
{
	//modify the frequency of harmonics of the base frequency
	auto idx					= check_range(  harmonic_range, value, "spec_frq_slider");
    Channel 					= select_channel( channel );
	spectrum.frqidx[ Channel ] 	= idx;
	float	frqadj 				= frequency->Frqadj( Channel, idx ); // see osc.cpp
	int		frqidx				= sds_p->spectrum_arr[OscId].frqidx[0];
	float	frq					= frequency->GetFrq( frqidx ) * frqadj;

	update_lcd					( frq, "[Hz]" );
	save_spectrum_data			();
};

uint8_t Spectrum_Dialog_class::select_channel( int8_t ch )
{
	auto channel = check_cycle2( channel_range, int8_t( ch + highFrequencies * 4 ) );
	return channel;
};
void Spectrum_Dialog_class::spec_vol_slider( int channel, int value )
{
	//modify the amplitude of harmonics of the base frequency
    Channel 					= select_channel( channel );	// switch the current channel
	spectrum.volidx[ Channel ] 	= value;
    update_lcd					( value, "Amp [%]" );
    save_spectrum_data			();
};

void Spectrum_Dialog_class::rb_high_low_slot()
{
	highFrequencies = ( adsrmode )	? false	: ui->rb_high_low->isChecked();
	Setup_widgets();
}
void Spectrum_Dialog_class::fS1( int value )
{
	spec_frq_slider( 1, value );
}

void Spectrum_Dialog_class::vS1( int value )
{
	spec_vol_slider( 1, value );
}

void Spectrum_Dialog_class::fS2( int value )
{
	spec_frq_slider( 2, value );
}

void Spectrum_Dialog_class::vS2( int value )
{
	spec_vol_slider( 2, value );
}

void Spectrum_Dialog_class::fS3( int value )
{
	spec_frq_slider( 3, value );
}

void Spectrum_Dialog_class::vS3( int value )
{
	spec_vol_slider( 3, value );
}

void Spectrum_Dialog_class::fS4( int value )
{
	spec_frq_slider( 4, value );
}

void Spectrum_Dialog_class::vS4( int value )
{
	spec_vol_slider(  4, value );
}

void Spectrum_Dialog_class::set_waveform_vec()
{
	size_t max = Waveform->waveform_str_vec.size() - 1;
	for( size_t n = 1; n < sb_vec.size(); n++ )
	{
		sb_vec[n]->setMaximum( max );
	}
}

void Spectrum_Dialog_class::setWaveformLabel( WFID_e wfid )
{
//	WFID_e	wfid 				= spectrum.wfid[Channel];
	string wf_str				= Waveform->Get_str( wfid );
	ui->lbl_waveform->setText	( Qstring( wf_str ) );
}

void Spectrum_Dialog_class::waveform_spinbox( uint8_t sb_id, int idx  )
{
	Channel 				= select_channel( sb_id );
	WFID_e wfid 			= Waveform->Get_id( idx );
	spectrum.wfid[Channel] 	= wfid;
	save_spectrum_data		();
	setWaveformLabel		( wfid );
};

void Spectrum_Dialog_class::sb_wf1(int idx )
{
	waveform_spinbox( 1, idx );
}

void Spectrum_Dialog_class::sb_wf2(int idx )
{
	waveform_spinbox( 2, idx );
}

void Spectrum_Dialog_class::sb_wf3(int idx )
{
	waveform_spinbox( 3, idx );
}

void Spectrum_Dialog_class::sb_wf4(int idx )
{
	waveform_spinbox( 4, idx );
}

void Spectrum_Dialog_class::saveInstrument()
{
	save_spectrum_data();
	add_event( SAVEINSTRUMENTKEY );
}

void Spectrum_Dialog_class::resolution_slot( int idx )
{
	Sds->Set	( sds_p->fourier.residx, uint8_t(idx) );
}

void Spectrum_Dialog_class::shift_frequency( int value )
{
	int8_t	shift	= value;
	Sds->Set		( sds_p->fourier.shift, shift );
	if( sds_p->fourier.mode == MANUAL ) return;
	Sds->Set( sds_p->fourier.filter, SHIFT );
	Sds->Set( sds_p->fourier.trigger, true );
}

void Spectrum_Dialog_class::fourierfilter_slot( int idx )
{
	FILTER_e filter = FILTER_e( idx );
	Sds->Set		( sds_p->fourier.filter, filter );
}

void Spectrum_Dialog_class::set_fourier_mode( MODE_e mode )
{
	WFID_e	wfid			= Mode.Wfid( adsrmode, mode );
	spectrum.wfid[0]		= ( wfid  == WFID_SIZE )	? reset_spectrum.wfid[0] : wfid;
	QString text			= Mode.Text( adsrmode, mode );
	ui->pB_Fourier->setText	( text );
	Sds->Set				( sds_p->fourier.mode	, mode );
	if( mode == MANUAL )
	{
		Sds->Set			( sds_p->fourier.shift, int8_t( 0 ) );
		ui->sl_shift_frequency->setValue( 0 );
	}
	save_spectrum_data		();
	if( sds_p->fourier.filter == SHIFT )
		Sds->Set( sds_p->fourier.filter, AUTO );
	Sds->Set				( sds_p->fourier.trigger, ( mode == MODE_e::FOURIER ) );
}

void Spectrum_Dialog_class::pB_Fourier_slot()
{	// Mode		: Description
	// Fourier	: use the waveform shown in the main display
	//			to generate a waveform by fourier analyzis and
	//			display the amplitude in A(w)
	// Manual	: use waveform input widgets
	//			to manage the waveform and display the amplitude in A(w)
	// the resulting waveform is displayed in the spectrum dialog data display A(t)
	auto switch_mode = [ this ]( int m )
	{
		return 	MODE_e( (++m) % MODE_SIZE );
	};
	MODE_e	mode		= switch_mode( sds_p->fourier.mode );
	set_fourier_mode	( mode );
}

void Spectrum_Dialog_class::SetSds()
{
	Interface_base::Set_Sds	();
	initDialog				();
}

void Spectrum_Dialog_class::SetInstrument()
{
    QString	instrument			{ this->Sds->Read_str( INSTRUMENTSTR ).data() };
    ui->pB_save_spectrum->setText( "Save " + instrument );
	read_spectrum				();
   	Setup_widgets				();
}

void Spectrum_Dialog_class::Setup_widgets()
{
	for( uint8_t channel : dialog_channel_vec )
	{
		uint8_t ch = select_channel(channel);
		fS_vec[channel]->setValue( spectrum.frqidx[ch] );
		vS_vec[channel]->setValue( spectrum.volidx[ch] );
		sb_vec[channel]->setValue( waveform->Get_idx(spectrum.wfid[ch]) );
	}

	setWaveformLabel						( spectrum.wfid[ Channel ] );
    QString QStr							{ int2char( sds_p->adsr_arr[ OscId ].bps ) };
    ui->cb_bps->setCurrentText				( QStr );
    ui->cB_resolution->setCurrentIndex		( sds_p->fourier.residx );
    ui->cB_fourierfilter->setCurrentText	( Qfilter_str_lst[ sds_p->fourier.filter ] );
}
