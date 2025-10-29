/*
 * Oscwaveform.h
 *
 *  Created on: Feb 21, 2025
 *      Author: sirius
 */

#ifndef OSCWAVEFORM_H_
#define OSCWAVEFORM_H_

#include <Ocean.h>

struct oscwaveform_struct
{
	enum WFID_t
	{
		SINUS,
		TRIANGLE,
		SGNSIN,
		RECTANGLE,
		SAWTOOTHL,
		SAWTOOTHR,
		PMW,
		DELTA,
		NOISE,
		RANDOM
	};
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
