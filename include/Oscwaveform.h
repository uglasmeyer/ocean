/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

#include <math.h>
#include <Ocean.h>


enum WFID_e : uint8_t
{
	SINUS = 0,
	COSIN,
	TRIANGLE,
	SGNSIN,
	RECTANGLE,
	SAWTOOTHL,
	SAWTOOTHR,
	TRAPECE,
	PMW,
	DELTA,
	NOISE,
	RANDOM,
	FOURIER,
	ZERO,
	ADSR,		// non selectable default adsr main waveform parametrized by attack/decay
	WFID_SIZE	// NIL
};
const range_T<int> wfid_range { 0, int(WFID_SIZE) };

typedef struct param_struct
{
	float 	amp		= 0.0;	// f = amp * f( phi )
	phi_t 	phi		= 0.0;
	phi_t 	dphi	= 0.0;	// dphi = w dt
	phi_t 	maxphi 	= 0.0;	// 0 < phi < maxphi, see also MODPHI
	float 	pmw		= 0.0;	// is available for pmw, sgnsin, trapez
} param_t;

typedef function<Data_t( param_t& )>wave_function_t;
struct waveFnc_struct
{
	int					idx;
	wave_function_t		fnc;
	phi_t 				maxphi 	= 2*M_PI;
	float 				width	= 1.0;
	WFID_e				wfid	= SINUS;
};
typedef waveFnc_struct 				waveform_t;
typedef vector<waveform_t>			waveFnc_vec_t;

extern const 	waveFnc_vec_t		waveFunction_vec;
extern const 	waveFnc_vec_t		adsrFunction_vec;
#define MODPHI( phi, maxphi )\
	( abs(phi) > maxphi ) ? ( phi - copysign(1, phi)*maxphi ) : phi;


/**************************************************
 * Waveform_class
 *************************************************/
class Waveform_class
{
	typedef unordered_set<WFID_e>
					waveform_set_t	;

	waveFnc_vec_t	WF_vec			;
	waveform_t 		default_waveFnc ;
	range_T<int> 	idx_range		;

public:
	waveform_set_t	waveform_Id_set	;
	WFID_e 			default_wfid	;
	vector<string> 	waveform_str_vec;
	uset_t			waveform_str_set;

					Waveform_class( waveFnc_vec_t wf_vec );
	virtual 		~Waveform_class() = default;
	waveform_t 		Get_fnc( WFID_e wfid );
	WFID_e 			Get_id( string wstr );
	WFID_e 			Get_id( int idx );
	uint8_t 		Get_idx( WFID_e wfid );
	string 			Get_str( WFID_e wfid );
	WFID_e			Set_id( WFID_e wfid, int dir );
};

extern Waveform_class* waveform;
extern Waveform_class* adsrform;

#endif /* OSCWAVEFORM_H_ */
