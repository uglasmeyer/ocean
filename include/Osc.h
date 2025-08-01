/*
 * osc.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_OSC_H_
#define INCLUDE_OSC_H_

#include <data/Memory.h>
#include <Spectrum.h>
#include <Oscbase.h>
#include <Ocean.h>
#include <System.h>
#include <Oscwaveform.h>


class Oscillator :
	virtual public 	Logfacility_class,
	virtual public 	ADSR_class
{
	string 			className = "";

public:
	Memory			Mem_vco;
	Memory 			Mem_fmo;
	Memory 			Mem;
	buffer_t		mem_frames;

	Scanner_class	scanner;

					Oscillator			( char role_id, char type_id, buffer_t bytes );
	virtual			~Oscillator() 		= default;
	void 			operator=			( const Oscillator& osc );

	void 			OSC 				( buffer_t frame_offset );

	void 			Connect_vol_data	( Oscillator* ); // connect the vco data of itr to this osc
	void 			Connect_frq_data	( Oscillator* );
	void 			Reset_frq_data		();
	void 			Reset_vol_data		();

	void 			Connection_reset	();

	void 			Data_reset			();
	void 			Phase_reset			();
	void 			Set_long_note		( bool );
	void 			Reset_beat_cursor	();


	Data_t* 		MemData_p			();
	Data_t*			GetData_p			( const buffer_t& frame_offset );
	Data_t 			MemData				( const buffer_t& n);

	void 			Test				();

private:

	spec_arr_dt		default_phase 		= { 0.0, 0.0, 0.0, 0.0, 0.0};
	spec_arr_dt		phase 				= default_phase;

	bool 			longnote			= false; // set trigger for long notes
	osc_roles_t		OscRole				= osc_struct();

	void 			mem_init			();
	void			self_Test			();

};


#endif /* INCLUDE_OSC_H_ */
