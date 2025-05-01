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
#include <Table.h>

class Instrument_class:
		virtual public Logfacility_class

{
public:
	string 					Name 		= "";

	interface_t*  			sds			= nullptr;
	Oscgroup_class			Oscgroup	{ osc_struct::INSTRID };
	Oscillator*				osc			= &Oscgroup.osc;
	Oscillator*				vco			= &Oscgroup.vco;
	Oscillator*				fmo			= &Oscgroup.fmo;
	Wavedisplay_class*		wd_p		= nullptr;

	Instrument_class( interface_t* ifd , Wavedisplay_class* wd );
	virtual ~Instrument_class();
;

	bool Set( string );
	void Save_Instrument( string );
	void Set_msec( buffer_t frames );
	void Update_spectrum();
	void Update_sds_connect( );
	void Connect( Oscillator* osc, Oscillator* sec, char mode );

	void Test_Instrument();

private:
	vector<spectrum_t*>		ifd_spectrum_vec		{};
	string 					Instrument_file			= "";
	string 					Default_instrument_file = "";
	int 					file_version 			= -1;
	set<int>				supported 				{ 0, 1, 2 };
	const string 			instr_ext 				= file_structure().snd_type;

	void 	set_name( string );
	void 	set_new_name( string );
	bool 	assign_adsr ( vector_str_t );
	bool	assign_adsr2( const vector_str_t& arr );

	bool 	read_instrument( );
	bool 	init_connections(  );
	bool 	connect_by_name(string, string, char );
	void 	init_data_structure( Oscillator*, vector_str_t);
	void 	Update_sds();
	void 	reuse_GUI_Data();
	int 	getVersion( fstream& File );
	bool	read_version1( fstream* File );
	bool	read_version2( fstream* File );
	void 	save_features( fstream& FILE );
	void 	save_connections( fstream& FILE, Oscillator* osc );


};

#endif /* INSTRUMENT_H_ */
