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

void Spectrum_class::assign_frq( int channel, string str  )
{
	int value = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		value = Str.secure_stoi( str );
		spectrum.frqidx[channel] = check_range( frqarr_range, value, "assign_frq" );
	}
	else
		spectrum.frqidx[channel] = 0;
	spectrum.frqadj[channel] = Frqadj(channel, value );
};

void Spectrum_class::assign_vol( int i, string str  )
{
	int value = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		value = Str.secure_stoi( str );
		spectrum.volidx[i] = check_range( volidx_range, value, "assign_vol" );
	}
	else
		spectrum.volidx[i] = 0;
};
void Spectrum_class::assign_waveform( int i, string str  )
{
	int value = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		value = Str.secure_stoi( str );
		spectrum.wfid[i] = check_range( waveform_range, value, "assign_waveform" );
	}
	else
		spectrum.wfid[i] = 0;
};


Spectrum_class::spectrum_t Spectrum_class::Parse_data(  vector_str_t arr,
														const char& oscid, const int& _type )
{
	auto assign_dta = [ this, _type  ]( vector<string> arr )
	{
		arr.erase( arr.begin()); 	// 0:SPEV, 1:SPEF
		arr.erase( arr.begin());	// 1:MAIN,VCO,FMO
		String Str{ arr[0] };		// compatibility
		if ( not Str.is_number() )
			arr.erase( arr.begin());	// 2:waveform str

		size_t i = 0;
		for ( string str : arr )
		{
			if ( i < spec_arr_len ) // ignore further entries
			{
				switch (_type)
				{
					case SPEV : { assign_vol(i, str); break; }
					case SPEF : { assign_frq(i, str); break; }
					case SPEW : { assign_waveform(i, str); break; }
				}
			}
			i++;
		}
	};
	spectrum.osc = oscid;
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
		if ( wf.compare( wstr ) == 0 )
			return id;
		id++;
	}
	return 0;
};


string Spectrum_class::Get_waveform_str( int id )
{
//	cout << "wf idx";
	int wfid = check_range( waveform_range, (int) id, "Get_waveform_str" );
	return waveform_str_vec[ wfid ];
};

string Spectrum_class::Show_this_spectrum( )
{
	stringstream strs{""};
	strs << endl;

	for ( int type : spectrumNum ) //spectrumTag )
	{
		strs << Show_spectrum_type( type, this->spectrum ) ;
		strs << endl;
	}
	return strs.str();
}

void Spectrum_class::Save_spectrum_table(fstream* f, const spectrum_t& spec )
{
	Table_class Table{ defaultopt };
	Table.opt.FILE 		= f;
	Table.opt.Separator = ',';
	Table.AddColumn("Type", 6);
	Table.AddColumn("Name", 6);
	Table.AddColumn("Main",6);
	Table.AddColumn("harm1",6 );
	Table.AddColumn("harm2",6 );
	Table.AddColumn("harm3",6 );
	Table.AddColumn("harm4",6 );
	Table.PrintHeader();
	Table.AddRow( 	spectrumTag[SPEV], OscRole.types[ spec.osc ],
		(int)spec.volidx[0], (int)spec.volidx[1], (int)spec.volidx[2], (int)spec.volidx[3],	(int)spec.volidx[4]);
	Table.AddRow( 	spectrumTag[SPEF], OscRole.types[ spec.osc ],
		(int)spec.frqidx[0], (int)spec.frqidx[1], (int)spec.frqidx[2], (int)spec.frqidx[3],	(int)spec.frqidx[4]);
	Table.AddRow( 	spectrumTag[SPEW], OscRole.types[ spec.osc ],
		(int)spec.wfid[0], (int)spec.wfid[1], (int)spec.wfid[2], (int)spec.wfid[3], (int)spec.wfid[4]);

}

string Spectrum_class::Show_spectrum_type( const int& _type, const spectrum_t& spec )
{
	stringstream strs{""};
	auto show_dta = [ &strs ]( auto val)
	{
		strs << setw(6) << dec << (int)val << ",";
	};

	strs 	<< right << spectrumTag[_type] << ","
			<< setw(9) << OscRole.types[ spec.osc ] << ",";

	switch ( _type )
	{
		case SPEV: { std::ranges::for_each( spec.volidx, show_dta); break; }
		case SPEF: { std::ranges::for_each( spec.frqidx, show_dta); break; }
		case SPEW: { std::ranges::for_each( spec.wfid  , show_dta); break; }
		default:
			break;
	}

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

	String str { "SPEF,VCO,1,2,3,4,5" };
	vector_str_t arr = str.to_array( ',' );
	Parse_data( arr, osc_struct::VCOID, SPEF );
	ASSERTION( spectrum.osc == osc_struct::VCOID, "oscid", osc_struct::VCOID, osc_struct::VCOID );
	ASSERTION( ( abs(spectrum.frqadj[4]) - 5.05) < 1E-6, "frqadj4", ( abs(spectrum.frqadj[4])) , 5.05 );

	str = "SPEV,FMO,1.0,1,3,2,3" ;
	arr = str.to_array( ',' );
	Parse_data( arr, osc_struct::FMOID, SPEV );
	cout << show_items( spectrum.vol) << endl;
	ASSERTION( ( abs( spectrum.vol[1] - 1.0/10.0) < 1E-6),"",abs( spectrum.vol[1] - 1.0/10.0), 0 );

	ASSERTION( strEqual( waveform_str_vec[3], waveFunction_vec[3].name),
			"waveform_str_vec",
			waveform_str_vec[3],
			waveFunction_vec[3].name );

	fstream f;
	f.open( "/tmp/Save_spectrum_table.txt", fstream::out );
	Save_spectrum_table( &f, this->spectrum );
	f.close();

//assert(false);

	TEST_END( className );
}


