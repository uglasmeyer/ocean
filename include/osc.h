/*
 * osc.h
 *
 *  Created on: Dec 30, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_OSC_H_
#define INCLUDE_OSC_H_

#include <synthesizer.h>
#include <synthmem.h>
#include <Spectrum.h>
#include <GUIinterface.h>
#include <Oscbase.h>

using namespace std;


class Oscillator : virtual public Logfacility_class, virtual public Oscillator_base
{

public:

	Memory 			Mem_vco	{ monobuffer_size };
	Memory 			Mem_fmo	{ monobuffer_size };
	Memory 			Mem		{ monobuffer_size };


	Oscillator( string name ) : Logfacility_class( name ), Oscillator_base()
	{
		mem_init();
		Mem_vco.Info( name );
		Mem_fmo.Info( name );
		Mem.Info( name );
		osc_type = name;

	};
	virtual ~Oscillator(){};


	void OSC ( buffer_t );
	void set_start_freq( float freq );
	void connect_vco_data( Oscillator* ); // connect the vco data of itr to this osc
	void connect_fmo_data( Oscillator* );
	void reset_data( Oscillator* );
	void set_long( bool );
	void test();

private:
	typedef function<Data_t(const float&, const float&)> osc_function;
	osc_function 	F;
	double 			phase 		= 0;
	double 			start_freq 	= 0;
	float 			fmophase 	= 0;
	bool 			longnote	= false; // set trigger for long notes
	buffer_t		decay_shift = 0;

	void hall_effect( adsr_struc_t, buffer_t, Data_t* );
	void Adsr(adsr_struc_t , buffer_t , Data_t*);
	double get_delta_freq( double freq );
	void set_phi( double phase );
	double get_phi( );
	void mem_init();

};


#endif /* INCLUDE_OSC_H_ */
