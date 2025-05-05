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
		virtual public Logfacility_class,
		virtual public Oscillator_base
{
	string className = "";
	Memory 			Mem_vco	{ monobuffer_size };
	Memory 			Mem_fmo	{ monobuffer_size };
	Memory 			Mem		{ monobuffer_size };

public:

	Oscillator( char role_id, char type_id );
	virtual ~Oscillator();

	void operator=(const Oscillator& osc)
    {
        this->wp 		= osc.wp;
        this->adsr		= osc.adsr;
        this->spectrum	= osc.spectrum;
    }

	void 	OSC ( const buffer_t& frame_offset );

	void 	Connect_vol_data( Oscillator* ); // connect the vco data of itr to this osc
	void 	Connect_frq_data( Oscillator* );
	void 	Reset_frq_data();
	void 	Reset_vol_data();

	void 	Connection_reset( );

	void 	Data_reset( );
	void 	Phase_reset();
	void 	Set_long_note( bool );
	void 	Reset_beat_cursor();

	Data_t* MemData_p();
	Data_t* GetData_p( const buffer_t& frame_offset );
	Data_t 	MemData( buffer_t n);
	void 	Shift_data( buffer_t n  );

	void 	Test();

private:

	spec_arr_dt	default_phase 	= { 0.0, 0.0, 0.0, 0.0, 0.0};
	spec_arr_dt	phase 			= default_phase;

	bool 		longnote	= false; // set trigger for long notes

	osc_roles_t	OscRole		= osc_struct();

	void 		apply_adsr( buffer_t frames, Data_t* data );
	void 		mem_init();
};


#endif /* INCLUDE_OSC_H_ */
