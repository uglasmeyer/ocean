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




Spectrum_base::spectrum_t Spectrum_base::Parse_data( vector_str_t arr, char oscid )
{

	auto assign_dta = [ this ]( vector<string> arr )
	{
		arr.erase( arr.begin()); 	// 0:SPEC
		arr.erase( arr.begin());	// 1:MAIN,VCO,FMO
		arr.erase( arr.begin());	// 2:waveform str
		size_t i = 0;

		for ( string str : arr )
		{
			if ( i < spec_dta_len ) // ignore further entries
			{
				if ( str.length() > 0 )
					spectrum.dta[i] = stoi( str );
				else
					spectrum.dta[i] = 0;
				i++;
			}
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


void Spectrum_base::Set_spectrum( uint8_t id, int channel, uint8_t value )
{
	spectrum.id = id;
	spectrum.dta[channel ] = value;
	Sum( spectrum );
}

vector<string> Spectrum_base::Get_waveform_vec()
{
	return waveform_str_vec;
}

int Spectrum_base::Get_waveform_id( string wstr )
{

	int id = 0;
	for ( string wf : waveform_str_vec)
	{
		if ( wf.compare( wstr ) == 0 ) return id;
		id++;
	}
	return -1;
};


string Spectrum_base::Get_waveform_str( uint id )
{
	set<int> wfid_set = range_set(0,waveform_str_vec.size());
	if ( wfid_set.contains( id ) )
	{
		return waveform_str_vec[ id ];
	}
	else
		return "unknown";
};

string Spectrum_base::Show_this_spectrum( spectrum_t spec )
{
	stringstream strs{""};

	auto show_dta = [ &strs ]( int val)
		{
		  strs << dec << setw(3) << val << ",";
		};
	auto show_struct = [ this, &strs, &show_dta ]( auto spec )
		{
			strs 	<< right << "SPEC,"
					<< setw(9) << osc_type_vec[ spec.osc ] << ","
					<< setw(9) << Get_waveform_str( spec.id ) << ",";
			std::ranges::for_each( spec.dta, show_dta);
			strs << dec << setw(3) << spec.sum ;
		};

	show_struct( spec );
	return strs.str();
}

void Spectrum_base::Sum( spectrum_t& spec )
{
	int sum = 0;
	for ( size_t i = 0; i < spec_dta_len ; i++ )
		sum = sum + spec.dta[i];
	if ( sum == 0 )
	{
		sum = 100;
		spec.dta[0] = 100;
	}
	spec.sum = sum;
}


