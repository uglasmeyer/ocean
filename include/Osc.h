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
