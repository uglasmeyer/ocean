/*
 * spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>

enum waveformId_t
{
	SINUS0,
	SINUS1,
	SGNSIN,
	RECTANGLE,
	SAWTOOTHL,
	SAWTOOTHR,
	TRIANGLE,
	PMW,
	DELTA,
	NOISE,
	RANDOM
};

const static size_t 	spec_arr_len = 4;
typedef float 			spec_dta_t;
typedef array<spec_dta_t,	spec_arr_len> spec_arr_t;
typedef array<phi_t		,	spec_arr_len> spec_darr_t;


class Spectrum_base : public virtual Logfacility_class
{
public:

	typedef struct spec_struct
	{
		spec_arr_t				vol	= { 0.0 } ;
		// [osc, amplitude ... ]
		spec_arr_t				frq	= { 0.0 } ;
		// [osc, frequency shift... ]
		spec_dta_t				sum = 0;
		spec_darr_t				phi {0.0};
		uint8_t	 				id 	= SINUS1; // waveform id
		uint8_t					osc = osc_struct::OSCID;
		float					base= oct_base_freq;
	} spectrum_t;

	const vector<string> waveform_str_vec
	{ // consistence with docu Table Standard Waveforms
		"sinus",		// 0
		"Sinus",		// 1
		"sgnSin",		// 2
		"rectangle",	// 3
		"sawtooth",		// 4
		"Sawtooth",		// 5
		"triangle",		// 6
		"PMW",			// 7
		"delta",		// 8
		"noise",		// 9
		"random"		// 10
	};

	const spectrum_t			default_spec 	= spec_struct();


	Spectrum_base() : Logfacility_class("Spectrum base") {};
	~Spectrum_base(){};

	spectrum_t 		Parse_data( vector_str_t, char oscid, char _type );
	void 			Set_spectrum( uint8_t, spectrum_t );
	int 			Get_waveform_id( string );
	string 			Get_waveform_str( uint );
	vector<string>	Get_waveform_vec( );
	string 			Show_this_spectrum( spectrum_t );
	void 			Sum( spectrum_t& );

private:
	spectrum_t		spectrum	= spec_struct();
	osc_roles_t		OscRole		= osc_struct();

};

#endif /* INCLUDE_SPECTRUM_H_ */
