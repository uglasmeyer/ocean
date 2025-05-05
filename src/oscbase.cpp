/*
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */



#include <Oscbase.h>



/* maxima

attack  : 10;
decay	: 95;
beatframes : 24000;
aframes : beatframes * attack / 100;
d_delta	: ( (100 - decay )/3.0) / beatframes;
y0		: exp( - (beatframes-aframes) * d_delta );
d_alpha	: (1-y0)/aframes;
fattack : y0 + n*d_alpha;
fdecay	: exp( - ( n - aframes ) * d_delta );
plot2d([fattack, fdecay], [n,0,beatframes]);

*/
void Oscillator_base::Gen_adsrdata( buffer_t bframes )
{
	if ( bframes == 0 ) return;

	buffer_t		aframes 	= 1;
	if ( adsr.attack 	> 0 )
				aframes = rint ( ( bframes * adsr.attack ) * 0.01 );;
	const float 	d_delta		= ( (100 - adsr.decay )/3.0) / bframes;
	const float 	delta 		= (bframes - aframes) * d_delta;
	const float		y0 			= expf( - delta );
	const float 	dy			= (1.0 - y0) / aframes;

	for ( buffer_t n = 0; n < bframes ; n++ )
	{
		if ( n < aframes ) 	// attack
		{
			adsrdata[n] 	= ( y0 + n*dy );
		}
		else				// decay
		{
			float delta 	= ( n - aframes ) * d_delta;
			adsrdata[n] 	= ( exp( -delta ) );
		}
	}
}

void Oscillator_base::set_beatcursorR()
{
	if ( beat_frames == 0 ) return;
	buffer_t hframes = rint( adsr.hall * min_frames * 0.01 );
	beat_cursorR = ( beat_cursorL + hframes ) % beat_frames;
}
void Oscillator_base::Set_adsr( adsr_t _adsr )
{
	adsr = _adsr;

	// overwrite instrument settings
	if ( ( has_kbd_role ) or ( has_notes_role ) )
	{
		adsr.bps = 1;
		beat_frames = wp.frames;
	}
	else
	{
		if ( adsr.bps > 0 )
			beat_frames = rint( max_frames / adsr.bps );
		else
			beat_frames = 0;
	}
	set_beatcursorR();
	Gen_adsrdata( beat_frames );
}


void Oscillator_base::Setwp_frames( uint16_t msec )
{
	wp.msec 	= msec;
	wp.frames	= rint( wp.msec * frames_per_msec );
	wp.frames	= check_range( frames_range, wp.frames );
}

uint8_t Oscillator_base::Set_frequency( string frqName, uint mode )
{
	uint index = Index( frqName );
	return Set_frequency( index, mode );
}

uint8_t Oscillator_base::Set_frequency( int arridx, uint mode )
{
	wp.frqidx = DynFrequency.SetupFrq( arridx, mode );
	spectrum.frqadj[0] 	= Frqadj(0, 0);
	spectrum.frqidx[0]	= wp.frqidx;

	return wp.frqidx;
}

void Oscillator_base::Set_volume( int vol, uint mode )
{
	wp.volume = check_range( volidx_range, vol );
	spectrum.vol[0] 	= (float)vol * 0.01;
	spectrum.volidx[0]	= vol;
}
void Oscillator_base::Set_pmw( uint8_t pmw )
{
	wp.PMW_dial = pmw;
}

void Oscillator_base::Set_glide( uint8_t value )
{
	wp.glide_effect = value;
}

void Oscillator_base::Set_waveform( spec_arr_8t wf_vec )
{
	spectrum.wfid	= wf_vec;

}

void Oscillator_base::Set_spectrum( spectrum_t spectrum )
{
	this->spectrum 	= spectrum;
}

void Oscillator_base::Line_interpreter( vector_str_t arr )
{
	String 			Str{""};

	vp.name			= osc_name;
	fp.name			= osc_name;
	spectrum.wfid[0]= Get_waveform_id( arr[2] );
	int frqidx	 	= Str.secure_stoi( arr[3] );
	Set_frequency( frqidx, FIXED );

	uint8_t msec 		= Str.secure_stoi(arr[4]);
	Setwp_frames( msec );
	wp.volume 		= Str.secure_stoi(arr[5]);
//	wp.frames 		= wp.msec*audio_frames/1000;
	wp.glide_effect = Str.secure_stoi( arr[13] );
	wp.PMW_dial 	= Str.secure_stoi( arr[14] );

	return ;
};



void Oscillator_base::Get_sound_stack( Table_class* T )
{

	T->AddRow( 	osc_name,
				Get_waveform_str( spectrum.wfid[0] ),
				GetFrq( wp.frqidx),
				(int) wp.volume,
				vp.name,
				fp.name
			);
}
