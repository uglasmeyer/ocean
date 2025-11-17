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
#include <Oscwaveform.h>
#include <Table.h>


const static size_t 		SPECARR_SIZE = 5;
typedef frq_t 				spec_dta_ft;
typedef array<spec_dta_ft,	SPECARR_SIZE>
							spec_arr_ft;
typedef array<phi_t		 ,	SPECARR_SIZE>
							spec_arr_dt;
typedef array<uint8_t	 ,	SPECARR_SIZE>
							spec_arr_8t;

struct spec_struct
{	// SDS  related
	spec_arr_ft					vol		= { 1.0, 0.0, 0.0, 0.0, 0.0 } ;		// [osc, amplitude 0.0 ... 1.0 ]
	spec_arr_ft					frqadj	= { 1.0, 2.0, 3.0, 4.0, 5.0 } ;		// [osc, frequency shift... ]
	spec_arr_8t					frqidx 	= { A3, 1, 1, 1, 1 };				// frq slider value
	spec_arr_8t					volidx 	= { 100, 0, 0, 0, 0 };				// frq slider value
	spec_dta_ft					sum 	= 1.0;								// sum over .vol
	spec_arr_8t					wfid 	= { 0,0,0,0,0 };// waveform wdid
	OSCID_e						osc 	= OSCID;
	bool						adsr	= false;
} ;

class Spectrum_class :
	public 	virtual 			Logfacility_class,
	public	virtual 			oscwaveform_struct,
	public	virtual 			Frequency_class,
	public 	virtual				osc_struct

{
	string 						className 			= "";

public:

	spec_struct					test_spectrum		=
	{
								.vol				= { 1.0, 0.0, 0.0, 0.0, 0.0 } ,
								.frqadj				= { 1.0, 2.0, 3.0, 4.0, 5.0 },
								.frqidx 			= { A3, C0, C0, C0, C0 },
								.volidx 			= { 100, 0, 0, 0, 0 },
								.sum 				= 1.0,
								.wfid 				= { SINUS, 0,0,0,0},
								.osc 				= OSCID,
								.adsr				= false
	};

	spec_struct					Parse_data			( vector_str_t arr );
	int 						Get_waveform_id		( string );
	string 						Get_waveform_str	( int );
	vector<string>				Get_waveform_vec	( );
	spec_struct					Get_spectrum		();
	string 						Show_spectrum		( const spec_struct spec );
	string 						Show_spectrum_type	( const int& _type, const spec_struct& spec );
	void						Show_spectrum_table	(fstream* f,
													const spec_struct& spec,
													bool header = true );
	Table_class*				Get_spectrum_table	();
	void 						Sum					( spec_struct& );
	char 						Type_flag			( const string& type_str );
	OSCID_e						Osc_TypeId			( const string& type_str );


	void 						Test_Spectrum		();

								Spectrum_class		();
	virtual 					~Spectrum_class		() = default;

private:
	const array<int		,3> 	spectrumNum 		= {  SPEV,   SPEF,   SPEW  };
	const array<string	,3> 	spectrumTag 		= { "SPEV", "SPEF", "SPEW" };
	spec_struct					spectrumTmp			= spec_struct();

	void 						assign_frq			( int channel, string str  );
	void 						assign_vol			( int channel, string str  );
	void 						assign_waveform		( int channel, string str );
	bool 						adsr_type			( const string& type_str );
	Table_class 				Table				{ defaultopt };

};

typedef spec_struct				spectrum_t;
const spectrum_t 				default_spectrum	= spec_struct();

#endif /* INCLUDE_SPECTRUM_H_ */
