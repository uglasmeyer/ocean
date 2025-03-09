/*
 * frequency.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: sirius
 */
#include <Frequency.h>

array<string, FRQARR_SIZE> frqNamesArray = {""};

constexpr void initFrqNamesArray()
{
	if ( frqNamesArray[C0].length() > 0 ) return ;

	auto index = [  ]( int oct, uint step  )
	{
		return (int) oct*oct_steps + step + C0;
	};

	for ( int oct = min_octave; oct < (int)max_octave; oct++)
	{
		char octave = int2char(oct);
		for( uint step = 0; step < oct_steps; step++ )
		{
			string 	frqName {""};
			frqName.push_back(OctChars[step]);
			frqName.push_back(octave);
			frqNamesArray[ index( oct, step ) ] = frqName;
//			cout << dec << (int) oct << (int)step<<" "<<(int)oct*oct_steps + step + C0 << " " << frqName << endl;
		}
	}
//	for( uint n = 0; n< FRQARR_SIZE-1; n++ )
//	{
//		cout << dec << (int) n << " " << frqNamesArray[n] << endl;
//	}
	cout << "Frequency Names initialized at " << hex << &frqNamesArray << endl;

}


array<float,FRQARR_SIZE> frq_vector = {0};

constexpr void initFrqVector(  )
{
	if ( frq_vector[0] > 0 ) return ;
	uint C0idx = C0-1;

	for ( uint n = 0; n < FRQARR_SIZE-1; n++)
	{
		float x = 0;
		if ( n < 9 ) 			// range 0.1 ... 0.9
			x = (n + 1 ) * 0.1;
		else
		{
			if (n < C0idx )		// range 1 ... 16
				x = n - 8  ;
			else
				x = pow(2.0, (n-C0idx)/((float)oct_steps) ) ; // C0 = oct_base_freq
		}
		frq_vector[n+1] = x ;
	}
	cout << "Piano key frequencies initialized at " << hex << &frq_vector << endl;
	frq_vector[0] = C0;
}


Frequency_class::Frequency_class()
{
	initFrqVector();
	initFrqNamesArray();
	frq_vector_len = frq_vector.size();

	ASSERTION( frq_vector_len == FRQARR_SIZE, "frq_vector", frq_vector_len, " >1" );
	ASSERTION( strEqual( frqNamesArray[C0], "C0"), "frqNamesArray", frqNamesArray[C0], "C0" );
}
Frequency_class::~Frequency_class(){};


frq_t Frequency_class::Calc( const frq_t& _base_freq, const int& idx )
{
	frq_t frq = 0;
	uint _idx = idx;
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


