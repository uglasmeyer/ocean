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
 * oscwaveform.cpp
 *
 *  Created on: Feb 21, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Oscwaveform.h>
#include <String.h>


random_device 	rd;
mt19937 		engine(rd());
uniform_real_distribution<> distrib( -1, 1 );


// https://stackoverflow.com/questions/6339970/c-using-function-as-parameter

int sgn( const float& x )
{
	return ( x >= 0 ) ? 1 : -1;
}

float psgn( const float& x )
{
	return ( x > 0 ) ? x : 0 ;
}




float Rnd( param_t& p )
{ 	// provides values between -amp .. amp
	float rint = p.amp * distrib( engine );
	return rint;
}


Data_t 	Delta( param_t& p )
{
	float f1 = fmod(p.phi, p.phi + p.dphi);
	return ( f1 < p.phi ) ? p.amp : 0.0;
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

Data_t Const( param_t& p )
{
	return p.maxphi * p.amp ;
}
Data_t Sinus( param_t& p )
{
	return p.amp * (  sin( p.phi ) );
}
Data_t SignSin( param_t& p )
{
	return ( p.amp * psgn(  sin ( p.phi ) ));
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
	return ( p.amp* (1.0 - modulo( p.phi,1 )));
}
Data_t Pmw( param_t& p )
{
	phi_t phi2 	= p.phi * p.pmw ;
	return p.amp *	abs(round(modulo( p.phi , p.maxphi))) *
					abs(round(modulo(phi2	, p.maxphi))) ;
	//	return abs(round(modulo(phi , maxphi)));
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

const vector<waveFnc_struct> waveFunction_vec
{
//     fnc         visible name   maxphi  width

	{  Sinus	, "sinus"		, 2*pi 	, 2.0 },
	{  Triangle	, "triangle"	, 2		, 2.0 },
	{  SignSin	, "signsin"		, 2*pi 	, 1.0 },
	{  Rectangle, "rectangle"	, 1 	, 2.0 },
	{  SawTooth	, "sawtoothL"	, 1		, 1.0 },
	{  Sawtooth	, "sawtoothR"	, 1		, 1.0 },
	{  Pmw		, "PWM"			, 1		, 1.0 },
	{  Delta	, "delta"		, 1		, 1.0 },
	{  Rnd		, "noise"		, 1		, 2.0 },
	{  Rnd_step	, "random"		, 1		, 2.0 }
};


constexpr vector<string>   gen_waveform_str_vec ( vector<waveFnc_struct> fnc_vec )
{
	vector<string> vec{};
	for ( waveFnc_struct wf : fnc_vec )
		vec.push_back( wf.name  );
	return vec;
};

const vector<string> waveform_str_vec { gen_waveform_str_vec( waveFunction_vec ) };
const range_T<int> waveform_range{ 0, (int) waveFunction_vec.size() -1 };

const vector<waveFnc_struct> adsrFunction_vec
{
	{  Const	, "constant"	, 1 	},
	{  Sinus	, "sinus"		, 2*pi 	},
	{  Triangle	, "triangle"	, 2		},
	{  SignSin	, "signsin"		, 2*pi 	},
	{  Rectangle, "rectangle"	, 1 	},
	{  SawTooth	, "sawtoothL"	, 1		},
	{  Sawtooth	, "sawtoothR"	, 1		},
	{  Pmw		, "PMW"			, 1		},
	{  Rnd_step	, "random"		, 1		}
};


const vector<string>adsrwf_str_vec 	{ gen_waveform_str_vec( adsrFunction_vec ) };
const range_T<int> 	adsrwf_range	{ 0, (int) adsrFunction_vec.size() -1 };

