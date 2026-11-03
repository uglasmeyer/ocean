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
 * Spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_

namespace spectrum_ns
{
	enum SPEC_e : uint8_t
	{
		FREQ,	// Spectrum of the Osc
		ADSR	// Spectrum of the Adsr
	};
	enum TYPE_e : uint8_t
	{
		SPEV,
		SPEF,
		SPEW
	};
	const array<uint8_t	,3> 	spectrumNum	= {  SPEV, SPEF, SPEW  };
	const array<string	,3> 	spectrumTag = { "SPEV", "SPEF", "SPEW" };
}

#include <Dynamic.h>
#include <Logfacility.h>
#include <Table.h>
#include <Oscwaveform.h>

template< typename Item >
constexpr array<Item, OSCIDSIZE > default_oscgroup_arr_T( Item dflt )
{
	array<Item, OSCIDSIZE > arr{};
	for ( char id = 0; id < OSCIDSIZE; id++ )
	{
		arr[id] = dflt;
	}
	return arr;
};


const vector<uint8_t>		dialog_channel_vec	= Iota_T<uint8_t>( 1, 4 );
// consider that;
// Iota_T<>( start, length )
// dialog_channel vector represents the number of sliders,
// that are available in the spectrum dialog of the GUI
// channel 0 is the base frequency channel, the spectrum dialog does not change it
// adsr base waveform channel 0 is adsr or fourier
// the adsr does not support high frequency channels

const vector<uint8_t>		osc_channel_vec		= Iota_T<uint8_t>( 0, SPEC_CHANNELS );
const range_T<int8_t>		channel_range		{ 1, SPEC_CHANNELS };

typedef array<phi_t		 ,	SPEC_CHANNELS>
							spec_arr_dt;
typedef array<uint8_t	 ,	SPEC_CHANNELS>
							spec_arr_8t;
typedef array<WFID_e	 ,	SPEC_CHANNELS>
							spec_arr_wt;
const spec_arr_dt			zero_phase	{ 0.0 };
struct spectrum_data
{	// SDS  related
	spec_arr_8t					volidx  { 100	, 0, 0, 0, 0 };			// frq slider value
	spec_arr_8t					frqidx 	{ A3	, 0, 0, 0, 0 };			// vol slider value
	spec_arr_wt					wfid 	{ SINUS };						// waveform id
	spec_arr_dt					phi		= zero_phase;
	OSCID_e						osc 	= OSCID;
	spectrum_ns::SPEC_e			type	= spectrum_ns::FREQ;
} ;

typedef spectrum_data			spectrum_t;
const spectrum_t 				default_spectrum	= spectrum_data();

/**************************************************
 * Spectrum_class
 *************************************************/

class Spectrum_class :
	public	virtual 			Frequency_class
{
public:
	const spectrum_t			TestSpectrum =
	{
		.volidx  	{ 100, 25, 25, 0, 0 },
		.frqidx 	{ A3, 0, 0, 0, 0 },
		.wfid 		{ SINUS },
		.phi		= zero_phase,
		.osc 		= OSCID,
		.type		= spectrum_ns::FREQ
	};

	Waveform_class*				Waveform;

	spectrum_t					Parse_data			( string_vec_t arr );
	vector<string>				Get_waveform_vec	( );
	spectrum_t					Get_spectrum		();
	string 						Show_spectrum		( const spectrum_t spec );
	string 						Show_spectrum_type	( const int& _type, const spectrum_t& spec );
	void						Show_spectrum_table	( ostream& f,
													const spectrum_t& spec,
													bool header = true );
	Table_class*				Get_spectrum_table	();
	float 						VolFactor			( spec_arr_8t& );
	char 						Type_flag			( const string& type_str );
	void 						Test_Spectrum		();
								Spectrum_class		( spectrum_ns::SPEC_e _type );
	virtual 					~Spectrum_class		() = default;

private:
	spectrum_t					spectrumTmp			= spectrum_data();
	Table_class 				Table				{ defaultopt };

	OSCID_e						osc_TypeId			( const string& type_str );
	void 						assign_frq			( string_vec_t arr  );
	void 						assign_vol			( int channel, string str  );
	spectrum_ns::SPEC_e 		spec_type			( const string& type_str );
	template< typename field_T, class range_T >
	void 						assign_values		( string_vec_t arr,
													array<field_T, SPEC_CHANNELS>& field,
													range_T range  );
	template< class field_T, class range_T >
	void 						assign_value		( string nr_str,
													field_T& field,
													range_T range  );
};

#endif /* INCLUDE_SPECTRUM_H_ */
