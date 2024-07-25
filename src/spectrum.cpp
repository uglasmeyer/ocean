/*
 * spectrum.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */
#include <Spectrum.h>


Spectrum_class::Spectrum_class()
: Logfacility_class("Spectrum")
{
	Set_Spec_Name("default");
}

void Spectrum_class::Set_Spec_Name( string name)
{
	if ( name.length() == 0 )
		Name = "default";
	else
		Name = name;
	Comment( INFO, "Spectrum Name: " + Name);
	Spectrum_file 			= dir_struct().instrumentdir + Name + ".kbd"; // same as instrument

}

bool Spectrum_class::Read( uint id )
{
	vector<string> osc_str_vec = {"MAIN", "VCO", "FMO" };
	string osc_str = osc_str_vec[ id ];
	Comment( INFO, "reading " + osc_str );
	return Read( osc_str );

}
bool Spectrum_class::Read( string osc_str )
{

	string line;
	fstream File;
	spectrum_list.clear();
	this->osc_type = osc_str;

	Comment( INFO, "using spectrum file: " + Spectrum_file );

	File.open( Spectrum_file, fstream::in );
	if ( not File.is_open())
	{
		Comment( ERROR, "Input file: " + Spectrum_file + " does not exist" );
		return false;
	}

	getline( File, line );
	do
	{
		parse_data( line );

	} while( getline( File, line));
	File.close();

	// append spectrum_list if file contains less entries
	if ( spectrum_list.size() < waveform_str_vec.size() )
	{
		Comment( WARN, "Reset spectrum "	+ to_string( spectrum_list.size()) + " < "
											+ to_string( waveform_str_vec.size()));
		reset();
		return true;
	}
	return true;
}

void Spectrum_class::reset()
{
	spectrum_list.clear();
	uint8_t id = 0;
	for ( string wf : waveform_str_vec )
	{
		spec_buffer 			= spec_struct();
		spec_buffer.id 			= id;
		spec_buffer.osc 		= wf;
		spectrum_list.push_back( spec_buffer );
		id++;
	}
}

void Spectrum_class::parse_data( string line )
{

	String 			Str	{""};
	vector_str_t 	arr	{""};
	Str.Str 			= line;
	Str.normalize();
	arr = Str.to_array( ',' );

	if ( arr.size() == 0   )
	{
		Comment( WARN, "empty spectrum using defaults, what might not be intended.");
		return;
	}
	if ( arr[0].compare( "SPEC" ) 	!= 0 ) return;
	if ( arr[1].compare( osc_type ) != 0 ) return;

	spec_buffer = spec_struct();
	int id = Get_waveform_id( arr[2] );
	if ( id >= 0 )
	{
		spec_buffer.id 	= id;
		spec_buffer.osc = arr[2];// 2:waveform str
	}
	else
	{
		Comment( WARN, "cannot assign id " + to_string( id ));
		return;
	}
	arr.erase( arr.begin()); 	// 0:SPEC
	arr.erase( arr.begin());	// 1:MAIN,VCO,FMO
	arr.erase( arr.begin());	// 2:waveform str

	size_t i = 0;

	for ( string str : arr )
	{
		if ( i < spec_dta_len ) // ignore further entries
		{
			if ( str.length() > 0 )
				spec_buffer.dta[i] = stoi( str );
			else
				spec_buffer.dta[i] = 0;
			i++;
		}
	}
	spec_buffer.Sum();
	spectrum_list.push_back( move( spec_buffer ) );

	Comment(DEBUG, line );
	return ;
}


void Spectrum_class::Set_spectrum( uint8_t id, int channel, int value )
{
	if ( not check_size( spectrum_list ) )	return;

	list_itr = spectrum_list.begin();
	advance( list_itr, id);
	int a = list_itr->dta[channel ];
	list_itr->dta[channel ] = value;
	list_itr->sum = list_itr->sum - a + value;
}

spec_struct_t Spectrum_class::Get_spectrum(uint8_t id )
{

	if ( id > spectrum_list.size( ) - 1)
	{
		Comment( WARN, "spectrum id " + to_string( id ) +
				" is larger than the spec list (" + to_string( spectrum_list.size() -1 )+ ")");
		Comment( INFO, "using default spectrum" );
		Comment ( INFO, "this may cause undefined behaviour");
		return default_spec;
	}

	list_itr = spectrum_list.begin();
	advance( list_itr, id);
	return *list_itr;
}

vector<string> Spectrum_class::Get_waveform_vec()
{
	return waveform_str_vec;
}

int Spectrum_class::Get_waveform_id( string wstr )
{
	int id = 0;
	for ( string wf : waveform_str_vec)
	{
		if ( wf.compare( wstr ) == 0 ) return id;
		id++;
	}
	return -1;
};


string Spectrum_class::Get_waveform_str( uint id )
{
	if (( id >= 0 ) and ( id < waveform_str_vec.size() ))
	{
		return waveform_str_vec[ id ];
	}
	else
		return "unknown";
};

string Spectrum_class::Show_spectrum(  )
{
	stringstream strs{""};

	auto show_dta = [ &strs ]( int val )
		{
		  strs << dec << setw(4) << val << ",";
		};
	auto show_struct = [ this, &strs, &show_dta ]( auto spec )
		{
			strs 	<< right << "SPEC,"
					<< setw(9) << osc_type << ","
					<< setw(9) << spec.osc << ",";
			for_each( spec.dta.begin(), spec.dta.end(), show_dta);
			strs << endl;
		};

	for_each( spectrum_list.begin(), spectrum_list.end(), show_struct );
	return strs.str();
}

bool Spectrum_class::check_size( spec_list_t sp )
{
	bool ret = (( sp.size() > 0 ));
	if( not ret ) Comment( ERROR, "empty struct");
	return ret;
}

void Spectrum_class::Test()
{

	Set_Loglevel(TEST, true );
	Comment( TEST, "Spectrum testing" );
	Set_Spec_Name( ".test" );
	Spectrum_class::Read("MAIN");
	Set_spectrum( 2, 5, 30 );

	spec_struct_t t = Get_spectrum(2);

	cout << t.osc << endl;
	assert( t.osc.compare( "sgnSin" ) == 0 );
	assert( t.dta[5] == 30 );
	assert( t.sum == 130 );
	Set_spectrum( 2,5,0 );
	assert( filesystem::exists(Spectrum_file));
	cout << Show_spectrum();

	Comment( TEST, "Spectrum test done" );

}


