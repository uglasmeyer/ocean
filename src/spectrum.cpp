/*
 * spectrum.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */


#include <Spectrum.h>
#include <String.h>
#include <Table.h>
#include <Ocean.h>

/*
 * Spectrum_base
 */


Spectrum_class::Spectrum_class() :
	Logfacility_class("Spectrum"),
	Frequency_class(),
	oscwaveform_struct()
{
	className 		= Logfacility_class::className;
	spectrumTmp		= default_spectrum;
	Comment( DEBUG, Show_spectrum( this->spectrumTmp ) );

	Table.opt.Separator = ',';
	Table.AddColumn("Type", 6);
	Table.AddColumn("Name", 6);
	Table.AddColumn("Main", 6);
	Table.AddColumn("harm1",6 );
	Table.AddColumn("harm2",6 );
	Table.AddColumn("harm3",6 );
	Table.AddColumn("harm4",6 );
}


spectrum_t Spectrum_class::Get_spectrum()
{
	return this->spectrumTmp;
}
void Spectrum_class::assign_frq( int channel, string str  )
{
	int value = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		value 					= Str.secure_stoi( str );
		spectrumTmp.frqidx[channel]= check_range( frqarr_range, value, "assign_frq" );
	}
	else
		spectrumTmp.frqidx[channel] = 0;
	spectrumTmp.frqadj[channel] = Frqadj(channel, value );
};

void Spectrum_class::assign_vol( int i, string str  )
{
	int value = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		value = Str.secure_stoi( str );
		spectrumTmp.volidx[i] = check_range( volidx_range, value, "assign_vol" );
	}
	else
		spectrumTmp.volidx[i] = 0;
};
void Spectrum_class::assign_waveform( int i, string str  )
{
	int value = 0;
	String Str{""};
	if ( str.length() > 0 )
	{
		value = Str.secure_stoi( str );
		spectrumTmp.wfid[i] = check_range( waveform_range, value, "assign_waveform" );
	}
	else
		spectrumTmp.wfid[i] = 0;
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
bool Spectrum_class::adsr_type( const string& type_str )
{
	string sub = type_str.substr(3, 1);
	return (  sub.length() > 0 );
}
OscId_t Spectrum_class::Osc_TypeId( const string& type_str )
{
	string str = type_str.substr(0, 3 );
	for( OscId_t id : oscIds )
	{
		if ( str.compare( typeNames[ id ] ) == 0 )
		{
			return id;
		}
	}
	Comment( ERROR, "unknown OSC type ", str );
	return NOOSCID;
}

spectrum_t Spectrum_class::Parse_data(  vector_str_t arr )
{
	auto assign_dta = [ this ]( vector<string> arr, char _type )
	{
		arr.erase( arr.begin()); 	// 0:SPEV, 1:SPEF
		arr.erase( arr.begin());	// 1:OSC,VCO,FMO
		String Str{ arr[0] };		// compatibility
		if ( not Str.is_number() )
			arr.erase( arr.begin());	// 2:waveform str

		size_t i = 0;
		for ( string str : arr )
		{
			if ( i < SPECARR_SIZE ) // ignore further entries
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


	char arrtype = Type_flag( arr[0] );
	spectrumTmp.adsr= adsr_type( arr[1] );//adsr;
	spectrumTmp.osc = Osc_TypeId( arr[1]);//oscid;
	assign_dta( arr, arrtype );
	Sum( spectrumTmp );
	return spectrumTmp;
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

void Spectrum_class::Show_spectrum_table(	fstream* f,
											const spectrum_t& spec,
											bool header )
{
	Table.opt.FILE 		= f;
	string OscRoleTag = typeNames[ spec.osc ];
	if( spec.adsr )
		OscRoleTag += "A";

	if ( header ) Table.PrintHeader();

	Table.AddRow( 	spectrumTag[SPEV], OscRoleTag,
		(int)spec.volidx[0], (int)spec.volidx[1], (int)spec.volidx[2], (int)spec.volidx[3],	(int)spec.volidx[4]);
	Table.AddRow( 	spectrumTag[SPEF], OscRoleTag,
		(int)spec.frqidx[0], (int)spec.frqidx[1], (int)spec.frqidx[2], (int)spec.frqidx[3],	(int)spec.frqidx[4]);
	Table.AddRow( 	spectrumTag[SPEW], OscRoleTag,
		(int)spec.wfid[0], (int)spec.wfid[1], (int)spec.wfid[2], (int)spec.wfid[3], (int)spec.wfid[4]);

}

string Spectrum_class::Show_spectrum_type( const int& _type, const spectrum_t& spec )
{
	stringstream strs{""};
	string OscRoleTag = typeNames[ spec.osc ];
	if( spec.adsr )
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

	String str { "SPEF,FMOA,1,2,3,4,5" };
	vector_str_t arr = str.to_array( ',' );
	Parse_data( arr );

	str = "SPEF,VCO,1,2,3,4,5";
	arr = str.to_array( ',' );
	Parse_data( arr );
	ASSERTION( 		spectrumTmp.frqidx[4] == 5, "spectrum.frqidx[4]",
			(int)	spectrumTmp.frqidx[4], 5 );
	ASSERTION(  ( abs(spectrumTmp.frqadj[4]) - 5.14286 ) < 1E-6, "frqadj4",
				( abs(spectrumTmp.frqadj[4])), 5.14286 );

	str = "SPEV,FMO,1.0,1,3,2,3" ;
	arr = str.to_array( ',' );
	Parse_data( arr );
	ASSERTION( spectrumTmp.osc == FMOID, "spectrum.osc", (int)spectrumTmp.osc, (int)FMOID)
//	cout << show_items( spectrum.vol) << endl;
	ASSERTION( ( abs( spectrumTmp.vol[1] - 1.0/10.0) < 1E-6),"",abs( spectrumTmp.vol[1] - 1.0/10.0), 0 );

	ASSERTION( strEqual( waveform_str_vec[3], waveFunction_vec[3].name),
			"waveform_str_vec",
			waveform_str_vec[3],
			waveFunction_vec[3].name );

	fstream f;
	f.open( "/tmp/Save_spectrum_table.txt", fstream::out );
	Show_spectrum_table( &f, this->spectrumTmp );
	f.close();

//assert(false);

	TEST_END( className );
}


