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
extern 		Data_t 	Sin( 		param_t& param );
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


	Oscwaveform_class() :
		Logfacility_class("Oscwaveform_class")
	{
		className = Logfacility_class::className;
	};
	~Oscwaveform_class() {};

	typedef function<Data_t( param_t& )>
						wave_function_t;
	struct waveFnc_struct
	{
		uint8_t 		idx 	= 0;
		wave_function_t fnc 	= Sin;
		string			name	= "sinus";
		phi_t 			maxphi 	= 2*numbers::pi;
	};
	typedef waveFnc_struct waveFnc_t;
	enum waveformId_t
	{
		SINUS0,
		SINUS1,
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
		{ SINUS0	, Sin		, "sinus"		, 2*pi 	},
		{ SINUS1	, Sin		, "Sinus"		, 2*pi 	},
		{ TRIANGLE	, Triangle	, "triangle"	, 2		},
		{ SGNSIN	, SignSin	, "signsin"		, 2*pi 	},
		{ RECTANGLE	, Rectangle	, "rectangle"	, 2 	},
		{ SAWTOOTHL	, SawTooth	, "sawtoothL"	, 2		},
		{ SAWTOOTHR	, Sawtooth	, "sawtoothR"	, 2		},
		{ PMW		, Pmw		, "PMW"			, 1		},
		{ DELTA		, Delta		, "delta"		, 1		},
		{ NOISE		, Rnd		, "noise"		, 1		},
		{ RANDOM	, Rnd_step	, "random"		, 1		}
	};


	void 			Test_wf();


private:


};



#endif /* OSCWAVEFORM_H_ */
