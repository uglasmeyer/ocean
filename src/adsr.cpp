/*
 * adsr.cpp
 *
 *  Created on: Aug 1, 2025
 *      Author: sirius
 */

#include <Adsr.h>
#include <Oscwaveform.h>

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
	if ( adsr_data.bps	 	== 0 ) return;
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


	param_t			param 		= param_struct();
	uint8_t			wfid		= adsr_data.spec.wfid[0];
	wave_function_t	fnc			= adsrFunction_vec[ wfid ].fnc ;
					param.maxphi= adsrFunction_vec[ wfid ].maxphi;
	float 			dT 			= param.maxphi / bframes;
	float 			freq 		= 1.0;
					param.dphi	= dT *( freq );
					param.amp	= 1.0;

	buffer_t		aframes 	= 1;
	if ( adsr_data.attack 	> 0 )
					aframes 	= rint ( ( bframes * adsr_data.attack ) * 0.01 );;
	const float 	d_delta		= ( (100 - adsr_data.decay )/3.0) / bframes;
	const float 	delta 		= (bframes - aframes) * d_delta;
	const float		y0 			= expf( - delta );
	const float 	dy			= (1.0 - y0) / aframes;

	for ( buffer_t n = 0; n < bframes ; n++ )
	{
		param.phi	= param.phi + param.dphi;
		param.phi 	= MODPHI( param.phi, param.maxphi );

		adsr_Mem.Data[n] = adsr_fnc( aframes, n, y0, dy, d_delta ) * fnc( param );
	}
}

void ADSR_class::Set_hallcursor( buffer_t cursor )
{
	if ( beat_frames == 0 ) return;
	hall_cursor 	= cursor % beat_frames;

	if ( cursor == 0 )
	{
		buffer_t
		hframes		= rint( adsr_data.hall * min_frames * 0.01 );
		hall_cursor	= ( beat_cursor + hframes ) % beat_frames;
	}
}
void ADSR_class::Set_beatcursor( buffer_t cursor )
{
	beat_cursor = cursor;
}

void ADSR_class::Set_feature( feature_t f )
{
	feature = f;
}

void ADSR_class::Set_adsr( adsr_t _adsr )
{
	adsr_data = _adsr;

	// overwrite instrument settings
	if ( ( has_notes_role ) )
	{
		adsr_data.bps = 1;
		beat_frames = wp.frames;
	}
	else
	{
		if ( adsr_data.bps > 0 )
			beat_frames = rint( adsr_frames / adsr_data.bps );
		else
			beat_frames = 0;
	}

	Set_hallcursor();
	adsrOSC( beat_frames );
}


string ADSR_class::Show_adsr	( adsr_t _adsr )
{
	return Show_spectrum_type( SPEW, _adsr.spec );
}



