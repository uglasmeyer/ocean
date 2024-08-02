/*
 * spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_

#include <synthesizer.h>


enum { MAINID, VCOID, FMOID, NOTESID, TESTID, OTHERID };

const static size_t spec_dta_len = 8;
typedef array<int,spec_dta_len> spec_dta_t;

typedef struct spec_struct
{
	spec_dta_t				dta	= { 100,0,0,0,0,0,0,0 } ; 	// [osc, amplitude ... ]
	int						sum = 100; // max of the array content
	char	 				id 	= 1;
	char					osc = MAINID;
} spec_struct_t;

class Spectrum_base : public virtual Logfacility_class
{
public:

	const vector<string> osc_type_vec { "MAIN", "VCO", "FMO", "NOTES", "Test", "NULL" };

	Spectrum_base() : Logfacility_class("Spectrum base") {};
	~Spectrum_base(){};


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

	const spec_struct_t			default_spec 	= spec_struct();

	spec_struct_t 	Parse_data( vector_str_t, char );
	void 			Set_spectrum( uint8_t, int, int );
	int 			Get_waveform_id( string );
	string 			Get_waveform_str( uint );
	vector<string>	Get_waveform_vec( );
	string 			Show_this_spectrum( spec_struct_t );
	void 			Sum( spec_struct_t& );

private:
	spec_struct_t				spectrum		= spec_struct();


};

class Spectrum_class : public virtual Logfacility_class, Spectrum_base
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
	string 			Show_this_spectrum( spec_struct_t );

	void 			Test();

private:


	typedef list<spec_struct_t>	spec_list_t;
	spec_list_t			 		spectrum_list	{};
	spec_list_t::iterator 		list_itr 		= spectrum_list.begin();
	spec_struct_t 				spec_buffer 	= spec_struct();

	string 						Name			= "";
	string 						Spectrum_file	= "";
	string						osc_type 		= "";

	const spec_struct_t			default_spec 	= spec_struct();

	bool 						check_size( spec_list_t  );
	void 						parse_data( string );
	void						reset();
};



#endif /* INCLUDE_SPECTRUM_H_ */
