//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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
