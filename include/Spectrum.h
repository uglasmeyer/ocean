/*
 * spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_

#include <synthesizer.h>

const static size_t spec_dta_len = 8;

typedef struct spec_struct
{
	array<int,spec_dta_len> dta	= { 100,0,0,0,0,0,0,0 } ; 	// [osc, amplitude ... ]
	int						sum = 100; // max of the array content
	string					osc = "Sinus"; // assignment to an oszillator
	uint8_t 				id 	= 1;
	void Sum( )
	{
		sum = 0;
		for ( size_t i = 0; i < spec_dta_len ; i++ )
			sum = sum + dta[i];
		if ( sum == 0 )
		{
			sum = 1;
			dta[0] = 1;
		}
	}

} spec_struct_t;



class Spectrum_class : public virtual Logfacility_class
{
public:
	Spectrum_class( ) ;
	~Spectrum_class(){};

	bool 			Read( string );
	bool 			Read( uint );
	void 			Set_Spec_Name( string );
	spec_struct_t 	Get_spectrum( uint8_t );
	void 			Set_spectrum( uint8_t, int, int );
	int 			Get_waveform_id( string );
	string 			Get_waveform_str( uint );
	vector<string>	Get_waveform_vec( );
	string			Show_spectrum(  );
	void 			Test();

private:

	const vector<string> waveform_str_vec
	{ // consistence with docu Table Standard Waveforms
		"sinus",		// 0
		"Sinus",		// 1
		"sgnSin",		// 2
		"rectangle",	// 3
		"sawtooth",		// 4
		"Sawtooth",		// 5
		"triangle",		// 6
		"PMW",			// 7
		"delta",		// 8
		"noise",		// 9
		"random"		// 10
	};

	typedef list<spec_struct_t>	spec_list_t;
	spec_list_t			 		spectrum_list	{};
	spec_list_t::iterator 		list_itr 		= spectrum_list.begin();

	string 						Name			= "";
	string 						Spectrum_file	= "";
	string						osc_type 		= "";

	spec_struct_t 				spec_buffer 	= spec_struct();
	const spec_struct_t			default_spec 	= spec_struct();

	bool 						check_size( spec_list_t  );
	void 						parse_data( string );
	void						reset();
};



#endif /* INCLUDE_SPECTRUM_H_ */
