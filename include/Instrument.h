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
 * Instrument.h
 *
 *  Created on: Feb 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <data/Device.h>
#include <Ocean.h>
#include <data/Interface.h>
#include <Spectrum.h>
#include <Osc.h>
#include <Oscgroup.h>
#include <Wavedisplay.h>
#include <String.h>
#include <System.h>
#include <Table.h>

class Instrument_class:
	virtual public 		Logfacility_class,
	virtual public		Device_class,
	virtual				osc_struct
{
	string 				className				= "";
public:
	string 				Name 					= "";

	interface_t*  		sds						= nullptr;
	Oscgroup_class		Oscgroup				{ INSTRROLE, monobuffer_bytes };
	Oscillator*			osc						= &Oscgroup.osc;
	Oscillator*			vco						= &Oscgroup.vco;
	Oscillator*			fmo						= &Oscgroup.fmo;
	Wavedisplay_class*	wd_p					= nullptr;
	file_structure*		fs						;

						Instrument_class		( interface_t* ifd ,
												Wavedisplay_class* wd,
												file_structure* fs);
	virtual 			~Instrument_class		();
;
	bool 				Set						( string );
	void 				Save			( string );
	void				Set_adsr				( char osdid );
	void 				Update_osc_spectrum		( char oscid );
	bool 				Connect					( Oscillator* osc,
												Oscillator* sec,
												char mode );

	void 				Test_Instrument			();

private:
	string 				Instrument_file			= "";
	string 				Default_instrument_file = "";//fs::instrumentdir + fs.default_snd;
	string 				instr_ext 				= "";//fs.snd_type;
	int 				file_version 			= -1;
	set<int>			supported 				{ 0, 1, 2, 3, 4 };
	int					actual_version			= 4;

	void 				selfTest				();
	void 				set_name				( string );
	void 				set_new_name			( string );
	bool 				assign_adsr 			( vector_str_t );
	bool				assign_adsr2			( const vector_str_t& arr );
	bool 				assign_adsr3			( const vector_str_t& arr );
	void 				assign_spectrum			( const string&	type,
												const string& 	name,
												const char& 	flag );
	bool 				read_instrument			();
	void 				init_data_structure		( Oscillator*, vector_str_t);
	void 				update_sds				();

	void 				showOscfeatures			( ostream& FILE = cout );
	int 				getVersion				( fstream& File );
	bool				read_version1			( fstream& File );
	bool				read_version2			( fstream& File );
	void 				save_features			( ostream& FILE );
	void 				save_connections		( ostream& FILE, Oscillator* osc );
};
#endif /* INSTRUMENT_H_ */
