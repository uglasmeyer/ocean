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

void Spectrum_class::assign_frq( int i, string str  )
{
	uint8_t f = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		f = Str.secure_stoi( str );
		( f == 0 ) ? spectrum.frqidx[i] = 0 : spectrum.frqidx[i] = f;
	}
	else
		spectrum.frqidx[i] = 0;
	spectrum.frqadj[i] = Frqadj(i, f );
};

void Spectrum_class::assign_vol( int i, string str  )
{
	uint8_t f = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		f = Str.secure_stoi( str );
		( f == 0 ) ? spectrum.volidx[i] = 0 : spectrum.volidx[i] = f;
	}
	else
		spectrum.volidx[i] = 0;
};

Spectrum_class::spectrum_t Spectrum_class::Parse_data( vector_str_t arr, char oscid )
{

	auto assign_dta = [ this  ]( vector<string> arr )
	{

		String Str{""};

		uint8_t _type = 0;
		if ( strEqual( arr[0], "SPEF") )
			_type = 1;

		arr.erase( arr.begin()); 	// 0:SPEV, 1:SPEF
		arr.erase( arr.begin());	// 1:MAIN,VCO,FMO
		arr.erase( arr.begin());	// 2:waveform str

		size_t i = 0;
		for ( string str : arr )
		{
			if ( i < spec_arr_len ) // ignore further entries
			{
				if ( _type == 0 )
				{
					assign_vol( i, str );
				}
				else
				{
					assign_frq( i, str );
				}
			}
			i++;

		}
	};

	spectrum = default_spec;
	spectrum.osc = oscid;

	int id 	= Get_waveform_id( arr[2] );
	if ( id < 0 )
	{
		return spectrum;
		Comment( WARN, "Spectrum does not have a valid waveformid" );
	}
	else
		spectrum.wfid = id;



	assign_dta( arr );

	Sum( spectrum );
	return spectrum;
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
		strs << setprecision(3) << setw(6) << val << ",";
	};

	strs 	<< right << _type << ","
			<< setw(9) << OscRole.types[ spec.osc ] << ","
			<< setw(9) << Get_waveform_str( spec.wfid ) << ",";
	if ( strEqual( "SPEV", _type) )
		std::ranges::for_each( spec.volidx, show_dta);
	else
		std::ranges::for_each( spec.frqidx, show_dta);
	return strs.str();
}

void Spectrum_class::Sum( spectrum_t& spec )
{
	float _sum = std::accumulate(spec.volidx.begin(), spec.volidx.end(), 0);;
	if ( ( abs(_sum - 1.0 )  > 1E-6) )
	    std::ranges::transform( spec.volidx, spec.vol.begin(), [ _sum ]( float f ){ return f/_sum ;} );
	spec.sum = _sum;
}

void Spectrum_class::Test_Spectrum()
{
	TEST_START( className );

	String str { "SPEF,VCO,sinus,1,2,3,4,5" };
	vector_str_t arr = str.to_array( ',' );
	Parse_data( arr, osc_struct::VCOID );
	ASSERTION( spectrum.osc == osc_struct::VCOID, "oscid", osc_struct::VCOID, osc_struct::VCOID );
	ASSERTION( ( abs(spectrum.frqadj[4]) - 5.05) < 1E-6, "frqadj4", ( abs(spectrum.frqadj[4])) , 5.05 );

	str = "SPEV,FMO,sinus,1.0,1,3,2,3" ;
	arr = str.to_array( ',' );
	Parse_data( arr, osc_struct::FMOID );
	cout << show_items( spectrum.vol ) << endl;
	ASSERTION( ( abs( spectrum.vol[1] - 1.0/10.0) < 1E-6),"",abs( spectrum.vol[1] - 1.0/10.0), 0 );

	TEST_END( className );
}


