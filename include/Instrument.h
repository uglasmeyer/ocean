/*
 * Instrument.h
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <data/Interface.h>
#include <Spectrum.h>
#include <Osc.h>
#include <Ocean.h>
#include <String.h>
#include <System.h>



class Instrument_class: virtual public Logfacility_class
{
public:
	string 					Name 		= "";

	Oscillator 				main{ MAINID};
	Oscillator 				vco	{ VCOID };
	Oscillator 				fmo	{ FMOID };
	vector<Oscillator*>		osc_vector { &vco, &fmo, &main };
	interface_t*  				ifd;

	Instrument_class( interface_t*  );
	virtual ~Instrument_class();
;

	bool Set( string );
	void Setup( interface_t* ifd );
	void Save_Instrument( string );
	void Update_spectrum();
	void Test_Instrument();
	void Run_osc_group();

private:
	vector<Spectrum_base::spectrum_t*> 	ifd_spectrum_vec;
	string 					Instrument_file;
	string 					Default_instrument_file;

	void set_name( string );
	void set_new_name( string );
	bool assign_adsr 	( vector_str_t );
	bool read_instrument( );
	bool init_connections( );
	Oscillator* get_osc_by_name( string );
	bool connect(string, string, string );
	void init_data_structure( Oscillator*, vector_str_t);
	void setup_GUI_Data();
	void reuse_GUI_Data();
	void show_sound_stack();


};

#endif /* INSTRUMENT_H_ */
