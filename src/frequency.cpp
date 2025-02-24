/*
 * frequency.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: sirius
 */
#include <Frequency.h>

array<float,FRQARR_SIZE> frq_vector = {0};

constexpr void initFrqVector(  )
{
	if ( frq_vector[0] > 1 ) return ;
	uint C0idx = 25;

	for ( uint n = 0; n < FRQARR_SIZE-1; n++)
	{
		float x = 0;
		if ( n < 9 )
			x = (n + 1 ) * 0.1;
		else
		{
			if (n < C0idx )
				x = n - 8  ;
			else
				x = pow(2.0, (n-(C0idx))/12.0) ; // C0 = 55/4
		}
		frq_vector[n+1] = x ;
	}
	cout << "Piano key frequencies initialized at " << hex << &frq_vector << endl;
	frq_vector[0] = C0idx + 1;
}

Frequency_class::Frequency_class()
{
	initFrqVector();
	frq_vector_len = frq_vector.size();

	ASSERTION( frq_vector_len == FRQARR_SIZE, "frq_vector", frq_vector_len, " >1" );

	C0 = frq_vector[0];
}
Frequency_class::~Frequency_class(){};


frq_t Frequency_class::Calc( const frq_t& _base_freq, const int& idx )
{
	frq_t frq = 0;
	int _idx = idx;
	if ( _idx >= frq_vector_len )
		_idx = frq_vector_len-1;
	( _idx < C0 ) ? frq = frq_vector[ _idx ] : frq = frq_vector[ _idx ] * _base_freq;
	return frq;
}

frq_t Frequency_class::Calc( const int& idx )
{
	return Calc( oct_base_freq, idx);
}

frq_t Frequency_class::Frqadj( const uint8_t& channel, const int8_t& value )
{
	return ( 1 + channel + (float)value * 0.01 );
};


