/*
 * oscwaveform.cpp
 *
 *  Created on: Feb 21, 2025
 *      Author: sirius
 */

#include <Oscwaveform.h>

vector<string> waveform_str_vec = gen_waveform_str_vec( waveFunction_vec );

random_device 	rd;
mt19937 		engine(rd());
uniform_real_distribution<> distrib( -1, 1 );


// https://stackoverflow.com/questions/6339970/c-using-function-as-parameter

int sgn( const float& x )
{
	return ( x >= 0 ) ? 1 : -1;
}

float psgn( const float& x )
{
	return ( x > 0 ) ? x : 0 ;
}

float Rnd( param_t& p )
{ 	// provides values between -amp .. amp
	float rint = p.amp * distrib( engine );
	return rint;
}


Data_t 	Delta( param_t& p )
{
	float f1 = fmod(p.phi, p.phi + p.dphi);
	return ( f1 < p.phi ) ? p.amp : 0.0;
}

Data_t recprev = 0.0;
Data_t Rnd_step( param_t& p )
{
	Data_t f = Rectangle( p );
	if ( fcomp( f, recprev ))
		return recprev;
	else
		recprev = f;
	return recprev * distrib( engine );
}

float modulo( const float& x,  const float& n )
{
	float 	d 	= floor ( x / n ); // floor corresponds to the gaussian bracket
	return 	x - d*n;
}

float maximum( const float& x,  const float& y)
{
	return ( x > y ) ? x : y;
}

Data_t Zero( const float& x)
{
	return 0.0;
}
Data_t Sinus( param_t& p )
{
	return (  p.amp * sin( p.phi ));
}
Data_t SignSin( param_t& p )
{
	return ( p.amp * psgn(  sin ( p.phi ) ));
}
Data_t Rectangle( param_t& p )
{ // maxima: round(mod(phi,1))*amp*2-amp;
	return - p.amp*( round(floor(p.phi)-p.phi)) - p.amp/2;
}
Data_t Triangle( param_t& p )
{ // maxima: f:2*amp*(abs(max( 1 - mod(phi, 2), -1 ) )-1)+amp;
	return (2*p.amp*(abs(maximum( 1 - modulo(p.phi, 2), -1 ) )-1)+p.amp);
}
Data_t SawTooth( param_t& p )
{
	return ( p.amp * modulo( p.phi,1 ));
}
Data_t Sawtooth( param_t& p )
{
	return ( p.amp* (1.0 - modulo( p.phi,1 )));
}
Data_t Pmw( param_t& p )
{
	phi_t phi2 	= p.phi * p.pmw ;
	return p.amp *	abs(round(modulo( p.phi , p.maxphi))) *
					abs(round(modulo(phi2	, p.maxphi))) ;
	//	return abs(round(modulo(phi , maxphi)));
}




void Oscwaveform_class::init_waveform_str_vec()
{
//	cout<<"init waveform_str_vector"<<endl;
}

void Oscwaveform_class::Test_wf()
{
	TEST_START( className );

	ASSERTION( fcomp( modulo(1,2), 1 ), "modulo", modulo(1,2), "1")
	ASSERTION( fcomp( modulo(2,1.2), 0.8), "modulo", modulo(2,1.2), "0.8")
	ASSERTION( fcomp( modulo(1,0.9), 0.1), "modulo", modulo(1,0.9), "0.1")
	ASSERTION( fcomp( modulo(2,0.9), 0.2), "modulo", modulo(2,0.9), "0.2")
	ASSERTION( fcomp( fmod(10.2,2.0), 0.2), "fmod", fmod(-1,2.9), 0.2)

	TEST_END( className );
}
