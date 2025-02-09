/*
 * spectrum.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */
#include <Spectrum.h>

/*
 * Spectrum_base
 */




Spectrum_class::spectrum_t Spectrum_class::Parse_data( vector_str_t arr, char oscid, char _type )
{

	auto assign_dta = [ this, _type ]( vector<string> arr )
	{
		arr.erase( arr.begin()); 	// 0:SPEC
		arr.erase( arr.begin());	// 1:MAIN,VCO,FMO
		arr.erase( arr.begin());	// 2:waveform str
		size_t i = 0;

		for ( string str : arr )
		{
			if ( i < spec_arr_len ) // ignore further entries
			{
				if ( _type == 0 )
				{
					if ( str.length() > 0 )
						spectrum.vol[i] = stof( str );
					else
						spectrum.vol[i] = 0;
				}
				else
				{
					if ( str.length() > 0 )
						spectrum.frq[i] = stof( str );
					else
						spectrum.frq[i] = 0;

				}
			}
			i++;

		}
	};

	spectrum = spec_struct();
	int id 	= Get_waveform_id( arr[2] );
	if ( id < 0 )
		return default_spec;
	else
		spectrum.id = id;

	assign_dta( arr );
	Sum( spectrum );
	spectrum.osc = oscid;
	return spectrum;
}


//void Spectrum_base::Set_spectrum( uint8_t id, int channel, spec_dta_t value )
void Spectrum_class::Set_spectrum( uint8_t id, spectrum_t spec )
{
	spectrum = spec;
	Sum( spectrum );
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
	set<int> wfid_set = range_set(0,waveform_str_vec.size());
	if ( wfid_set.contains( id ) )
	{
		return waveform_str_vec[ id ];
	}
	else
		return "unknown";
};

string Spectrum_class::Show_spectrum( const string& _type, const spectrum_t& spec )
{
	stringstream strs{""};

	auto show_dta = [ &strs ]( spec_dta_ft val)
	{
		strs << setprecision(2) << setw(5) << val << ",";
	};

	strs 	<< right << _type << ","
			<< setw(9) << OscRole.types[ spec.osc ] << ","
			<< setw(9) << Get_waveform_str( spec.id ) << ",";
	if ( strEqual( "SPEV", _type) )
		std::ranges::for_each( spec.vol, show_dta);
	else
		std::ranges::for_each( spec.frq, show_dta);
	return strs.str();
}

void Spectrum_class::Sum( spectrum_t& spec )
{
	spec.sum = 0;
	std::ranges::for_each( spec.vol, [ &spec ]( float f){ spec.sum += f ;});

}

void Spectrum_class::Test_Spectrum()
{
	TEST_START( className );
	spectrum_t spec = spec_struct();
	spec.vol = { 1, 2, 4, 6 };
	Sum( spec );
	ASSERTION( spec.sum == 13, "spectrum.sum", spec.sum, "= 13" );


	TEST_END( className );
}


