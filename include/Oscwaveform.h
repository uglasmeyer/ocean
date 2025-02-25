/*
 * Oscwaveform.h
 *
 *  Created on: Feb 21, 2025
 *      Author: sirius
 */

#ifndef OSCWAVEFORM_H_
#define OSCWAVEFORM_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>


extern vector<string> waveform_str_vec ;

struct param_struct
{
	float amp		= 0.0;
	phi_t phi		= 0.0;
	phi_t dphi		= 0.0;
	phi_t maxphi 	= 0.0;
	float pmw		= 0.0;
};
typedef param_struct param_t;


extern  	int 	sgn(  		const float& x );
extern  	float 	psgn( 		const float& x );
extern  	float 	modulo( 	const float& x );
extern  	float 	maximum(	const float& x,
								const float& y );

extern 		Data_t 	Rnd(  		param_t& param );
extern 		Data_t 	Delta( 		param_t& param );
extern 		Data_t	Rnd_step( 	param_t& param );
extern 		Data_t 	Sinus( 		param_t& param );
extern  	Data_t 	SignSin( 	param_t& param );
extern  	Data_t 	Rectangle( 	param_t& param );
extern  	Data_t 	Triangle( 	param_t& param );
extern  	Data_t 	SawTooth( 	param_t& param );
extern  	Data_t 	Sawtooth( 	param_t& param );
extern  	Data_t 	Pmw( 		param_t& param );



class Oscwaveform_class :
	public virtual Logfacility_class
{
	string className = "";
public:



	typedef function<Data_t( param_t& )>
						wave_function_t;
	struct waveFnc_struct
	{
		wave_function_t fnc 	= Sinus;
		string			name	= "sinus";
		phi_t 			maxphi 	= 2*numbers::pi;
	};
	typedef waveFnc_struct waveFnc_t;
	enum waveformId_t
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
	const vector<waveFnc_t> waveFunction_vec =
	{
		{  Sinus	, "sinus"		, 2*pi 	},
		{  Triangle	, "triangle"	, 2		},
		{  SignSin	, "signsin"		, 2*pi 	},
		{  Rectangle, "rectangle"	, 1 	},
		{  SawTooth	, "sawtoothL"	, 1		},
		{  Sawtooth	, "sawtoothR"	, 1		},
		{  Pmw		, "PMW"			, 1		},
		{  Delta	, "delta"		, 1		},
		{  Rnd		, "noise"		, 1		},
		{  Rnd_step	, "random"		, 1		}
	};

	Oscwaveform_class() :
		Logfacility_class("Oscwaveform_class")
	{
		className = Logfacility_class::className;
		init_waveform_str_vec();
	};
	~Oscwaveform_class() {};

	void 			Test_wf();
	void init_waveform_str_vec();



private:


};



#endif /* OSCWAVEFORM_H_ */
