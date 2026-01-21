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
 * spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_



enum  { SPEV, SPEF, SPEW };


#include <Dynamic.h>
#include <Ocean.h>
#include <Logfacility.h>
#include <Frequency.h>
#include <Table.h>


const static size_t 		SPECARR_SIZE = 5;
typedef frq_t 				spec_dta_ft;
typedef array<spec_dta_ft,	SPECARR_SIZE>
							spec_arr_ft;
typedef array<phi_t		 ,	SPECARR_SIZE>
							spec_arr_dt;
typedef array<uint8_t	 ,	SPECARR_SIZE>
							spec_arr_8t;


struct spectrum_data
{	// SDS  related
	spec_arr_ft					vol		= { 1.0, 0.0, 0.0, 0.0, 0.0 } ;		// [osc, amplitude 0.0 ... 1.0 ]
	spec_arr_ft					frqadj	= { 1.0, 2.0, 3.0, 4.0, 5.0 } ;		// [osc, frequency shift... ]
	spec_arr_8t					frqidx 	= { A3, 0, 0, 0, 0 };				// frq slider value
	spec_arr_8t					volidx 	= { 100, 0, 0, 0, 0 };				// frq slider value
	spec_dta_ft					sum 	= 1.0;								// sum over .vol
	spec_arr_8t					wfid 	= { 0,0,0,0,0 };// waveform wdid
	OSCID_e						osc 	= OSCID;
	bool						adsr	= false;
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


	spectrum_data				Parse_data			( vector_str_t arr );
	int 						Get_waveform_id		( string );
	string 						Get_waveform_str	( int );
	vector<string>				Get_waveform_vec	( );
	spectrum_data				Get_spectrum		();
	string 						Show_spectrum		( const spectrum_data spec );
	string 						Show_spectrum_type	( const int& _type, const spectrum_data& spec );
	void						Show_spectrum_table	( ostream& f,
													const spectrum_data& spec,
													bool header = true );
	Table_class*				Get_spectrum_table	();
	void 						Sum					( spectrum_data& );
	char 						Type_flag			( const string& type_str );
	OSCID_e						Osc_TypeId			( const string& type_str );


	void 						Test_Spectrum		();

								Spectrum_class		();
	virtual 					~Spectrum_class		() = default;

private:
	const array<int		,3> 	spectrumNum 		= {  SPEV,   SPEF,   SPEW  };
	const array<string	,3> 	spectrumTag 		= { "SPEV", "SPEF", "SPEW" };
	spectrum_data				spectrumTmp			= spectrum_data();

	void 						assign_frq			( int channel, string str  );
	void 						assign_vol			( int channel, string str  );
	void 						assign_waveform		( int channel, string str );
	bool 						adsr_type			( const string& type_str );
	Table_class 				Table				{ defaultopt };

};



#endif /* INCLUDE_SPECTRUM_H_ */
