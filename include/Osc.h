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
 * osc.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_OSC_H_
#define INCLUDE_OSC_H_

#include <Spectrum.h>
#include <Adsr.h>
#include <data/Memory.h>
#include <Ocean.h>
#include <System.h>
#include <Oscwaveform.h>


class Oscillator :
	virtual public 	Logfacility_class,
	virtual public 	Spectrum_class,
	virtual public 	Oscillator_base,
	virtual public 	ADSR_class
{
	string 			className = "";

public:
	Heap_Memory			Mem_vco;
	Heap_Memory 			Mem_fmo;
	Heap_Memory 			Mem;
	buffer_t		mem_frames;

	Scanner_class	scanner;


					Oscillator			( OscroleId_t role_id, OSCID_e type_id, buffer_t bytes );
	virtual			~Oscillator() 		= default;
	void 			operator=			( Oscillator& osc );

	void 			OSC 				( buffer_t frame_offset );

	void 			Connect_vol_data	( Oscillator* ); // connect the vco data of itr to this osc
	void 			Connect_frq_data	( Oscillator* );
	void 			Set_connection		( Oscillator* frq, Oscillator* vol );
	void 			Reset_frq_data		();
	void 			Reset_vol_data		();

	void 			Connection_reset	();

	void 			Data_reset			();
	void 			Phase_reset			();
	void 			Set_long_note		( bool );
	void			Setwp_frames		( uint16_t );

	void 			Reset_beat_cursor	();

	Data_t* 		MemData_p			();
	Data_t*			GetData_p			( const buffer_t& frame_offset );
	Data_t 			MemData				( const buffer_t& n);

	void 			Test				();

private:



	bool 			longnote			= false; // set trigger for long notes
	const float		fmo_scale			= 0.003;//0.001;
	void 			mem_init			();
	void			self_Test			();

};


#endif /* INCLUDE_OSC_H_ */
