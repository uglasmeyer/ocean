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
#include <Exit.h>
#include <Frequency.h>


enum waveformId_t
{
	SINUS0,
	SINUS1,
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

const static size_t 	spec_arr_len = 4;
typedef float 			spec_dta_ft;
typedef uint			spec_dta_ut;
typedef array<spec_dta_ft,	spec_arr_len> spec_arr_ft;
typedef array<phi_t		,	spec_arr_len> spec_arr_dt;
typedef array<spec_dta_ut,	spec_arr_len> spec_arr_ut;


class Spectrum_base : public virtual Logfacility_class
{
public:

	typedef struct spec_struct
	{
		spec_arr_ft				vol	= { 0.0 } ;		// [osc, amplitude ... ]
		spec_arr_ft				frq	= { 0.0 } ;		// [osc, frequency shift... ]
		spec_arr_ut				idx = { 0 };
		spec_dta_ft				sum = 0;
		spec_arr_dt				phi {0.0};
		uint8_t	 				id 	= SINUS1; // waveform id
		uint8_t					osc = osc_struct::OSCID;
		float					base= oct_base_freq;
	} spectrum_t;

	const vector<string> waveform_str_vec
	{ // consistence with docu Table Standard Waveforms
		"sinus",		// 0
		"Sinus",		// 1
		"triangle",		// 6
		"sgnSin",		// 2
		"rectangle",	// 3
		"sawtooth",		// 4
		"Sawtooth",		// 5
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
	string 			Show_spectrum( const string& _type, const spectrum_t& spec );
	void 			Sum( spectrum_t& );

private:
	spectrum_t		spectrum	= spec_struct();
	osc_roles_t		OscRole		= osc_struct();

};

#endif /* INCLUDE_SPECTRUM_H_ */
