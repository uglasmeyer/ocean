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
 * Oscgroup.h
 *
 *  Created on: Jan 22, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSCGROUP_H_
#define OSCGROUP_H_

#include <Ocean.h>
#include <notes/Notesbase.h>
#include <Osc.h>
#include <Wavedisplay.h>
#include <data/SharedDataSegment.h>



class 						Oscgroup_class :
	public virtual 			Logfacility_class,
	virtual					Frequency_class,
	public virtual 			Note_base,
	virtual					osc_struct
{
	string					className			= "";
public:
	Oscillator 				vco;
	Oscillator 				fmo;
	Oscillator 				osc;

	vector<Oscillator*>		member				{};// = { &vco, &fmo, &osc };



	OscroleId_t				oscroleId;

							Oscgroup_class		( OscroleId_t role, buffer_t bytes  );
	void 					operator=			( Oscgroup_class& oscg );
	virtual 				~Oscgroup_class		();

	void					Set_Note_Frequency	( interface_t* sds,
												  const uint8_t& idx,
												  const uint& mode );
	void 					Set_Combine_Frequency(const uint8_t& base_idx,
												  const uint8_t& idx,
												  const uint& mode );
	void 					Set_Osc_Note		( interface_t* sds,
												  const uint8_t& key,
												  const uint& duration,
												  const uint& volume,
												  const uint& mode);
	void 					Run_OSCs			( const buffer_t& offs );
	void 					Set_Duration		( const uint& duration );

	void 					Data_Reset			();
	void 					Phase_Reset			();
	void 					Connection_Reset	();
	void 					Set_Connections		( interface_t* sds );
	void 					SetWd				( Wavedisplay_class* wd );
	void 					SetScanner			( const buffer_t& maxlen );
	string 					Show_Spectrum		();
	Oscillator* 			Get_osc_by_name		( const string& name );
	OSCID_e 				Get_oscid_by_name	( const string& name );

	void 					SetSlide			( const uint8_t& value );
	void 					SetFeatures			( interface_t* sds );
	void 					SetAdsr				( interface_t* sds );
	void 					SetSpectrum			( interface_t* sds );

	void 					Show_sound_stack	();
	void 					SetInstrument		( interface_t* sds );

private:
	void 					selfTest			();


};

#endif /* OSCGROUP_H_ */
