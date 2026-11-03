/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

/*
 * spectrum.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Oscwaveform.h>
#include <Spectrum.h>
#include <String.h>
#include <Table.h>

using namespace spectrum_ns;

Spectrum_class	OscSpectrum { SPEC_e::FREQ };
Spectrum_class*	oscspectrum = &OscSpectrum;

Spectrum_class::Spectrum_class( SPEC_e _type ) :
	Logfacility_class	("Spectrum")
{
	Waveform 		= ( _type == SPEC_e::ADSR ) ? adsrform : waveform;
	spectrumTmp		= default_spectrum;
	spectrumTmp.type= _type;

	Table.opt.Separator = ',';
	Table.AddColumn("Type", 6);
	Table.AddColumn("Name", 6);
	Table.AddColumn("Main", 6);
	Table.AddColumn("harm1",6 );
	Table.AddColumn("harm2",6 );
	Table.AddColumn("harm3",6 );
	Table.AddColumn("harm4",6 );
	Table.AddColumn("harm5",6 );
	Table.AddColumn("harm6",6 );
	Table.AddColumn("harm7",6 );
	Table.AddColumn("harm8",6 );
}


spectrum_t Spectrum_class::Get_spectrum()
{
	return this->spectrumTmp;
}

template< class field_T, class range_T >
void Spectrum_class::assign_value( string nr_str,
									field_T& field,
									range_T range  )
{
	String Str{""};
	int value = Str.secure_stoi( nr_str, range.min );
	typeof( range.min ) val	= value;
	auto f = check_range( range, val, "assign_value" );
	field = field_T( f );

};

template< typename field_T, class range_T >
void Spectrum_class::assign_values( string_vec_t arr,
									array<field_T, SPEC_CHANNELS>& field,
									range_T range  )
{

	uint8_t channel = 0;
	for_each( arr, [ this, &channel, &field, range ]( string nr_str )
	{
		if( channel < SPEC_CHANNELS )
			assign_value( nr_str, field[channel], range );
		channel++;
	});
};

void Spectrum_class::assign_frq( string_vec_t arr )
{
	assign_values( arr, spectrumTmp.frqidx, harmonic_range );
	assign_value( arr[0], spectrumTmp.frqidx[0], frqarr_range );// overwrite 0
};

char Spectrum_class::Type_flag( const string& type_str )
{
	if ( type_str.length() == 0 ) return -1;
	char Flag	= type_str[type_str.length()-1];

	switch ( Flag )
	{
		case 'F' : { return SPEF; break; }
		case 'V' : { return SPEV; break; }
		case 'W' : { return SPEW; break; }
		default  : { return -1 	; break; }
	} // switch  Flag
	return -1;
}
SPEC_e Spectrum_class::spec_type( const string& type_str )
{
	return (  type_str.length() > 3 ) ? SPEC_e::ADSR : SPEC_e::FREQ;
}
OSCID_e Spectrum_class::osc_TypeId( const string& type_str )
{
	string str = type_str.substr(0, 3 );
	for( OSCID_e id : oscIds )
	{
		if ( str.compare( typeNames[id] ) == 0 )
		{
			return id;
		}
	}
	Comment( ERROR, "unknown OSC type ", str );
	return NOOSCID;
}

spectrum_t Spectrum_class::Parse_data(  string_vec_t arr )
{
	auto assign_dta = [ this, &arr ]()
	{
		char _type = Type_flag( arr[0] );
		arr.erase( arr.begin()); 	// 0:SPEV, 1:SPEF
		arr.erase( arr.begin());	// 1:OSC,VCO,FMO
		String Str{ arr[0] };		// compatibility
		if ( not Str.is_number() )
			arr.erase( arr.begin());	// 2:waveform str
		switch (_type)
		{
			case SPEV : { assign_values	( arr,spectrumTmp.volidx, volidx_range ); break; }
			case SPEW : { assign_values	( arr,spectrumTmp.wfid	, wfid_range )	; break; }
			case SPEF : { assign_frq	( arr ); break; }
		}	;
	};


	spectrumTmp.type= spec_type	( arr[1] );//adsr;
	spectrumTmp.osc = osc_TypeId( arr[1]);//oscid;
	assign_dta		();
	return spectrumTmp;
}

vector<string> Spectrum_class::Get_waveform_vec()
{
	return Waveform->waveform_str_vec;
}

string Spectrum_class::Show_spectrum( const spectrum_t spec )
{
	stringstream strs{""};
	strs << endl;

	for ( int type : spectrumNum ) //spectrumTag )
	{

		strs << Show_spectrum_type( type, spec ) ;
		strs << endl;
	}
	return strs.str();
}

Table_class* Spectrum_class::Get_spectrum_table	()
{
	return &this->Table;
}

void Spectrum_class::Show_spectrum_table(	ostream& f,
											const spectrum_t& spec,
											bool header )
{
	auto show = [ this ]( string tag0, string tag1, auto arr )
	{
		Table.AddRowItem( 0, tag0 );
		Table.AddRowItem( 1, tag1 );
		size_t	n = 2;
		for_each( arr, [ this, &n ]( int item)
				{ Table.AddRowItem(n++, item ) ;});
		Table.PrintItems();
	};

	Table.opt.FILE 		= &f;
	string OscRoleTag	= typeNames[spec.osc];
	if( spec.type == SPEC_e::ADSR )
		OscRoleTag += "A";

	if ( header ) Table.PrintHeader();
	show( spectrumTag[SPEV], OscRoleTag, spec.volidx );
	show( spectrumTag[SPEF], OscRoleTag, spec.frqidx );
	show( spectrumTag[SPEW], OscRoleTag, spec.wfid   );

}

string Spectrum_class::Show_spectrum_type( const int& _type, const spectrum_t& spec )
{
	stringstream strs{""};
	string OscRoleTag = typeNames[spec.osc];
	if( spec.type == SPEC_e::ADSR )
		OscRoleTag += "A";

	auto show_dta = [ &strs, &OscRoleTag ]( auto val)
	{
		//strs << setw(6) << dec << (int)val << ",";
		strs << setw(6) << dec << (float)val << ",";
	};

	strs 	<< right << spectrumTag[_type] << ","
			<< setw(9) << OscRoleTag << ",";

	switch ( _type )
	{
		case SPEV	: { for_each( spec.volidx, show_dta); break; }
		case SPEF	: { for_each( spec.frqidx, show_dta); break; }
		case SPEW	: { for_each( spec.wfid  , show_dta); break; }
		default		: break;
	}

	return strs.str();
}

float Spectrum_class::VolFactor( spec_arr_8t& idxarr )
{

	float _sum = std::accumulate( idxarr.begin(), idxarr.end(), 0.0f );

	if( fcomp( _sum, 0.0f ) )
		return 1.0;

	return 1.0/_sum;
}

#include <System.h>
#include <fstream>
void Spectrum_class::Test_Spectrum()
{
	spectrum_t test_spectrum =
	{
		.volidx = { 100, 0, 0, 0, 0 },
		.frqidx = { A3, 0, 0, 0, 0 },
		.wfid 	= { SINUS,SINUS,SINUS,SINUS,SINUS},
		.phi	= zero_phase,
		.osc 	= OSCID,
		.type	= SPEC_e::FREQ
	};

	TEST_START( className );

	assign_value( string("-111119999999999"), spectrumTmp.frqidx[0], frqarr_range );
	Assert_equal( (int)spectrumTmp.frqidx[0], 0 );
	assign_value( string("-11111"), spectrumTmp.frqidx[0], frqarr_range );
	Assert_equal( (int)spectrumTmp.frqidx[0], 0 );
	assign_value( string("11111"), spectrumTmp.frqidx[0], frqarr_range );
	Assert_equal( (int)spectrumTmp.frqidx[0], 109 );

	String str { "SPEF,FMOA,1,2,3,4,5" };
	string_vec_t arr = str.to_array( ',' );
	Parse_data( arr );

	str = "SPEF,VCO,1,2,3,4,5";
	arr = str.to_array( ',' );
	Parse_data( arr );
	ASSERTION( 		spectrumTmp.frqidx[4] == 5, "spectrum.frqidx[4]",
			(int)	spectrumTmp.frqidx[4], 5 );
	Assert_equal( Frqadj( 4, spectrumTmp.frqidx[4] ),  1.714286f );

	str = "SPEV,FMO,100,5,5,0,0" ;
	arr = str.to_array( ',' );
	Parse_data( arr );
	Assert_equal( (int)spectrumTmp.osc, (int)FMOID );
	float volfactor = VolFactor( spectrumTmp.volidx );
	Assert_equal( fcomp( volfactor, 1.0f/110.0f ), true );
	float volsum = 0.0f;
	for( int v : spectrumTmp.volidx )
		volsum += v*volfactor;
	Assert_equal( fcomp( volsum, 1.0 ), true );
	string test_file = "/tmp/Save_spectrum_table.txt";
	Remove_file( test_file );
	fstream f { test_file, fstream::out };
	Show_spectrum_table( f, test_spectrum );
	f.close();
	Assert_equal( exists( test_file ) , true );



	TEST_END( className );
}


