/*
 * frequency.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: sirius
 */
#include <Frequency.h>

frqstrarr_t frqNamesArray 		{};
frqarray_t 	frqArray 			{};

bool 		frqnamesarray_done 	= false;
bool 		frqarray_done 		= false;


Frequency_class::Frequency_class() :
	Logfacility_class("Frequency_class"),
	frequency_range{0, 0 }
{
	className 					= Logfacility_class::className;

	if ( not frqarray_done )
		initFrqArray();
	frequency_range.min 		= frqArray[1];
	frequency_range.max 		= frqArray[FRQARR_SIZE-1] ;

	if(not frqnamesarray_done )
		initFrqNamesArray();
}

frq_t Frequency_class::Calc( const frq_t& _base_freq, const int& idx )
{
	frq_t frq = 0;
	uint _idx = check_range( frqarr_range, idx, "Calc" );

	( _idx < C0 ) ? frq = frqArray[ _idx ] : frq = frqArray[ _idx ]* _base_freq / oct_base_freq;
	return frq;
}

frq_t Frequency_class::GetFrq( const int& idx )
{
//	cout << "index";
	uint frqidx = check_range( frqarr_range, idx, "GetFrq" );
	return frqArray[ frqidx];
}


uint Frequency_class::Index( const string& frqName )
{
	for( int n = frqarr_range.min; n <= frqarr_range.max; n++ )
		if ( strEqual( frqNamesArray[n], frqName ) )
			return n;
	// if frqName is a string of an index, needed by the interpreter
	String idx { frqName };
	if ( idx.is_number() )
		return idx.secure_stoi( frqName );
	cout << "WARNING: " << frqName << "-index set to default" << endl;
	return C0; // = ""
}

uint  Frequency_class::Index( const frq_t& freq )
{
	int idx = frqarr_range.min;
	if( freq >= oct_base_freq )
	{
		idx = rint( log(freq / oct_base_freq) / log2 * oct_steps + C0 );
	}
	else//if( freq < oct_base_freq )
	{
		if( freq < 1 )
		{
			idx = rint( freq * 10 );
		}
		else
			idx = rint( freq + 9 );
	}
	return check_range( frqarr_range, idx, "Index" );
}


uint  Frequency_class::Index( const int& oct, const int& step )
{
	int idx = frqIndex(step,  oct );
	return check_range( frqarr_range, idx, "Index" );
}

frq_t Frequency_class::Frqadj( const uint8_t& channel, const int8_t& value )
{
	return ( 1 + channel + (float)value * 0.01 );
};

void Frequency_class::ShowFrqTable()
{
	Table_class Table{ "", 0 };
	Table.AddColumn( "Index", 6);
	Table.AddColumn( "Frequency", 16 );
	Table.AddColumn( "Name", 4);
	Table.AddColumn( "",8);
	Table.AddColumn( "Index", 6);
	Table.AddColumn( "Frequency", 16 );
	Table.AddColumn( "Name", 4);

	Table.PrintHeader();
	const uint m = frqarr_range.max / 2 ;
	for( int n = frqarr_range.min; n <= frqarr_range.max / 2 ; n++ )
	{
		Table.AddRow( n, frqArray[n], frqNamesArray[n], "|",
				n+m, frqArray[n+m], frqNamesArray[n+m] );
	}
}

void Frequency_class::initFrqArray(  )
{

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
				// log(x/oct_base_freq)/log2*oct_steps + C0idx = n
				x = pow(2.0, (n-C0idx)/((float)oct_steps) ) * oct_base_freq ; // C0 = oct_base_freq
		}
		frqArray[n+1] =  round ( x * 10000 ) / 10000 ; // adjust digit precision
	}
	frqArray[0] = C0;
	cout << "Piano key frequencies initialized at " << hex << &frqArray << endl;
	frqarray_done = true;
}

void Frequency_class::initFrqNamesArray()
{
	// https://de.wikipedia.org/wiki/Frequenzen_der_gleichstufigen_Stimmung

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
			frqName.push_back(OctChars[ step ]);
			frqName.push_back(octave);
			frqNamesArray[ Index( oct, step ) ] = frqName;
		}
	}
	cout << "Frequency Names initialized " << endl;
	frqnamesarray_done = true;
}



void Frequency_class::TestFrequency()
{
	TEST_START( className );

	ShowFrqTable();
	frq_t f;
	f = GetFrq( 10 );
	ASSERTION( f == 1, "Frq calc 10", f, 1 );
	uint idx;
	idx = Index( "A3" );
	f = GetFrq( idx );
	ASSERTION( fcomp(f,220), "Frq calc A3", f, 220 );
	idx = Index( "C2" );
	f = Calc( 10, idx);
	ASSERTION( fcomp(f,40), "Frq base 10", f, 40 );
	f = Frqadj( 1, 45 );
	ASSERTION( fcomp(f,2.45), "Frq adj ", f, 2.45 );


	idx = Index( 220.0 );
	ASSERTION( idx == A3, "Index ", idx, A3 );
	idx = Index( 400.0 );
	ASSERTION( idx == 81, "Index ", idx, 81 );
	idx = Index( 410.0 );
	ASSERTION( idx == 82, "Index ", idx, 82 );
	idx = Index( 9.0 );
	ASSERTION( idx == 18, "Index ", idx, 18 );
	idx = Index( 0.9 );
	ASSERTION( idx == 9, "Index ", idx, 9 );
	idx = Index( 0.0 );
	ASSERTION( idx == 1, "Index ", idx, 1 );
	idx = Index( 4000.0 );
	ASSERTION( idx == 97, "Index ", idx, 97 );
	TEST_END( className );
}

