/*
 * spectrum.h
 *
 *  Created on: Dec 26, 2023
 *      Author: sirius
 */

#ifndef INCLUDE_SPECTRUM_H_
#define INCLUDE_SPECTRUM_H_

#include <Dynamic.h>
#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>
#include <Frequency.h>
#include <Oscwaveform.h>
#include <Table.h>

struct oscwaveform_struct
{
	enum waveformId
	{
		SINUS,
		TRIANGLE,
		SGNSIN,
		RECTANGLE,
		SAWTOOTHL,
		SAWTOOTHR,
		PMW,
		DELTA,
		NOISE,
		RANDOM
	};
};


enum  { SPEV, SPEF, SPEW };

const static size_t 		spec_arr_len = 5;
typedef frq_t 				spec_dta_ft;
typedef int					spec_dta_it;
typedef array<spec_dta_ft,	spec_arr_len> spec_arr_ft;
typedef array<phi_t		 ,	spec_arr_len> spec_arr_dt;
typedef array<spec_dta_it,	spec_arr_len> spec_arr_it;
typedef array<uint8_t,		spec_arr_len> spec_arr_8t;

class Spectrum_class :
	public 	virtual Logfacility_class,
	public 	virtual Frequency_class,
			virtual oscwaveform_struct
{
public:
	const array<int		,3> spectrumNum = {  SPEV,   SPEF,   SPEW  };
	const array<string	,3> spectrumTag = { "SPEV", "SPEF", "SPEW" };
	struct spec_struct
	{	// SDS  related
		spec_arr_ft				vol		= { 1.0, 0.0, 0.0, 0.0, 0.0 } ;		// [osc, amplitude 0.0 ... 1.0 ]
		spec_arr_ft				frqadj	= { 1.0, 2.0, 3.0, 4.0, 5.0 } ;		// [osc, frequency shift... ]
		spec_arr_8t				frqidx 	= { A3, 1, 1, 1, 1 };				// frq slider value
		spec_arr_8t				volidx 	= { 100, 0, 0, 0, 0 };				// frq slider value
		spec_dta_ft				sum 	= vol[0];							// sum over .vol
		spec_arr_8t				wfid 	= {SINUS, SINUS, SINUS, SINUS, SINUS };// waveform wdid
		uint8_t					osc 	= osc_struct::OSCID;
		bool					adsr	= false;
	} ;


	const spec_struct			default_spec 	= spec_struct();
	string 						className 		= "";

	spec_struct					Parse_data( vector_str_t arr,
											const char& oscid,
											const int& _type,
											bool		adsr = false );
	int 						Get_waveform_id( string );
	string 						Get_waveform_str( int );
	vector<string>				Get_waveform_vec( );
	spec_struct					Get_spectrum();
	string 						Show_this_spectrum( );
	string 						Show_spectrum_type( const int& _type, const spec_struct& spec );
	void						Save_spectrum_table(fstream* f, const spec_struct& spec );
	void 						Sum( spec_struct& );
	char 						Type_flag( const string& type_str );
	void 						Test_Spectrum();

								Spectrum_class();
								Spectrum_class( char osc_type, bool adsr_type );
	virtual 					~Spectrum_class() = default;

private:
	spec_struct					spectrum	= spec_struct();
	osc_roles_t					OscRole		= osc_struct();
	void 						assign_frq( int channel, string str  );
	void 						assign_vol( int channel, string str  );
	void 						assign_waveform( int channel, string str );

};

typedef Spectrum_class::spec_struct 	spectrum_t;

#endif /* INCLUDE_SPECTRUM_H_ */
