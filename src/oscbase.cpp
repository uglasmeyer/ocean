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
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Oscbase.h>
#include <Frequency.h>
extern Frequency_class* frequency;

Oscillator_base::Oscillator_base(  OSCID_e osc_type, RoleId_e role  )
	: Logfacility_class	( "Oscillator_base" )
	, osctype_struct	( osc_type, role )
	, fp				( FMOID )
	, vp				( VCOID )
	, DynFrequency		( DYNFACE_e::FRQ, frqext_range, frqslide_range )
	, spectrum			()
{
	Set_spectrum		( default_spectrum, INIT );
	this->Connect		= { osc_type, osc_type };
};

uint8_t Oscillator_base::Set_frequency( string frqName, DYNAMIC_e mode )
{
	uint index = frequency->Index( frqName );
	return Set_frequency( index, mode );
}

uint8_t Oscillator_base::Set_frequency( int frqidx, DYNAMIC_e mode )
{
	frqidx				= DynFrequency.Setup( frqidx, mode, 0 );
	spectrum.frqidx[0]  = frqidx;
	return 				frqidx;
}

void Oscillator_base::Set_spectrum( spectrum_t spectrum, DYNAMIC_e mode )
{
	this->spectrum 		= spectrum;
	this->spectrum.osc	= typeId;
	Set_frequency		( spectrum.frqidx[0], mode );
	for( size_t channel = 0; channel < SPEC_CHANNELS; channel++ )
		DynFrequency.Setup( spectrum.frqidx[channel], mode, channel );
}

void Oscillator_base::Set_pmw( uint8_t pmw )
{
	features.PWM = check_range( pmw_range, pmw );
}

void Oscillator_base::Set_frq_slidevalue( uint8_t value )
{
	features.frq_slide_value = value;
}

void Oscillator_base::Set_feature( feature_t f )
{
	features = f;
}

void Oscillator_base::Set_waveform( spec_arr_wt wf_vec )
{

	spectrum.wfid	= wf_vec;
	spectrum.phi	= zero_phase; // because of maxphi changes
}

void Oscillator_base::Set_volume( int vol )
{
	// set the volume of the oscillator this
	// consider that the dynamic volume is managed by the mixer
	// vco and fmo have no dynamic volume
	spectrum.volidx[0] 	= check_range( volidx_range, vol, "Set_volume" );
}

void Oscillator_base::Line_interpreter( string_vec_t arr )
{
	String 			Str{""};

	vp.name			= osctype_name;
	fp.name			= osctype_name;
	spectrum.wfid[0]= waveform->Get_id( arr[2] );
	int
	frqidx	 		= Str.secure_stoi( arr[3] );
	Set_frequency	( frqidx, FIXED );

	uint8_t
	msec 			= Str.secure_stoi(arr[4]);
	wp.msec 		= msec;
	wp.frames		= check_range( frames_range,  wp.msec * frames_per_msec, "Setwp_frames" );
	spectrum.volidx[0] 		= Str.secure_stoi(arr[5]);
	features.frq_slide_value = Str.secure_stoi( arr[13] );
	features.PWM 	= Str.secure_stoi( arr[14] );

	return ;
};



void Oscillator_base::Get_sound_stack( Table_class* T )
{
	int frqidx = spectrum.frqidx[0];
	T->AddRow( 	osctype_name,
				vp.name,
				fp.name,
				waveform->Get_str( spectrum.wfid[0] ),
				(int) spectrum.volidx[0],
				"%",
				frequency->GetFrq( frqidx ),
				"Hz"
			);
}


