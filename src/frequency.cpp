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

	for ( uint n = 1; n<10;n++ )
	{
		string frqName{""};
		frqName.push_back('0');
		frqName.push_back(int2char(n) );
		frqNamesArray[n] = frqName;
	}
	for( uint n = 10; n<C0; n++ )
	{
		string frqName{ to_string( n-9 )};
		frqNamesArray[n] = frqName;
	}
	for ( int oct = min_octave; oct < (int)max_octave; oct++)
	{
		char octave = int2char(oct);
		for( uint step = 0; step < oct_steps; step++ )
		{
			string 	frqName {""};
			frqName.push_back(OctChars[step]);
			frqName.push_back(octave);
			frqNamesArray[ index( oct, step ) ] = frqName;
		}
	}
	cout << "Frequency Names initialized at " << hex << &frqNamesArray << endl;

}


array<float,FRQARR_SIZE> frqArray = {0};

constexpr void initFrqArray(  )
{
	if ( frqArray[0] > 0 ) return ;
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
				x = pow(2.0, (n-C0idx)/((float)oct_steps) ) * oct_base_freq ; // C0 = oct_base_freq
		}
		frqArray[n+1] = x ;
	}
	cout << "Piano key frequencies initialized at " << hex << &frqArray << endl;
	frqArray[0] = C0;
}


Frequency_class::Frequency_class() :
		Logfacility_class("Frequency_class")
{
	className = Logfacility_class::className;
	initFrqArray();
	initFrqNamesArray();
	frq_vector_len = frqArray.size();

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

	( _idx < C0 ) ? frq = frqArray[ _idx ] : frq = frqArray[ _idx ]* _base_freq / oct_base_freq;
	return frq;

}

frq_t Frequency_class::Calc( const int& idx )
{
	return frqArray[ idx]; //Calc( oct_base_freq, idx);
}
uint Frequency_class::Index( string frqName )
{
	for( uint n = 0; n < FRQARR_SIZE; n++ )
		if ( strEqual( frqNamesArray[n], frqName ) )
			return n;
	return 0; // = ""
}
frq_t Frequency_class::Frqadj( const uint8_t& channel, const int8_t& value )
{
	return ( 1 + channel + (float)value * 0.01 );
};

void Frequency_class::ShowFrqTable()
{
	Table.AddColumn( "Index", 6);
	Table.AddColumn( "Frequency", 16 );
	Table.AddColumn( "Name", 4);
	Table.PrintHeader();
	for( uint n = 1; n < FRQARR_SIZE; n++ )
	{
		Table.AddRow( n, frqArray[n], frqNamesArray[n] );
	}
}

void Frequency_class::TestFrequency()
{
	TEST_START( className );
	ShowFrqTable();
	frq_t f;
	f = Calc( 10 );
	ASSERTION( f == 1, "Frq calc 10", f, 1 );
	uint i;
	i = Index( "A3" );
	f = Calc( i );
	ASSERTION( fcomp(f,220), "Frq calc A3", f, 220 );
	i = Index( "C2" );
	f = Calc( 10, i);
	ASSERTION( fcomp(f,40), "Frq base 10", f, 40 );
	f = Frqadj( 1, 45 );
	ASSERTION( fcomp(f,2.45), "Frq adj ", f, 2.45 );
	TEST_END( className );
}

