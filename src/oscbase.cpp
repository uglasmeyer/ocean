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

Oscillator_base::Oscillator_base( OSCID_e osc_type ) :
	Logfacility_class	( "Oscillator_base" )
	, typeId			( osc_type )
	, DynFrequency		(  frqext_range )
{
	this->spectrum		= default_spectrum;
	this->spectrum.osc 	= osc_type;

	this->Connect		= { osc_type, osc_type };
	this->phase 		= default_phase;
};

uint8_t Oscillator_base::Set_frequency( string frqName, DYNAMIC mode )
{
	uint index = Index( frqName );
	return Set_frequency( index, mode );
}

uint8_t Oscillator_base::Set_frequency( int frqidx, DYNAMIC mode )
{
	frqidx				= DynFrequency.SetupFrq( frqidx, mode );
	spectrum.frqadj[0] 	= Frqadj(0, 0);
	spectrum.frqidx[0]  = frqidx;
	return 				frqidx;
}

void Oscillator_base::Set_spectrum_volume( int vol )
{
	spectrum.volidx[0] 	= check_range( volidx_range, vol, "Set_volume" );
	spectrum.vol[0] 	= (float)vol * percent;
}
void Oscillator_base::Set_pmw( uint8_t pmw )
{
	features.PWM = pmw;
}

void Oscillator_base::Set_slideFrq( uint8_t value )
{
	features.slide_frq = value;
}

void Oscillator_base::Set_waveform( spec_arr_8t wf_vec )
{
	spectrum.wfid	= wf_vec;
	phase 			= default_phase; // because of maxphi changes

}

void Oscillator_base::Set_spectrum( spectrum_t spectrum )
{
//	Set_frequency	( spectrum.frqidx[0], FIXED );
	Set_frequency	( spectrum.frqidx[0], SLIDE );
	Set_spectrum_volume		( spectrum.volidx[0] );
	this->spectrum 	= spectrum;
//	Set_waveform	( spectrum.wfid );
}


void Oscillator_base::Line_interpreter( vector_str_t arr )
{
	String 			Str{""};

	vp.name			= osctype_name;
	fp.name			= osctype_name;
	spectrum.wfid[0]= Get_waveform_id( arr[2] );
	int
	frqidx	 		= Str.secure_stoi( arr[3] );
	Set_frequency	( frqidx, FIXED );

	uint8_t
	msec 			= Str.secure_stoi(arr[4]);
	wp.msec 		= msec;
	wp.frames		= check_range( frames_range,  wp.msec * frames_per_msec, "Setwp_frames" );
	spectrum.volidx[0] 		= Str.secure_stoi(arr[5]);
	features.slide_frq = Str.secure_stoi( arr[13] );
	features.PWM 	= Str.secure_stoi( arr[14] );

	return ;
};



void Oscillator_base::Get_sound_stack( Table_class* T )
{
	int frqidx = spectrum.frqidx[0];
	T->AddRow( 	osctype_name,
				vp.name,
				fp.name,
				Get_waveform_str( spectrum.wfid[0] ),
				(int) spectrum.volidx[0],
				"%",
				GetFrq( frqidx ),
				"Hz"
			);
}


