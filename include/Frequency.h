/*
 * Frequency.h
 *
 *  Created on: Feb 7, 2025
 *      Author: sirius
 */

#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include <Ocean.h>
#include <Exit.h>

class Frequency_class
{
	vector<float> frq_vector { 0 };
	uint A0idx = 0;


	void initFrqVector(  )
	{
		if ( frq_vector.size() > 1 ) return ;

		for ( int n = 0; n < 9; n++)
		{
			float x = (n+1) * 0.1;
			frq_vector.push_back( x );
		}
		for ( int n = 0; n < 16; n++)
		{
			float x = (n+1) ;
			frq_vector.push_back( x );
		}
		A0idx = frq_vector.size();

		for( int n = 0; n < 72; n++ )
		{
			float x = pow(2.0, (n+3)/12.0) ; // 3 considers A0 = 55/4
			frq_vector.push_back( x );
		}
		cout << "Piano key frequencies initialized" << endl;
		frq_vector[0] = A0idx;
	}

public:
	int frq_vector_len	= 0;
	int A0				= 0;

	float Calc( const float& _base_freq, int idx )
	{
		float frq = 0;
		if ( idx >= frq_vector_len ) idx = frq_vector_len-1;
		( idx < A0 ) ? frq = frq_vector[ idx ] : frq = frq_vector[ idx ] * _base_freq;
		return frq;
	}

	float Calc( int idx )
	{
		return Calc( oct_base_freq, idx);
	}

	Frequency_class()
	{
		initFrqVector();
		frq_vector_len = frq_vector.size();

		ASSERTION( frq_vector_len > 1, "frq_vector", frq_vector_len, " >1" );

		A0 = frq_vector[0];
	}
	~Frequency_class(){};

};




#endif /* FREQUENCY_H_ */
