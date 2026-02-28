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
#include <fstream>

/**************************************************
 * Instrument_class
 *************************************************/
Instrument_class::Instrument_class(	Dataworld_class* data,
									Wavedisplay_class* _wd_p )
	: Logfacility_class	( "Instrument_class")
	, Interface_base	( data )
	, Device_class		( Interface_base::sds_p )
{
	this->wd_p 				= _wd_p;
	Default_instrument_file = fs->instrumentdir + fs->default_snd;
	instr_ext 				= fs->snd_type;
	Oscgroup.Set_Duration	( min_msec );
	Oscgroup.SetWd			( this->wd_p );
	selfTest				();
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
	for( OSCID_e oscid : oscIds )
	{
		sds_p->adsr_arr[oscid]	= Oscgroup.member[oscid]->Get_adsr();
		sds_p->features[oscid] 	= Oscgroup.member[oscid]->features;
		sds_p->spectrum_arr[oscid]= Oscgroup.member[oscid]->spectrum;
	}

}

void Instrument_class::Update_osc_spectrum( char oscid )
{
	Oscillator* osc 	= Oscgroup.member[ oscid ];
	osc->spectrum		= sds_p->spectrum_arr[ oscid ] ;
}

void Instrument_class::Set_adsr( char oscid )
{
	Oscgroup.member[ oscid ]->Set_adsr	( sds_p->adsr_arr[ oscid ] );
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
		Name 			= default_name;
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



void Instrument_class::showOscfeatures( ostream& FILE )
{
	tableopt_t 	opt 			= defaultopt;
				opt.FILE 		= &FILE;
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
						(int)osc->features.slide_frq,
						(int)vco->features.PWM);

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
						(int)osc->features.slide_frq,
						(int)sds_p->slide_duration,
						(int)vco->features.PWM);
};

bool Instrument_class::assign_adsr3( const vector_str_t& arr )
{
	String 	Str			{""};
	sds_p->adsr_arr[OSCID].bps	= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[OSCID].hall	= Str.secure_stoi( arr[2] );
	sds_p->adsr_arr[OSCID].attack	= Str.secure_stoi( arr[4] );
	sds_p->adsr_arr[OSCID].decay	= Str.secure_stoi( arr[5] );

	sds_p->adsr_arr[VCOID].attack = Str.secure_stoi( arr[6]	);
	sds_p->adsr_arr[VCOID].decay	= Str.secure_stoi( arr[7] 	);
	sds_p->adsr_arr[FMOID].attack = Str.secure_stoi( arr[8]	);
	sds_p->adsr_arr[FMOID].decay	= Str.secure_stoi( arr[9] 	);

	sds_p->features[OSCID].slide_frq = Str.secure_stoi( arr[10] );
	if ( file_version == 4 )
	{
		sds_p->slide_duration			= Str.secure_stoi( arr[11] ); //Audioserver
		sds_p->features[VCOID].PWM	= Str.secure_stoi( arr[12] );
	}
	else
	{
		sds_p->features[VCOID].PWM	= Str.secure_stoi( arr[11] );
	}

	Oscgroup.SetAdsr( sds_p );
	Oscgroup.SetFeatures( sds_p );

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

bool Instrument_class::read_version1( fstream& File )
{
	String 			Str			{""};
	string 			keyword;
	vector_str_t 	arr 		{};
	Oscillator* 	osc 		= nullptr;

	getline( File, Str.Str );
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
					sds_p->spectrum_arr[ osc->typeId ] = osc->spectrum;
			}
		}

	} while( getline( File, Str.Str));

	File.close();
	return true;
}


bool Instrument_class::read_version2( fstream& File )
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
	getline( File, Str.Str );
	do
	{
		Str.normalize();
		arr = Str.to_array( ',' );
		string CfgType = arr[0];

		if( strEqual( "Type", CfgType ))
		{
			Type = "noOSC"; //ignore
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
				sds_p->adsr_arr[oscid].spec = spec_tmp ;
			else
			{
				sds_p->spectrum_arr[oscid] = spec_tmp;
			}
		}
		if ( strEqual( "CON", Type ))
		{
			OSCID_e	secid 	= Oscgroup.Get_oscid_by_name( arr[2] );
			if( oscid == FMOID )
				secid = FMOID;
			char 	mode 	= arr[3][0];
			connectId_t con_tmp = sds_p->connect_arr[oscid];
			sds_p->connect_arr[oscid] = get_con( con_tmp, secid, mode );
		}

	} while( getline( File, Str.Str));

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
			code = read_version1( File );
			break;
		case 2 :
		case 3 :
		case 4 :
			code = read_version2( File );
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

	return ret;

}

void Instrument_class::save_features( ostream& FILE )
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
			(int) osc->features.slide_frq,
			(int) vco->features.PWM
			);

}

void Instrument_class::save_connections( ostream& FILE, Oscillator* osc )
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

void Instrument_class::Save( string str )
{
	set_new_name( str );
	Info( "saving sound to:", Instrument_file);

	fstream FILE	{ Instrument_file, fstream::out };

	// Instrument file version
	FILE 	<< "VERSION=" << (int)actual_version << endl;
	file_version = actual_version;

	// Type ADSR
	showOscfeatures( FILE );

	for ( OSCID_e oscid : oscIds )
	{
		// Type SPE
		osc->Show_spectrum_table( FILE, sds_p->spectrum_arr[oscid] );
		osc->Show_spectrum_table( FILE, sds_p->adsr_arr[oscid].spec );
		// Type CONN
		save_connections( FILE, Oscgroup.member[oscid] );
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
		showOscfeatures();
		Oscgroup.Show_sound_stack();
	}
	else
	{
		Comment(ERROR, "cannot load instrument", name );
		return false;
	}

	Oscgroup.Data_Reset();
	Oscgroup.SetInstrument( sds_p );

	return true;
}

#include <numbers>
#include <Oscwaveform.h>
void Instrument_class::Test_Instrument()
{
	TEST_START( className );

	connectId_t connect = Get_connect_state( OSCID );

	Assert_equal( strEqual( osc->fp.name, typeNames[OSCID]), true );
	Assert_equal( strEqual( osc->vp.name, typeNames[OSCID]), true );
	Assert_equal( connect, default_connect( OSCID ), "connect_state" );

	assert( Set( ".test2" ) );
	Assert_equal( strEqual( osc->fp.name, typeNames[FMOID] ), true );
	assert( strEqual( osc->vp.name, typeNames[VCOID]  ));
	ASSERTION( Get_connect_state( OSCID ).vol == sds_p->connect_arr[OSCID].vol , "connect_state",
			(int)Get_connect_state( OSCID ).vol, (int)sds_p->connect_arr[OSCID].vol );
	ASSERTION( Get_connect_state( OSCID ).frq == sds_p->connect_arr[OSCID].frq, "connect_state",
			(int)Get_connect_state( OSCID ).frq, (int)sds_p->connect_arr[OSCID].frq );



	showOscfeatures( );

	Save( ".test2" );
	showOscfeatures( );
	ASSERTION( file_version == actual_version, "version", file_version , actual_version );

	Oscgroup.osc.Setwp_frames( min_msec );
	float f = frqArray[ Oscgroup.osc.spectrum.frqidx[0] ];
	ASSERTION( f == (float)220, "frequency", f, 220.000000 );
	Assert_equal( (int)sds_p->spectrum_arr[OSCID].frqidx[0], 71, "frequency" );

	Oscgroup.osc.Phase_reset();
	Oscgroup.osc.Set_frequency( 71, FIXED ); // 220 Hz

	Oscgroup.Data_Reset();
	osc->Connection_reset();
	fmo->Set_spectrum_volume(100);
	vco->Set_spectrum_volume(0);
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
	vco->features.PWM = 98;
	vco->spectrum.wfid[0] = SGNSIN;
	vco->Set_frequency( "A1", FIXED);
	Assert_equalstr( 	waveform_str_vec[ SGNSIN ],
						vco->Get_waveform_str( vco->spectrum.wfid[0] )) ;

	Save( ".test2" );
	vco->features.PWM = 0;
	vco->spectrum.wfid[0] = RECTANGLE;
	Assert_equalstr( 	waveform_str_vec[ RECTANGLE ],
						vco->Get_waveform_str( vco->spectrum.wfid[0] ));


	assert( Set( ".test2" ) );
	ASSERTION( 	sds_p->features[VCOID].PWM == 98,"Set PMW_dial",
			(int)sds_p->features[VCOID].PWM , 98);
	Assert_equal( vco->features.PWM, uint8_t(98) );
	string a = waveform_str_vec[ SGNSIN ];
	string b = vco->Get_waveform_str( vco->spectrum.wfid[0] );
	ASSERTION( strEqual( a,b), "SGNSIN", a, b);

	assert( Oscgroup.osc.fp.Mem->Data == fmo->MemData_p() );
//	assert( Oscgroup.osc.fp.data == fmo->Mem.Data );
	fmo->Set_frequency( C0, FIXED );
	f = frqArray[ fmo->spectrum.frqidx[0] ];
	ASSERTION( fcomp( f, 16.3516 ), "" ,f, 16.3516 );


	assert( ( sin(1.0) - sin(1.0-2*numbers::pi) ) < 1E-6);

	adsr_t adsr = Oscgroup.osc.Get_adsr();
	ASSERTION( 	 adsr.hall == 20, "Oscgroup.osc.adsr_data.hall",
			(int)adsr.hall, 20 );
	ASSERTION( 	 adsr.bps == 1, "Oscgroup.osc.adsr_data.bps",
			(int)adsr.bps, 1);

	TEST_END( className );

}
