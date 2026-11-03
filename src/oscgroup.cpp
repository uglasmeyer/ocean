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
 * Oscgroup.cpp
 *
 *  Created on: Jan 22, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Oscgroup.h>
#include <data/Device.h>

Oscgroup_class::Oscgroup_class( RoleId_e role, buffer_t bytes )
	: Logfacility_class( "Oscgroup_class" )
	, roleid( role )
	, vco	( roleid, VCOID, bytes )
	, fmo	( roleid, FMOID, bytes )
	, osc	( roleid, OSCID, bytes )
{
	this->member 	= { &vco, &fmo, &osc };
	DataReset();
	selfTest();
}


void Oscgroup_class::operator=( Oscgroup_class& oscg )
{
	this->vco = oscg.vco;
	this->fmo = oscg.fmo;
	this->osc = oscg.osc;
}

Oscgroup_class::~Oscgroup_class()
{
	DESTRUCTOR( className )
}

void Oscgroup_class::SetSpectrum( const spectrum_arr_t& arr )
{
	for( OSCID_e oscid : oscIds )
	{
		DYNAMIC_e mode = ( member[oscid]->features.frq_slide_value > 0 )	? SLIDE	: FIXED;
		member[oscid]->Set_spectrum	( arr[ oscid ], mode );
	}
}

void Oscgroup_class::Read_fft_coeff( const string& file )
{
	fft_file_struct	fft_file{};
	fft_file.Read			( file );
	for_each( member,
			[ &fft_file ](Oscillator*  o)
			{
				o->Oscillator::fourier.Arr2Coeff( fft_file.GetData_p()->osc[o->typeId] );
				o->ADSR_class::fourier.Arr2Coeff( fft_file.GetData_p()->adsr[o->typeId] );
			} );
}

void Oscgroup_class::Save_fft_coeff( const string& file )
{
	fft_file_struct	fft_file		{};
	for_each( member,
			[ &fft_file ](Oscillator*  o)		// transfer fft coefficients to osc_fourier data
			{
				fft_file.SetOsc_Data( o->typeId, o->Oscillator::fourier.Coeff2Arr() );
				fft_file.SetAdsrData( o->typeId, o->ADSR_class::fourier.Coeff2Arr() );
			} );
	fft_file.Save( file );	// save all  fft coefficients to disk
}


void Oscgroup_class::SetInstrument( interface_t* sds )
{
	// init the osc with the sds content
	Comment( DEBUG, "SetInstrument from sds");
	SetAdsr			( sds->adsr_arr );
	SetFeatures		( sds->features );
	OscConnectState	( sds->connect_arr );
	SetSpectrum		( sds->spectrum_arr );
}

void Oscgroup_class::ConnectionReset()
{
	for_each( member, [](Oscillator*  o)
			{ o->Reset_connections() ;});
}
void Oscgroup_class::OscConnectState( const connect_arr_t& arr )
{
	// OSC <- SEC
	for( Oscillator* osc_p : member )
	{
		connectId_t Connect = arr[osc_p->typeId];
		osc_p->Set_connection( member[ Connect.frq ], member[ Connect.vol] );
	}
}

void Oscgroup_class::SetFeatures( const feature_arr_t& features )
{
	for( OSCID_e	oscid : oscIds )
	{
		member[ oscid ]->Set_feature( features[oscid] );
	}
}
void Oscgroup_class::SetAdsr( const adsr_arr_t& arr )
{
	for_each( oscIds, [ this, &arr ] (OSCID_e oscid )
			{ member[oscid]->Set_adsr	( arr[oscid] ); } );
}

void Oscgroup_class::KbdAttack()
{
	for( OSCID_e	oscid : oscIds )
	{
		member[oscid]->KbdAttack();
	}
}

void Oscgroup_class::SetFrqSlidevalue( const uint8_t& value )
{
	for_each( member, [ value ](Oscillator*  o)
			{ o->Set_frq_slidevalue( value ); });
}

void Oscgroup_class::SetWd( Wavedisplay_class* Wd )
{
	for_each( member, [ this, Wd ](Oscillator*  osc )
		{ Wd->Add_data_ptr( osc->typeId,
							osc->roleId,
							osc->MemData_p(),
							&osc->wp.frames );
		});

	if( osc.roleId == INSTRROLE )
	{
		for_each( member, [ this, Wd ](Oscillator*  osc )
		{
			Wd->Add_data_ptr( 	osc->typeId,
								ADSRROLE,
								osc->AdsrMemData_p(),
								&osc->adsr_wp_frames );
		});
	}
}

void Oscgroup_class::SetScanner( const buffer_t& maxlen )
{
	for_each( member, [ maxlen ](Oscillator*  o)
	{
		o->scanner.Set_fillrange( maxlen );
	});

}
void Oscgroup_class::SetBeatcursor( buffer_t pos )
{
	for_each( member, [ pos ]( Oscillator*  o)
	{
		o->Set_beatcursor( pos );

	});
}

void Oscgroup_class::ShowSoundStack() // show_status
{
	Table_class 	Table{"Sound stack" };
	Table.AddColumn( "Osc", 4 );
	Table.AddColumn( "Vol", 4 );
	Table.AddColumn( "Frq", 8 );
	Table.AddColumn( "Waveform", 10 );
	Table.AddColumn( "Vol", 4 );
	Table.AddColumn( "U", 8);
	Table.AddColumn( "Frq", 4 );
	Table.AddColumn( "U", 3);

	Table.PrintHeader();
	for_each( member, [ this, &Table ](Oscillator*  o)
			{ o->Get_sound_stack( &Table) ; });
}

bool Oscgroup_class::Sync()
{
	return osc.Sync();
}

void Oscgroup_class::setNoteFrequency	( 	interface_t*	sds,
											const uint8_t& 	idx,
											DYNAMIC_e mode )
{

	int diff 		= 0;

	osc.Set_frequency( idx - diff, mode );

	if( sds->features[VCOID].combine ) // keep index distance to main osc
	{
		diff = sds->spectrum_arr[OSCID].frqidx[0] - sds->spectrum_arr[VCOID].frqidx[0];
		vco.Set_frequency( idx - diff, mode );
	}
	if( sds->features[FMOID].combine )
	{
		diff = sds->spectrum_arr[OSCID].frqidx[0] - sds->spectrum_arr[FMOID].frqidx[0];
		fmo.Set_frequency( idx - diff, mode );
	}
}

void Oscgroup_class::setSliderFrequency( 	interface_t*		sds ,
											const uint8_t& 		idx,
											const DYNAMIC_e& 	mode )
{
	int diff = idx - osc.spectrum.frqidx[0];
	osc.Set_frequency( idx, mode );

	if( sds->features[VCOID].combine )
	{
		vco.Set_frequency( sds->spectrum_arr[VCOID].frqidx[0] + diff, mode );
		sds->spectrum_arr[VCOID].frqidx[0] = vco.spectrum.frqidx[0];
	}
	if( sds->features[FMOID].combine )
	{
		fmo.Set_frequency( sds->spectrum_arr[FMOID].frqidx[0] + diff, mode );
		sds->spectrum_arr[FMOID].frqidx[0] = fmo.spectrum.frqidx[0];
	}
}

void Oscgroup_class::SetSliderFrequency( interface_t* sds )
{
	uint8_t		idx 	= sds->spectrum_arr[OSCID].frqidx[0];
	DYNAMIC_e 	mode 	= sds->frq_slidermode;
	setSliderFrequency	( sds, idx, mode );
}

void Oscgroup_class::ShiftSliderFrequency( interface_t* sds )
{
	int8_t shift 						= sds->Noteline_prefix.octave_shift;
	sds->Noteline_prefix.octave_shift 	= 0;
	uint8_t idx 						= sds->spectrum_arr[OSCID].frqidx[0];
	uint8_t frqidx 						= check_range( frqext_range, idx + shift*oct_steps );
	setSliderFrequency					( sds, frqidx, COMBINE );
	sds->spectrum_arr[OSCID].frqidx[0]	= frqidx;
}


void Oscgroup_class::SetDuration( const uint& msec )
{
	uint duration = check_range( duration_range, msec, "Set_Duration");
	for_each( member, [ &duration ](Oscillator*  o)
		{ o->Setwp_frames( duration );});
}
void Oscgroup_class::ResetBeatCursor()
{
	for_each( member, [ ](Oscillator*  o)
		{ o->ResetBeatCursor();});

}
void Oscgroup_class::ChordData( interface_t* sds, chord_t chord )
{
	const uint 		frame_delay	= sds->Noteline_prefix.chord_delay * frames_per_msec;
	DYNAMIC_e		mode		= ( osc.features.frq_slide_value > 0 )	? SLIDE	: FIXED;
	uint 			n 			= 0;

	for ( pitch_t& pitch : chord )
	{
		restorePhase			( pitch );
		setNoteFrequency		( sds, pitch.frqidx, mode );
		RunOSCs					( n*frame_delay );
		backup_phase			( pitch );
		n++;
	}
}

void Oscgroup_class::DataReset()
{
	for_each( member,
			[](Oscillator*  o)
			{
				o->Data_reset() ;
			} );
}

void Oscgroup_class::restorePhase( pitch_t& pitch)
{
	for_each( member,
			[ this, &pitch ]( Oscillator* osc )
			{
				osc->spectrum.phi = pitch.phi[osc->typeId];
			} );
}
void Oscgroup_class::backup_phase( pitch_t& pitch )
{
	for_each( member,
			[ this, &pitch ]( Oscillator* osc )
			{
				pitch.phi[osc->typeId] = osc->spectrum.phi;
			} );
}
void Oscgroup_class::RunOSCs( const buffer_t& offs )
{
	for_each( member, [ &offs ]( Oscillator* osc )
	{
		osc->OSC( offs ) ;
	} );
}

OSCID_e Oscgroup_class::GetOscid_byName( const string& name )
{
	for( OSCID_e id : oscIds )
	{
		if ( strEqual( typeNames[id], name ) )
			return id;
	}
	Exception( "unknown Oscillator name: " + name );
	return NOOSCID;
}

Oscillator* Oscgroup_class::GetOsc_byName( const string& name )
{
	Oscillator* ret = nullptr;

	for_each( member, [ name, &ret ](Oscillator* o)
	{
		if ( strEqual( o->osctype_name, name ) )
			ret = o;
	}
	);
	if ( strEqual( "MAIN", name  ) ) // compatibility
		ret = member[ OSCID ];
	if ( ret == nullptr )
		{ Exception( "unknown Oscillator name: " + name ); }

	return ret;
}

void Oscgroup_class::selfTest()
{
	ASSERTION( fmo.spectrum.osc == FMOID, "fmo.spectrum.osc",
				(int) fmo.spectrum.osc, (int) FMOID );

	adsr_data_t adsr = fmo.Get_adsr();
	Assert_equal( adsr.spec.type, spectrum_ns::SPEC_e::ADSR );
}

void Oscgroup_class::Test( interface_t* sds )
{
	TEST_START( className );

	// Note Frequency
//	sds->frq_slidermode			= FIXED;
	sds->features[VCOID].combine = true;
	sds->features[FMOID].combine = true;
	sds->spectrum_arr[OSCID].frqidx[0] = A3;
	sds->spectrum_arr[VCOID].frqidx[0] = A4;
	sds->spectrum_arr[FMOID].frqidx[0] = A2;
	setNoteFrequency( sds, A2, FIXED );
	Assert_equal( osc.spectrum.frqidx[0], A2 );
	Assert_equal( vco.spectrum.frqidx[0], A3 );
	Assert_equal( fmo.spectrum.frqidx[0], A1 );

	sds->features[VCOID].combine = false;
	sds->features[FMOID].combine = false;
	setNoteFrequency(sds, A3, FIXED );
	Assert_equal( osc.spectrum.frqidx[0], A3 );
	Assert_equal( vco.spectrum.frqidx[0], A3 );
	Assert_equal( fmo.spectrum.frqidx[0], A1 );

	TEST_END( className );
}
