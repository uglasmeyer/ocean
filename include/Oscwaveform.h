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
 * Oscwaveform.h
 *
 *  Created on: Feb 21, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSCWAVEFORM_H_
#define OSCWAVEFORM_H_

#include <Ocean.h>

struct oscwaveform_struct
{
	enum WFID_t
	{
		SINUS = 0,
		TRIANGLE,
		SGNSIN,
		RECTANGLE,
		SAWTOOTHL,
		SAWTOOTHR,
		PMW,
		DELTA,
		NOISE,
		RANDOM,
		WFID_SIZE
	};
	const range_T<WFID_t> wfid_range{ SINUS, WFID_SIZE };
};

struct param_struct
{
	float amp		= 0.0;
	phi_t phi		= 0.0;
	phi_t dphi		= 0.0;
	phi_t maxphi 	= 0.0;
	float pmw		= 0.0;
};
typedef param_struct param_t;

typedef function<Data_t( param_t& )>	wave_function_t;
struct waveFnc_struct
{
	wave_function_t		fnc;
	string				name	= "sinus";
	phi_t 				maxphi 	= 2*numbers::pi;
	float 				width	= 1.0;
};
typedef 		waveFnc_struct 		waveFnc_t;

extern const 	vector<waveFnc_t>  	waveFunction_vec;
extern const 	vector<string> 		waveform_str_vec;
extern const 	range_T<int> 		waveform_range;

extern const 	vector<waveFnc_t>  	adsrFunction_vec;
extern const 	vector<string> 		adsrwf_str_vec;
extern const 	range_T<int> 		adsrwf_range;

extern  		int 				sgn( const float& x );



#define MODPHI( phi, maxphi )\
	( abs(phi) > (maxphi) ) ? (phi) - sgn(phi)*(maxphi) : phi;


#endif /* OSCWAVEFORM_H_ */
