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
 * oscwaveform.cpp
 *
 *  Created on: Feb 21, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <random>

#include <Oscwaveform.h>
#include <String.h>


std::random_device 				rd;
std::mt19937 					engine(rd());
std::uniform_real_distribution<> distrib( -1, 1 );


// https://stackoverflow.com/questions/6339970/c-using-function-as-parameter

float psgn( const float& x, const float& y )
{
	return ( x + 1.0 > y ) ? x : 0 ;
}

float modulo( const float& x,  const float& n )
{
	float 	d 	= floor ( x / n ); // floor corresponds to the gaussian bracket
	return 	x - d*n;
}

float maximum( const float& x,  const float& y)
{
	return ( x > y ) ? x : y;
}



float Rnd( param_t& p )
{ 	// provides values between -amp .. amp
	float rint = p.amp * distrib( engine );
	return rint;
}

Data_t Delta( param_t& p )
{
	float f1 = fmod(p.phi, p.phi + p.dphi);
	return ( f1 < p.phi ) ? p.amp : 0.0;
}

Data_t Const( param_t& p )
{
	return p.maxphi * p.amp ;
}
Data_t Sinus( param_t& p )
{
	return p.amp * (  sin( p.phi ) );
}
Data_t CoSinus( param_t& p )
{
	return p.amp * (  cos( p.phi ) );
}
Data_t SignSin( param_t& p )
{
	return ( p.amp * psgn(  sin ( p.phi ), p.pmw ));
}
Data_t Rectangle( param_t& p )
{ // maxima: round(mod(phi,1))*amp*2-amp;
	return - p.amp*( round(floor(p.phi)-p.phi)) - p.amp/2;
}
Data_t Triangle( param_t& p )
{ // maxima: f:2*amp*(abs(max( 1 - mod(phi, 2), -1 ) )-1)+amp;
	return (2*p.amp*(abs(maximum( 1 - modulo(p.phi, 2), -1 ) )-1)+p.amp);
}
Data_t SawTooth( param_t& p )
{ // right
	return ( p.amp * modulo( p.phi,1 ));
}
Data_t Sawtooth( param_t& p )
{ // left
	return ( p.amp * (1.0 - modulo( p.phi,1 )));
}
const float	lambda 		= 2.0;
const float lambda_2 	= lambda/2.0;
Data_t Trapeze( param_t& p )			// x 1__ dx
{// 0 < x < lambda, 1 < pmw < 2   		     /  \__
										// x   23
										// pwm=1.2 , expected:
	float 	dx	= ( p.pmw - 1.0f );		// dx = 0.2, dx=0 -> triangle	, dx=1 -> rectangle
//	if( dx == 1.0f )
//		return Rectangle( p );
//	if( dx == 0.0f )
//		return Triangle( p );
	float 	x1  = lambda_2 - dx;		// x1 = 0.8, x1 = 1.0			, x1 = 0.0
	float 	x2	= x1 + dx;				// x2 = 1.0, x2 = 1.0			, x2 = 1.0
	float	x3	= x2 + x1;				// x3 = 1.8, x3 = 2.0		 	, x3 = 1.0

	auto 	Y 	= [ x1, x2, x3 ]( float x )
	{
		if( x < x1 ) 					// 0..x1 positive slope
			return x / x1;
		if( x < x2 ) 					// x1..x2 const
			return 1.0f;
		// (a+b*x2)=1 -> a=1-b*x2, (a+b*x3)=0 -> a=-b*x3, =>  b=1/(x2-x3)=-1/x1, a=x3/x1
		if( x < x3 ) 					// x2..x3 negative slope
			return ( x3  - x ) / x1;
		// x < lambda
		return 0.0f;
		;
	};

	return p.amp * ( Y( p.phi ) - 0.5 );
}
Data_t Pmw( param_t& p )
{
	phi_t phi2 	= p.phi * p.pmw ;
	return p.amp *	abs(round(modulo( p.phi , p.maxphi ))) *
					abs(round(modulo( phi2	, p.maxphi ))) ;
}

Data_t 	prev_step = 0.0;
float 	rnd_step = 0.0;
Data_t Rnd_step( param_t& p )
{
	Data_t step = round(p.phi - floor(p.phi) );
	if ( not fcomp( step, prev_step ))
	{
		rnd_step = distrib( engine );
		prev_step = step;
	}
	return p.amp * step * rnd_step ;
}

//#include <Fourier.h>
Data_t FourierFnc( param_t& p )
{
	return 0.0;// dummy fnc to satisfy the waveFunction vector
}

Data_t Zero( param_t p )
{
	return 0.0;
}

typedef struct wfid_map_struct
{
	WFID_e id;
	string str;
} wfid_map_t;

typedef vector<wfid_map_t> wf_id_str_map_t;

wf_id_str_map_t all_wf_id_str_map
{
	{ SINUS		, "Sinus" },
	{ COSIN		, "Cosinus" },
	{ TRIANGLE	, "Triangle" },
	{ SGNSIN	, "SignSin" },
	{ RECTANGLE	, "Rectangle" },
	{ SAWTOOTHL	, "SawtoothL" },
	{ SAWTOOTHR	, "SawtoothR" },
	{ TRAPECE	, "Trapece" },
	{ PMW		, "PWM" },
	{ DELTA		, "Delta" },
	{ NOISE		, "Noise" },
	{ RANDOM	, "Random" },
	{ FOURIER	, "Fourier" },
	{ ZERO		, "Zero" },
	{ ADSR		, "Adsr" }
};

const waveFnc_vec_t waveFunction_vec
{
//   idx  fnc         lambda  size wfid
	{ 0	, Sinus		, 2*M_PI, 2.0, SINUS	}, // default
	{ 1	, Triangle	, 2		, 2.0, TRIANGLE },
	{ 2	, Trapeze	, lambda, 1.0, TRAPECE 	},
	{ 3	, Rectangle	, 1 	, 2.0, RECTANGLE},
	{ 4	, SignSin	, 2*M_PI, 1.0, SGNSIN 	},
	{ 5	, SawTooth	, 1		, 1.0, SAWTOOTHL},
	{ 6	, Sawtooth	, 1		, 1.0, SAWTOOTHR},
	{ 7	, Pmw		, 1		, 1.0, PMW 		},
	{ 8	, Delta		, 1		, 1.0, DELTA 	},
	{ 9	, Rnd		, 1		, 2.0, NOISE 	},
	{10	, Rnd_step	, 1		, 2.0, RANDOM 	},
	{11	, FourierFnc, 2*M_PI, 2.0, FOURIER 	}
};

const waveFnc_vec_t adsrFunction_vec
{
	{ 0	, CoSinus	, 2*M_PI, 2.0, COSIN	}, // default
	{ 1	, Sinus		, 2*M_PI, 2.0, SINUS 	},
	{ 2	, Triangle	, 2		, 2.0, TRIANGLE },
	{ 3	, SignSin	, 2*M_PI, 1.0, SGNSIN 	},
	{ 4	, Rectangle	, 1 	, 2.0, RECTANGLE},
	{ 5	, SawTooth	, 1		, 1.0, SAWTOOTHL},
	{ 6	, Sawtooth	, 1		, 1.0, SAWTOOTHR},
	{ 7	, Pmw		, 1		, 1.0, PMW 		},
	{ 8	, Rnd_step	, 1		, 2.0, RANDOM 	}
};

constexpr string_vec_t   gen_waveform_str_vec ( waveFnc_vec_t vec )
{
	assert( WFID_SIZE == all_wf_id_str_map.size() );
	string_vec_t	Vec{};
	for ( waveform_t wf : vec )
		Vec.push_back( all_wf_id_str_map[ wf.wfid ].str );
	return Vec;
};
constexpr unordered_set<WFID_e> gen_wfid_set( waveFnc_vec_t fnc_vec )
{
	unordered_set<WFID_e> Set{};
	for( waveform_t wf : fnc_vec )
		Set.insert( wf.wfid );
	return Set;
}

Waveform_class::Waveform_class( waveFnc_vec_t wf_vec )
	: idx_range		{ 0, (int)wf_vec.size()-1 }
{
	WF_vec 			= wf_vec;
	default_waveFnc = WF_vec[0];
	default_wfid	= WF_vec[0].wfid;
	waveform_Id_set	= gen_wfid_set( wf_vec );
	waveform_str_vec= gen_waveform_str_vec( wf_vec ) ;
	waveform_str_set= vector2uset( waveform_str_vec );

	// selfTest()
	Assert_equal(	(int)Get_id(4), (int)Get_fnc( Get_id(4) ).wfid );
	Assert_equal(	(int)Set_id( Get_id(4), 1), (int)Get_id(5) );
	WFID_e last_wfid= WF_vec.back().wfid;
	Assert_equal( 	(int)Set_id( last_wfid, 1),	(int)default_wfid );
	Assert_equal( 	(int)Set_id( default_wfid, -1),	(int)last_wfid );

};

string Waveform_class::Get_str( WFID_e id )
{
	WFID_e	wfid	= ( waveform_Id_set.contains( id ) ) ? id : default_wfid;
	return all_wf_id_str_map[wfid].str;
}

waveform_t Waveform_class::Get_fnc( WFID_e wfid )
{
	for( waveform_t fnc : WF_vec )
	{
		if( fnc.wfid == wfid )
			return fnc;
	}
	return default_waveFnc;
}

uint8_t Waveform_class::Get_idx( WFID_e wfid )
{
	return Get_fnc( wfid ).idx;
}
WFID_e Waveform_class::Get_id( string wstr )
{
	for ( wfid_map_t wf : all_wf_id_str_map )
	{
		if ( strEqual( wf.str, wstr ) )
			return wf.id;
	}
	return default_wfid;
};

WFID_e Waveform_class::Get_id( int idx )
{
	for ( waveform_t wf : WF_vec )
	{
		if ( wf.idx == idx )
			return wf.wfid;
	}
	return default_wfid;
};

WFID_e Waveform_class::Set_id( WFID_e wfid, int dir )
{
	int idx = Get_fnc( wfid ).idx + dir;
	idx 	= check_ring( idx_range, idx );
	return 	WF_vec[idx].wfid;
}

Waveform_class Waveform 	{ waveFunction_vec };
Waveform_class ADSRform 	{ adsrFunction_vec };
Waveform_class* waveform 	= &Waveform;
Waveform_class* adsrform 	= &ADSRform;
