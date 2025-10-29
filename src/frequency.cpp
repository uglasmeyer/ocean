/*
 * frequency.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: sirius
 */
#include <Frequency.h>
#include <Table.h>

frqstrarr_t frqNamesArray 		{};
frqarray_t 	frqArray 			{};
harmonic_t  harmonicArray		{};

bool 		frqnamesarray_done 	= false;
bool 		frqarray_done 		= false;
bool		harmonics_done		= false;

Frequency_class::Frequency_class() :
	Logfacility_class( "Frequency_class" )
{
	className 					= Logfacility_class::className;

	if ( not frqarray_done )
		initFrqArray();
	if( not frqnamesarray_done )
		initFrqNamesArray();
	if( not harmonics_done )
		initHarmonics();
}
Frequency_class::~Frequency_class()
{
	DESTRUCTOR( className );
}
frq_t Frequency_class::Calc( const frq_t& _base_freq, const int& idx )
{
	frq_t frq = 0;
	uint _idx = check_range( frqext_range, idx, "Calc" );

	( _idx < C0 ) ? frq = frqArray[ _idx ] : frq = frqArray[ _idx ]* _base_freq / oct_base_freq;
	return frq;
}

frq_t Frequency_class::GetFrq( const int& idx )
{
	uint frqidx = check_range( frqext_range, idx, "GetFrq" );
	return frqArray[ frqidx];
}


uint Frequency_class::Index( const string& frqName )
{
	for( int n = frqext_range.min; n <= frqext_range.max; n++ )
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
	int idx = frqext_range.min;
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
	return check_range( frqext_range, idx, "Index" );
}


uint  Frequency_class::Index( const int& oct, const int& step )
{
	int idx = frqIndex(step,  oct );
	return check_range( frqext_range, idx, "Index" );
}

frq_t Frequency_class::Frqadj( const uint8_t& channel, const int8_t& value )
{
	char dir = 1;
	if( value < 0 )
		dir = -1;
	int n = check_range( harmonic_range, value, "Frqadj" );
	return ( 1 + dir*harmonicArray[ abs(n)] + channel ) ;
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

void Frequency_class::initHarmonics()
{
	harmonics_done = true;
	vector<int> 				harmonic_prims = { 1,2,3,5,7,11,13,17 };
	assert( harmonic_prims.size() == HARMON_PRIMS );
	vector<frq_t> harmonic_vec {0.0};
	for( uint i = 0 ; i < HARMON_PRIMS; i++ )
		for( uint j = i+1; j < HARMON_PRIMS; j++ )
		{
			int pi = harmonic_prims[i];
			int pj = harmonic_prims[j];
			float fraction = float(pi) / pj;
			harmonic_vec.push_back( fraction );
//			coutf << pi << "/" << pj << "=" << fraction <<  endl;
		}
	std::ranges::sort( harmonic_vec );

	for( uint n = 0; n < harmonic_vec.size(); n++ )
		harmonicArray[n] = harmonic_vec[n];
//	coutf << "size: " << harmonic_vec.size() << endl;

}
void Frequency_class::initFrqArray(  )
{

	uint C0idx = C0-1;

	for ( uint n = 0; n < FRQEXT_SIZE-1; n++)
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
	string addr = to_hex( (long)&frqArray );
	Comment( DEBUG, "Piano key frequencies initialized at ", addr );
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

	for ( int oct = min_octave; oct < (int)(max_octave+3); oct++)
	{
		char octave = int2char(oct);
		for( uint step = 0; step < oct_steps; step++ )
		{
			string 	frqName {""};
			frqName.push_back(OctChars_EN[ step ]);
			frqName.push_back(octave);
			frqNamesArray[ Index( oct, step ) ] = frqName;
		}
	}
	string addr = to_hex( (long)&frqNamesArray );
	Comment( DEBUG, "Frequency Names initialized at ", addr );
	frqnamesarray_done = true;
}



void Frequency_class::TestFrequency()
{
	TEST_START( className );

	ShowFrqTable();
	frq_t f;
	f = GetFrq( 10 );
	ASSERTION( f == 1, "Frq calc 10", f, 1 );
	int idx;
	idx = Index( "A3" );
	f = GetFrq( idx );
	ASSERTION( fcomp(f,220), "Frq calc A3", f, 220 );
	idx = Index( "C2" );
	f = Calc( 10, idx);
	ASSERTION( fcomp(f,40), "Frq base 10", f, 40 );

	int idxh=HARMON_SIZE-1;
	f = Frqadj( 0, idxh );
	float g = 1+harmonicArray[idxh];
	ASSERTION( fcomp(f, g), "Frq adj ", f, g );

	idxh = -idxh;
	f = Frqadj( 0, idxh );
	g = 1-harmonicArray[abs(idxh)];
	ASSERTION( fcomp(f, g), "Frq adj ", f, g );

	typedef struct test_struct
	{
		float frq;
		int idx;
	} test_t;
	vector<test_t> testvec =
	{
			{16000,FRQEXT_SIZE-1},
			{220,A3},
			{400,81},
			{410,82},
			{9,18},
			{0.9,9},
			{1,10}
	} ;

	for( test_t test : testvec )
	{
		idx = Index( test.frq );
		ASSERTION( idx == test.idx, "Index ", idx, test.idx );
	}

	TEST_END( className );
}

