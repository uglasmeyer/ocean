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
 * Instrument.h
 *
 *  Created on: Feb 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <data/Device.h>
#include <data/DataWorld.h>
#include <Spectrum.h>
#include <Oscgroup.h>
#include <Wavedisplay.h>

class Instrument_class
	: virtual public 		Logfacility_class
	, virtual public		Interface_base
	, virtual public		Device_class
{
	Oscillator*				osc						;
	Oscillator*				vco						;
	Oscillator*				fmo						;

public:
	Oscgroup_class			Oscgroup				;
//	string 					Name 					; // the name of the instrument
	Wavedisplay_class*		wd_p					;
	cstring_T<cstring_name> Instrument_name			;

							Instrument_class		( Dataworld_class* data ,
													Wavedisplay_class* wd );
	virtual 				~Instrument_class		();
;
	bool 					Set						( string );
	bool 					Set						();
	void 					SetDir					( string subdir );
	void 					Save					( string );
	void 					Update_osc_spectrum		( char oscid );
	bool	 				Connect					( Oscillator* osc,
													Oscillator* sec,
													char mode );
	bool 					Sync					( bool run );

	void 					Test_Instrument			();

private:
	dircontent_struct		instruments				;

	string					instrument_dir			;
	string					fourier_dir				;
	string 					instrument_file			;
	string					fourierBinFile			;
	int 					file_version 			= -1;
	const set<int>			supported 				{ 4, 5, 6 };
	const int				actual_version			= 6;

	void 					selfTest				();
	void	 				select					( string );
	bool 					set_new_name			( string );
	bool 					assign_adsr 			( Oscillator* osc, string_vec_t );
	bool					assign_adsr2			( const string_vec_t& arr );
	bool 					assign_adsr3			( const string_vec_t& arr );
	bool	 				assign_adsr5			( OSCID_e oscid, const string_vec_t& arr );
	void 					assign_spectrum			( const string&	type,
													const string& 	name,
													const char& 	flag );
	bool 					read_instrument			();
	void 					init_data_structure		( Oscillator*, string_vec_t);

	void 					showOscfeatures			( ostream& FILE = cout );

	int						getVersion				( fstream& File );
	bool					read_version1			( fstream& File );
	bool					assign_sds				( fstream& File );
	void 					save_connections		( ostream& FILE, Oscillator* osc );
	void 					connect					( OSCID_e oscid, OSCID_e secid, char mode );

};
#endif /* INSTRUMENT_H_ */
