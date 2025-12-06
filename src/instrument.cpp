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
 * Instrument.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Config.h>
#include <Instrument.h>
#include <System.h>

Instrument_class::Instrument_class(	interface_t* _sds,
									Wavedisplay_class* _wd_p,
									file_structure* fs )
	: Logfacility_class("Instrument_class")
	, Device_class( _sds )
{
	this->sds 				= _sds;
	className				= Logfacility_class::className;

	this->wd_p 				= _wd_p;
	this->fs				= fs;
	Default_instrument_file = fs->instrumentdir + fs->default_snd;
	instr_ext 				= fs->snd_type;

	Oscgroup.SetWd( this->wd_p );
	selfTest();
}

Instrument_class::~Instrument_class()
{
	DESTRUCTOR( className )
}


void Instrument_class::selfTest()
{
	assert ( Oscgroup.osc.MemData_p() != nullptr );
	Assert_equal( Oscgroup.osc.typeId, OSCID, "type" );
	ASSERTION( fmo->spectrum.osc == FMOID, "fmo->spectrum.osc",
				(int) fmo->spectrum.osc, (int) FMOID );

	adsr_t adsr = fmo->Get_adsr();
	ASSERTION( adsr.spec.adsr == true, "fmo->adsr_data.spec.adsr",
				(int) adsr.spec.adsr, 1 );

}

void Instrument_class::update_sds()
{
	// use this function to update the ifd with the current set of shared information.
	// that are generated during the Set procedure, with the purpose to
	// update the SndlabGUI and to notify SDSview about the new data.

	Comment(INFO, "Update SDS data");
	for( char oscid : oscIds )
	{
		sds->adsr_arr[oscid]	= Oscgroup.member[oscid]->Get_adsr();
		sds->features[oscid] 	= Oscgroup.member[oscid]->features;
		sds->spectrum_arr[oscid]= Oscgroup.member[oscid]->spectrum;
	}

}

void Instrument_class::Update_osc_spectrum( char oscid )
{
	Oscillator* osc 	= Oscgroup.member[ oscid ];
	osc->spectrum		= sds->spectrum_arr[ oscid ] ;
}

void Instrument_class::Set_adsr( char oscid )
{
	Oscgroup.member[ oscid ]->Set_adsr	( sds->adsr_arr[ oscid ] );
}

void Instrument_class::init_data_structure( Oscillator* osc, vector_str_t arr  )
{
	osc->spectrum = default_spectrum;
	osc->spectrum.osc = osc->typeId;

	osc->Line_interpreter( arr );

	if ( osc->is_osc_type  )
		assign_adsr( arr );

}
void Instrument_class::set_new_name( string name )
{
	Name 				= name;
	Instrument_file 	= fs->instrumentdir + Name + instr_ext;
	Comment( INFO, "Instrument Name: " + Name );
}

void Instrument_class::set_name( string name )
{
	Instrument_file 	= fs->instrumentdir + name + instr_ext;
	if ( filesystem::exists( Instrument_file ) )
		Name 			= name;
	else
	{
		Name 			= fs->default_name;
		Instrument_file = Default_instrument_file;
	}
	Comment( INFO, "Instrument Name: " + Name );
}

bool Instrument_class::assign_adsr 	( vector_str_t arr )
{
	String	Str			{""};
	adsr_t 	adsr		= default_adsr;
	adsr.decay 			= Str.secure_stoi( arr[9 ]	);
	adsr.bps			= Str.secure_stoi( arr[10] 	);
	adsr.attack			= Str.secure_stoi( arr[11] 	);
	adsr.hall		= Str.secure_stoi( arr[12] 	);
	Oscgroup.osc.Set_adsr( adsr );

	return true;
}

/*
auto showOscfeatures2 = [  ]( Oscillator* osc, Oscillator* vco )
{
	Table_class Table { "Features" };
	Table.AddColumn( "Attack"	, 6 );
	Table.AddColumn( "Decay"	, 6 );
	Table.AddColumn( "Hall"		, 6 );
	Table.AddColumn( "Beats"	, 6 );
	Table.AddColumn( "SlideF"	, 6 );
	Table.AddColumn( "Pmw"		, 6 );
	Table.PrintHeader();
	adsr_t adsr = osc->Get_adsr();
	Table.AddRow(	(int)adsr.attack,
					(int)adsr.decay,
					(int)adsr.hall,
					(int)adsr.bps,
					(int)osc->features.glide_effect,
					(int)vco->features.PMW_dial);
};
*/

void Instrument_class::showOscfeatures( fstream* FILE )
{
	tableopt_t 	opt 			= defaultopt;
				opt.FILE 		= FILE;
				opt.Separator 	= ',';
	Table_class Table			{ opt };
	Table.AddColumn( "Type"		, 6 );
	Table.AddColumn( "Name"		, 6 );
	Table.AddColumn( "Hall"		, 6 );
	Table.AddColumn( "Beats"	, 6 );
	Table.AddColumn( "AttOSC"	, 6 );
	Table.AddColumn( "DecOSC"	, 6 );
	Table.AddColumn( "AttVCO"	, 6 );
	Table.AddColumn( "DecVCO"	, 6 );
	Table.AddColumn( "AttFMO"	, 6 );
	Table.AddColumn( "DecFMO"	, 6 );
	Table.AddColumn( "SlideF"	, 6 );
	if ( file_version == 4 )
		Table.AddColumn( "SlideV"	, 6 ) ;
	Table.AddColumn( "Pmw"		, 6 );
	Table.PrintHeader();
	adsr_t adsr_data = osc->Get_adsr();
	if (file_version == 3 )
		Table.AddRow(	"ADSR", "OSC",
						(int)adsr_data.hall,
						(int)adsr_data.bps,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)osc->features.glide_effect,
						(int)vco->features.PMW_dial);

	if (file_version == 4 )
		Table.AddRow(	"ADSR", "OSC",
						(int)adsr_data.hall,
						(int)adsr_data.bps,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)osc->features.glide_effect,
						(int)sds->slide_duration,
						(int)vco->features.PMW_dial);
};

bool Instrument_class::assign_adsr3( const vector_str_t& arr )
{
	String 	Str			{""};
	sds->adsr_arr[OSCID].bps	= Str.secure_stoi( arr[3] );
	sds->adsr_arr[OSCID].hall	= Str.secure_stoi( arr[2] );
	sds->adsr_arr[OSCID].attack= Str.secure_stoi( arr[4] );
	sds->adsr_arr[OSCID].decay	= Str.secure_stoi( arr[5] );

	sds->adsr_arr[VCOID].attack = Str.secure_stoi( arr[6]	);
	sds->adsr_arr[VCOID].decay	= Str.secure_stoi( arr[7] 	);
	sds->adsr_arr[FMOID].attack = Str.secure_stoi( arr[8]	);
	sds->adsr_arr[FMOID].decay	= Str.secure_stoi( arr[9] 	);

	sds->features[OSCID].glide_effect = Str.secure_stoi( arr[10] );
	if ( file_version == 4 )
	{
		sds->slide_duration			= Str.secure_stoi( arr[11] ); //Audioserver
		sds->features[VCOID].PMW_dial	= Str.secure_stoi( arr[12] );
	}
	else
	{
		sds->features[VCOID].PMW_dial	= Str.secure_stoi( arr[11] );
	}

	Oscgroup.SetAdsr( sds );
	Oscgroup.SetFeatures( sds );

	return true;

}
bool Instrument_class::assign_adsr2( const vector_str_t& arr )
{
	String 	Str			{""};
	adsr_t 	adsr		= default_adsr;
			adsr.decay 	= Str.secure_stoi( arr[2] );
			adsr.bps	= Str.secure_stoi( arr[3] );
			adsr.attack	= Str.secure_stoi( arr[4] );
			adsr.hall	= Str.secure_stoi( arr[5] );

			Set_sds_adsr( OSCID, adsr );
	return true;
}


int Instrument_class::getVersion( fstream& File )
{
	int version = -1;

	String Str{  "" };
	getline( File, Str.Str );
	vector_str_t arr = Str.to_array('=');

	if( strEqual( arr[0], "VERSION" ) )
		version = Str.secure_stoi( arr[1]);
	else
		version = 0;
	Comment( DEBUG, "File version: ", (int) version );
	if( supported.contains( version ) )
		return version;
	Comment( ERROR, "unsupported instrument file version: ", version );
	return -1;
}

bool Instrument_class::read_version1( fstream* File )
{
	String 			Str			{""};
	string 			keyword;
	vector_str_t 	arr 		{};
	Oscillator* 	osc 		= nullptr;

	getline( *File, Str.Str );
	do
	{
		Str.normalize();

		arr = Str.to_array( ',' );
		keyword = arr[0];
		if ( ( strEqual("OSC", keyword) ) or ( strEqual("TYPE", keyword)) ) // TYPE compatibility
		{
			osc = Oscgroup.Get_osc_by_name( arr[1] );
			init_data_structure( osc, arr );
		}
		if ( osc != nullptr)
		{	char TypeFlag	= osc->Type_flag( keyword );
			if ( TypeFlag >= 0 )
			{
					osc->spectrum = osc->Parse_data( arr );
					sds->spectrum_arr[ osc->typeId ] = osc->spectrum;
			}
		}

	} while( getline( *File, Str.Str));

	File->close();
	return true;
}


bool Instrument_class::read_version2( fstream* File )
{
	auto get_con = [ this ]( connectId_t con, OSCID_e secid, char mode )
	{
		switch (mode)
		{
			case CONF : { con.frq = secid; break; }
			case CONV : { con.vol = secid; break; }
			default : { break; }
		} // switch mode
		return con;
	};

	String 			Str		{""};
	vector_str_t 	arr 	{};
	string 			Type	{""};
	char		 	oscid 	= -1;

	Comment( DEBUG, "read_version2 ", Name );
	getline( *File, Str.Str );
	do
	{
		Str.normalize();
		arr = Str.to_array( ',' );
		string CfgType = arr[0];

		if( strEqual( "Type", CfgType ))
		{
			Type = ""; //ignore
		}
		else
		{
			string Name	= arr[1].substr(0,3);
			oscid 		= Oscgroup.Get_oscid_by_name( Name );
			Type		= CfgType.substr(0,3);
		}

		if ( ( strEqual("ADS", Type) ) )
		{
			if ( file_version == 2 )
				assign_adsr2( arr );
			if (( file_version == 3 ) or ( file_version == 4 ))
				assign_adsr3( arr );
		}
		if ( strEqual( "SPE", Type ))
		{
			spectrum_t spec_tmp = osc->Parse_data( arr ) ;
			if ( spec_tmp.adsr )
				sds->adsr_arr[oscid].spec = spec_tmp ;
			else
			{
				sds->spectrum_arr[oscid] = spec_tmp;
			}
		}
		if ( strEqual( "CON", Type ))
		{
			OSCID_e	secid 	= Oscgroup.Get_oscid_by_name( arr[2] );
			char 	mode 	= arr[3][0];
			connectId_t con_tmp = sds->connect_arr[oscid];
			sds->connect_arr[oscid] = get_con( con_tmp, secid, mode );
		}

	} while( getline( *File, Str.Str));

	return true;
}

bool Instrument_class::read_instrument( )
{
	fstream 	File;

	Comment( INFO, "using instrument file: " , Instrument_file);
	File.open(Instrument_file, fstream::in );
	if ( not File.is_open())
	{
		Comment(ERROR, "Input file: " + Instrument_file + " does not exist" );
		Comment(INFO, "using default instrument file: " + Default_instrument_file);
		Instrument_file = Default_instrument_file;
		File.open( Instrument_file, fstream::in );
		if ( not File.is_open())
		{
			Comment( ERROR, "Input file: " , Default_instrument_file , " does not exist" );
			Comment( ERROR, "Giving up");
			return false;
		}
	}

	bool code = false;
	file_version = getVersion( File );
	switch ( file_version )
	{
		case 0 :
		case 1 :
			code = read_version1( &File );
			break;
		case 2 :
		case 3 :
		case 4 :
			code = read_version2( &File );
			break;
		default :
			code = false;
			break;
	}




	return code;
}

bool Instrument_class::Connect( Oscillator* osc, Oscillator* sec, char mode )
{
	bool ret = true;
	switch ( mode )
	{
		case CONF 	:
		{
			osc->Connect_frq_data( sec );
			break;
		}
		case CONV 	:
		{
			osc->Connect_vol_data( sec );
			break;
		}
		default 	:
		{
			Comment( WARN, "unknown connect mode " + to_string( mode) );
			ret = false;
			break;
		}
	};
	Set_sds_connect_state(osc->typeId, osc->Connect );
//	Show_Connection_names( osc->typeId );
	assert( osc->Connect == Get_connect_state( osc->typeId ) );

	return ret;

}

void Instrument_class::save_features( fstream& FILE )
{
	tableopt_t opt = defaultopt;
	opt.FILE = &FILE;
	opt.Separator = ',';
	Table_class Table{ opt };
	Table.AddColumn("Type",	6 );
	Table.AddColumn("Name",	6 );
	Table.AddColumn("hall",	6 );
	Table.AddColumn("bps",	6 );
	Table.AddColumn("decay",6 );
	Table.AddColumn("atack",6 );
	Table.AddColumn("slide",6 );
	Table.AddColumn("pmw",	6 );
	Table.PrintHeader();

	// Type Features
	adsr_t adsr_data = osc->Get_adsr();
	Table.AddRow( "ADSR", "OSC",
			(int) adsr_data.decay,
			(int) adsr_data.bps,
			(int) adsr_data.attack,
			(int) adsr_data.hall,
			(int) osc->features.glide_effect,
			(int) vco->features.PMW_dial
			);

}

void Instrument_class::save_connections( fstream& FILE, Oscillator* osc )
{
	tableopt_t opt = defaultopt;
	opt.FILE = &FILE;
	opt.Separator = ',';
	Table_class Table{ opt };
	Table.AddColumn("Type",	6 );
	Table.AddColumn("Osc",	6 );
	Table.AddColumn("Sec",	6 );
	Table.AddColumn("Mode",	6 );
	Table.PrintHeader();

	// Type Connection
	Table.AddRow("CONN",
			osc->osctype_name,
			osc->fp.name,
			"F");
	Table.AddRow("CONN",
			osc->osctype_name,
			osc->vp.name,
			"V");


}
void Instrument_class::Save_Instrument( string str )
{
	set_new_name( str );
	Info( "saving sound to:", Instrument_file);

	fstream FILE;
	FILE.open(Instrument_file, fstream::out ); // overwrite the file content

	// Instrument file version
	FILE 	<< "VERSION=" << (int)actual_version << endl;
	file_version = actual_version;

	// Type ADSR
	showOscfeatures( &FILE );

	for ( Oscillator* osc : Oscgroup.member )
	{
		// Type SPE
		osc->Show_spectrum_table( &FILE, osc->spectrum );
		osc->Show_spectrum_table( &FILE, osc->Get_adsr().spec );
		// Type CONN
		save_connections( FILE, osc );
	}

	FILE.close();

	return ;
}

bool Instrument_class::Set( string name )
{
	set_name( name );

	Oscgroup.Connection_Reset();

	if ( read_instrument( ) )
	{
		Info( "sucessfully loaded instrument", name );
		showOscfeatures(); // stdout
		Oscgroup.Show_sound_stack();
	}
	else
	{
		Comment(ERROR, "cannot load instrument", name );
		return false;
	}

	Oscgroup.Data_Reset();
	Oscgroup.SetInstrument( sds );

	return true;
}

#include <numbers>

void Instrument_class::Test_Instrument()
{
	TEST_START( className );

	connectId_t connect = Get_connect_state( OSCID );
	assert( strEqual( osc->fp.name, typeNames[ OSCID ] ) );
	assert( strEqual( osc->vp.name, typeNames[ OSCID ] ) );
	Assert_equal( connect, default_connect( OSCID ), "connect_state" );

	assert( Set( ".test2" ) );
	assert( strEqual( osc->fp.name, typeNames[ FMOID ] ) );
	assert( strEqual( osc->vp.name, typeNames[ VCOID ] ) );
	ASSERTION( Get_connect_state( OSCID ).vol == sds->connect_arr[OSCID].vol , "connect_state",
			(int)Get_connect_state( OSCID ).vol, (int)sds->connect_arr[OSCID].vol );
	ASSERTION( Get_connect_state( OSCID ).frq == sds->connect_arr[OSCID].frq, "connect_state",
			(int)Get_connect_state( OSCID ).frq, (int)sds->connect_arr[OSCID].frq );
	Connect( fmo, fmo, CONF );
	Connect( fmo, fmo, CONV );



	showOscfeatures( );

	Save_Instrument( ".test2" );
	showOscfeatures( );
	ASSERTION( file_version == actual_version, "version", file_version , actual_version );

	Oscgroup.osc.Setwp_frames( min_msec );
	float f = Oscgroup.osc.GetFrq( Oscgroup.osc.spectrum.frqidx[0] );
	ASSERTION( f == (float)220, "frequency", f, 220.000000 );
	ASSERTION( fcomp( 	sds->spectrum_arr[OSCID].frqidx[0], 71), "frequency",
						sds->spectrum_arr[OSCID].frqidx[0], 71 );

	Oscgroup.osc.Phase_reset();
	Oscgroup.osc.Set_frequency( 71, FIXED ); // 220 Hz

	Oscgroup.Data_Reset();
	osc->Connection_reset();
	fmo->Set_volume(100,FIXED);
	vco->Set_volume(0,FIXED);
	Oscgroup.Show_sound_stack();
	Oscgroup.Run_OSCs(0);
	uint8_t past = Oscgroup.osc.DynFrequency.GetCurrent().past;
	assert( past == Oscgroup.osc.spectrum.frqidx[0]);
	float amp0 = Oscgroup.osc.MemData(0) ;
	for ( int n = 0; n < 9; n++ )
	{
		Oscgroup.Data_Reset();
		Oscgroup.Run_OSCs( 0 );
		float amp1 = Oscgroup.osc.MemData(0) ;
		cout << amp0 << " " << amp1 << " " << numbers::pi << endl;
		string com = "member" + to_string(n);
		ASSERTION( fcomp( amp0, amp1), com.data(),
					abs ( amp1- amp0 ), "~0" );
	}
	vco->Test();
	assert( Set( ".test2" ) );
	Oscgroup.vco.features.PMW_dial = 98;
	Oscgroup.vco.spectrum.wfid[0] = oscwaveform_struct::SGNSIN;
	Oscgroup.vco.Set_frequency( "A1", FIXED);
	assert( strEqual( 	waveform_str_vec[ oscwaveform_struct::SGNSIN ],
						Oscgroup.vco.Get_waveform_str( Oscgroup.vco.spectrum.wfid[0] )));

	Save_Instrument( ".test2" );
	Oscgroup.vco.features.PMW_dial = 0;
	Oscgroup.vco.spectrum.wfid[0] = oscwaveform_struct::RECTANGLE;
	assert( strEqual( 	waveform_str_vec[ oscwaveform_struct::RECTANGLE ],
						Oscgroup.vco.Get_waveform_str( Oscgroup.vco.spectrum.wfid[0] )));


	assert( Set( ".test2" ) );
	ASSERTION( 	sds->features[VCOID].PMW_dial == 98,"Set PMW_dial",
			(int)sds->features[VCOID].PMW_dial , 98);
	assert( Oscgroup.vco.features.PMW_dial == 98 );
	string a = waveform_str_vec[ oscwaveform_struct::SGNSIN ];
	string b = Oscgroup.vco.Get_waveform_str( Oscgroup.vco.spectrum.wfid[0] );
	ASSERTION( strEqual( a,b), "SGNSIN", a, b);

	assert( Oscgroup.osc.fp.Mem->Data == Oscgroup.fmo.MemData_p() );
//	assert( Oscgroup.osc.fp.data == Oscgroup.fmo.Mem.Data );
	Oscgroup.fmo.Set_frequency( C0, FIXED );
	f = Oscgroup.fmo.GetFrq( Oscgroup.fmo.spectrum.frqidx[0]);
	ASSERTION( fcomp( f, 16.3516 ), "" ,f, 16.3516 );


	assert( ( sin(1.0) - sin(1.0-2*numbers::pi) ) < 1E-6);

	adsr_t adsr = Oscgroup.osc.Get_adsr();
	ASSERTION( 	 adsr.hall == 20, "Oscgroup.osc.adsr_data.hall",
			(int)adsr.hall, 20 );
	ASSERTION( 	 adsr.bps == 1, "Oscgroup.osc.adsr_data.bps",
			(int)adsr.bps, 1);

	TEST_END( className );

}
