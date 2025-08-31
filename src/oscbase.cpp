/*
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#include <Oscbase.h>

Oscillator_base::Oscillator_base()
	: Logfacility_class( "Oscillator_base" )
	, Spectrum_class()
{
	this->spectrum = default_spectrum;
};

Oscillator_base::Oscillator_base( char osc_type ) :
	Logfacility_class( "Oscillator_base" )
{
	this->spectrum.osc 	= osc_type;
	typeId				= osc_type;
	Connect				= default_connect( osc_type );
	this->className		= Logfacility_class::className;
};



void Oscillator_base::Setwp( wave_t _wp )
{
	this->wp 			= _wp;
}

uint8_t Oscillator_base::Set_frequency( string frqName, uint mode )
{
	uint index = Index( frqName );
	return Set_frequency( index, mode );
}

uint8_t Oscillator_base::Set_frequency( int arridx, uint mode )
{
	wp.frqidx 			= DynFrequency.SetupFrq( arridx, mode );
	wp.freq				= GetFrq( wp.frqidx );
	spectrum.frqadj[0] 	= Frqadj(0, 0);
	spectrum.frqidx[0]	= wp.frqidx;

	return wp.frqidx;
}

void Oscillator_base::Set_volume( int vol, uint mode )
{
	wp.volume 			= check_range( volidx_range, vol, "Set_volume" );
	spectrum.vol[0] 	= (float)vol * percent;
	spectrum.volidx[0]	= vol;
}
void Oscillator_base::Set_pmw( uint8_t pmw )
{
	wp.PMW_dial = pmw;
}

void Oscillator_base::Set_glide( uint value )
{
	feature.glide_effect = value;
}

void Oscillator_base::Set_waveform( spec_arr_8t wf_vec )
{
	spectrum.wfid	= wf_vec;
	phase 			= default_phase; // because of maxphi changes

}

void Oscillator_base::Set_spectrum( spectrum_t spectrum )
{
	this->spectrum 	= spectrum;
	Set_frequency	( spectrum.frqidx[0], FIXED );
	Set_volume		( spectrum.volidx[0], FIXED );
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
	wp.volume 		= Str.secure_stoi(arr[5]);
	feature.glide_effect = Str.secure_stoi( arr[13] );
	wp.PMW_dial 	= Str.secure_stoi( arr[14] );

	return ;
};



void Oscillator_base::Get_sound_stack( Table_class* T )
{

	T->AddRow( 	osctype_name,
				vp.name,
				fp.name,
				Get_waveform_str( spectrum.wfid[0] ),
				(int) wp.volume,
				"%",
				GetFrq( wp.frqidx),
				"Hz"
			);
}


