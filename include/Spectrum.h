/*
 * spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>
#include <Frequency.h>
#include <Oscwaveform.h>


const static size_t 		spec_arr_len = 5;
typedef float 				spec_dta_ft;
typedef int					spec_dta_it;
typedef array<spec_dta_ft,	spec_arr_len> spec_arr_ft;
typedef array<phi_t		 ,	spec_arr_len> spec_arr_dt;
typedef array<spec_dta_it,	spec_arr_len> spec_arr_it;
typedef array<uint8_t,		spec_arr_len> spec_arr_8t;

class Spectrum_class :
	public virtual Logfacility_class,
	public virtual Frequency_class,
	virtual public Oscwaveform_class

{
public:
	typedef struct spec_struct
	{
		spec_arr_ft				vol		= { 1.0, 0.0, 0.0, 0.0, 0.0 } ;		// [osc, amplitude 0.0 ... 1.0 ]
		spec_arr_ft				frqadj	= { 1.0, 2.0, 3.0, 4.0, 5.0 } ;		// [osc, frequency shift... ]
		spec_arr_it				frqidx 	= { 0, 0, 0, 0, 0 };				// frq slider value
		spec_arr_it				volidx 	= { 100, 0, 0, 0, 0 };				// frq slider value
		spec_dta_ft				sum 	= 1.0;								// sum over .vol
		spec_arr_8t				wfid 	= {SINUS, SINUS, SINUS, SINUS, SINUS };// waveform wdid
		uint8_t					osc 	= osc_struct::OSCID;
		float					base	= oct_base_freq;
	} spectrum_t;


	const spectrum_t	default_spec 	= spec_struct();
	string 				className = "";
	vector<string>	spectrumType { "SPEV", "SPEF", "SPEW" };


	Spectrum_class() :
		Logfacility_class("Spectrum"),
		Frequency_class(),
		Oscwaveform_class()
	{
		className = Logfacility_class::className;
	};
	virtual ~Spectrum_class(){};

	spectrum_t 		Parse_data( vector_str_t, char oscid );
	int 			Get_waveform_id( string );
	string 			Get_waveform_str( uint );
	vector<string>	Get_waveform_vec( );
	string 			Show_this_spectrum( );
	string 			Show_spectrum_type( const string& _type, const spectrum_t& spec );
	void 			Sum( spectrum_t& );
	void 			Test_Spectrum();

private:
	spectrum_t		spectrum	= spec_struct();
	osc_roles_t		OscRole		= osc_struct();
	void assign_frq( int channel, string str  );
	void assign_vol( int channel, string str  );
	void assign_waveform( int channel, string str );

};

#endif /* INCLUDE_SPECTRUM_H_ */
