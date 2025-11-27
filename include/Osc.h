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

					Oscillator			( RoleId_t role_id, OSCID_e type_id, buffer_t bytes );
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
