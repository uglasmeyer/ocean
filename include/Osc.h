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

using namespace std;


class Oscillator : 	virtual public Logfacility_class,
					virtual public Oscillator_base
{
	string className = "";
public:

	Memory 			Mem_vco	{ monobuffer_size };
	Memory 			Mem_fmo	{ monobuffer_size };
	Memory 			Mem		{ monobuffer_size };


	Oscillator(  );
	virtual ~Oscillator();

	void operator=(const Oscillator& osc)
    {
        this->wp 		= osc.wp;
        this->adsr		= osc.adsr;
        this->spectrum	= osc.spectrum;
    }

	void OSC ( const buffer_t& frame_offset );
	void SetId( char role, char type );
	void Set_start_freq( float freq );
	void Connect_vco_data( Oscillator* ); // connect the vco data of itr to this osc
	void Connect_fmo_data( Oscillator* );
	void Reset_data( Oscillator* );
	void Set_long( bool );
	void Reset_cursor();

	void Test();

private:
	typedef function<Data_t(const float&, const float&)> osc_function;
	osc_function 	F;
	spec_arr_dt		phase 		= { 0.0, 0.0, 0.0, 0.0, 0.0};
	bool 			longnote	= false; // set trigger for long notes
	buffer_t 		beat_cursor = 0;

	osc_roles_t	OscRole		= osc_struct();



	void 	apply_hall( buffer_t, Data_t* );
	void 	apply_adsr( buffer_t frames, Data_t* data );
	double 	get_delta_freq( float freq );
//	void 	set_phi( double , double );
//	double 	get_phi( );
	void 	mem_init();
};


#endif /* INCLUDE_OSC_H_ */
