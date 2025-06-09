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


typedef struct param_struct
{
	float amp		= 0.0;
	phi_t phi		= 0.0;
	phi_t dphi		= 0.0;
	phi_t maxphi 	= 0.0;
	phi_t hphi		= 0.0;
	float pmw		= 0.0;
} param_t;
typedef function<Data_t( param_t& )>	wave_function_t;
struct waveFnc_struct
{
	wave_function_t		fnc;
	string				name	= "sinus";
	phi_t 				maxphi 	= 2*numbers::pi;
};
typedef waveFnc_struct 	waveFnc_t;

extern const vector<waveFnc_t>  waveFunction_vec;
extern const vector<string> waveform_str_vec;
extern const range_T<int> waveform_range;
extern  	int 	sgn(  		const float& x );


class Oscwaveform_class :
	public virtual Logfacility_class
{
	string className = "";
public:

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

	vector<string> wf_v;
	Oscwaveform_class();
	~Oscwaveform_class() {};

	void 			Test_wf();



private:


};



#endif /* OSCWAVEFORM_H_ */
//typedef param_struct param_t;

/*
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
*/
