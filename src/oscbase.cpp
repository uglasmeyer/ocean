/*
 * Trackclass.cpp
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */



#include <Oscbase.h>

ADSR_class::ADSR_class() :
	Logfacility_class("ADSR_class")
{
	className = Logfacility_class::className;

};
ADSR_class::~ADSR_class()
{
	if( LogMask[ DEBUG ] )
		coutf << "~" << className << endl;
}

Data_t* ADSR_class::AdsrMemData_p()
{
	return adsr_Mem.Data;
}

void ADSR_class::Apply_adsr( buffer_t frames, Data_t* Data, buffer_t frame_offset )
{
	if ( adsr.bps 		== 0 ) return;
	if ( beat_frames 	== 0 ) return;

	float		dbB 		= 0.5;
	float 		dbH			= 1.0 - dbB;
	uint		pos			= frame_offset;
				kbd_trigger = false;
	for ( uint m = 0; m < frames; m++ )
	{
		Data[ pos ] = Data[ pos ] * ( 	adsr_Mem.Data[ beat_cursor ]*dbB +
										adsr_Mem.Data[ hall_cursor ]*dbH );
		hall_cursor = ( hall_cursor + 1 ) % beat_frames;
		beat_cursor = ( beat_cursor + 1 ) % beat_frames;
		kbd_trigger = (( beat_cursor == 0 ) or kbd_trigger );
		pos			= ( pos + 1 ) % adsr_frames;
	}
	if ( has_kbd_role )
		Comment( DEBUG, "beat_cursor: " , (int) beat_cursor );
}

/* maxima
kill(all);
attack  : 10;
decay	: 95;
beatframes : 24000;
aframes : beatframes * attack^2 / 100;
d_delta	: ( (100 - decay )/3.0) / beatframes;
y0		: exp( - (beatframes-aframes) * d_delta );
d_alpha	: (1-y0)/aframes;
fattack : y0 + n^2*d_alpha;
fdecay	: exp( - ( n - aframes ) * d_delta );
plot2d([fattack, fdecay], [n,0,beatframes],[y,0,2]);

*/
auto adsr_fnc = [  ]( buffer_t aframes, buffer_t n, float y0, float dy, float d_delta  )
{
	if ( n < aframes ) 	// attack
	{
		return ( y0 + n*dy );
	}
	else				// decay
	{
		float delta 	= ( n - aframes ) * d_delta;
		return ( exp( -delta ) );
	}
};

void ADSR_class::adsrOSC( buffer_t bframes )
{
	if ( bframes == 0 ) return;

	buffer_t		aframes 	= 1;
	if ( adsr.attack 	> 0 )
					aframes 	= rint ( ( bframes * adsr.attack ) * 0.01 );;
	const float 	d_delta		= ( (100 - adsr.decay )/3.0) / bframes;
	const float 	delta 		= (bframes - aframes) * d_delta;
	const float		y0 			= expf( - delta );
	const float 	dy			= (1.0 - y0) / aframes;

	for ( buffer_t n = 0; n < bframes ; n++ )
	{
		adsr_Mem.Data[n] = adsr_fnc( aframes, n, y0, dy, d_delta );
	}
}

void ADSR_class::Set_hallcursor( buffer_t cursor )
{
	if ( beat_frames == 0 ) return;
	hall_cursor 	= cursor % beat_frames;

	if ( cursor == 0 )
	{
		buffer_t
		hframes		= rint( adsr.hall * min_frames * 0.01 );
		hall_cursor	= ( beat_cursor + hframes ) % beat_frames;
	}
}
void ADSR_class::Set_beatcursor( buffer_t cursor )
{
	beat_cursor = cursor;
}


void ADSR_class::Set_adsr( feature_t _adsr )
{
	adsr = _adsr;

	// overwrite instrument settings
	if ( ( has_notes_role ) )
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

	Set_hallcursor();
	adsrOSC( beat_frames );
}

Oscillator_base::Oscillator_base() :
	Logfacility_class( "Oscillator_base" ),
	Spectrum_class()
{
};
void Oscillator_base::Setwp_frames( uint16_t msec )
{
	wp.msec 	= msec;
	wp.frames	= check_range( frames_range,  wp.msec * frames_per_msec, "Setwp_frames" );
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
	wp.volume = check_range( volidx_range, vol, "Set_volume" );
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

	vp.name			= osctype_name;
	fp.name			= osctype_name;
	spectrum.wfid[0]= Get_waveform_id( arr[2] );
	int
	frqidx	 		= Str.secure_stoi( arr[3] );
	Set_frequency	( frqidx, FIXED );

	uint8_t
	msec 			= Str.secure_stoi(arr[4]);
	Setwp_frames	( msec );
	wp.volume 		= Str.secure_stoi(arr[5]);
	wp.glide_effect = Str.secure_stoi( arr[13] );
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


