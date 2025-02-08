/*
 * Instrument.h
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <Ocean.h>
#include <data/Interface.h>
#include <Spectrum.h>
#include <Osc.h>
#include <Oscgroup.h>
#include <Wavedisplay.h>
#include <String.h>
#include <System.h>



class Instrument_class: virtual public Logfacility_class
{
public:
	string 					Name 		= "";

	interface_t*  			ifd;
	Oscgroup_class			Oscgroup	{ osc_struct::INSTRID };
	Oscillator*				osc			= &Oscgroup.osc;
	Oscillator*				vco			= &Oscgroup.vco;
	Oscillator*				fmo			= &Oscgroup.fmo;
	Wavedisplay_class*		wd_p;

	const string instr_ext = file_structure().snd_type;
	Instrument_class( interface_t* ifd , Wavedisplay_class* wd );
	virtual ~Instrument_class();
;

	bool Set( string );
	void Setup( interface_t* ifd );
	void Save_Instrument( string );
	buffer_t Set_msec( uint16_t msec );
	void Update_spectrum();
	void Test_Instrument();
	void Run_osc_group();

private:
	vector<Spectrum_base::spectrum_t*> 	ifd_spectrum_vec;
	string 					Instrument_file;
	string 					Default_instrument_file;

	Frequency_class				frequency {};
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
