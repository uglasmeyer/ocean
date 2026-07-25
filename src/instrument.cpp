/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
#include <Fourier.h>

/**************************************************
 * Instrument_class
 *************************************************/
Instrument_class::Instrument_class(	Dataworld_class* data,
									Wavedisplay_class* _wd_p )
	: Logfacility_class	( "Instrument_class")
	, Interface_base	( data )
	, Device_class		( Interface_base::sds_p )
	, Oscgroup			( INSTRROLE, monobuffer_bytes )
	, Instrument_name	( &sds_p->Instrument, SDSNAME_SIZE )
	, instruments		( { fs->instrumentdir }, fs->snd_type )
{
	this->wd_p 					= _wd_p;
	this->osc					= &Oscgroup.osc;
	this->vco					= &Oscgroup.vco;
	this->fmo					= &Oscgroup.fmo;
	this->Instrument_name.read	();
	std::ranges::for_each		( Oscgroup.member, [ this ]( Oscillator* osc )
		{ osc->Set_beatclock( &sds_p->instrumentClock ); } );

	SetDir						( sds_master->Session );
	select						( sds_p->Instrument );
	Oscgroup.SetDuration		( min_msec );
	Oscgroup.SetWd				( this->wd_p );
	selfTest					();
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

bool Instrument_class::Sync( bool composer )
{
	bool	sync_adsr 					= Oscgroup.Sync();
			sds_p->mixer_state.sync_adsr= sync_adsr;

	return	( composer ? true : sync_adsr );
}
void Instrument_class::Update_osc_spectrum( char oscid )
{
	Oscillator* osc 	= Oscgroup.member[ oscid ];
	osc->spectrum		= sds_p->spectrum_arr[ oscid ] ;
}

void Instrument_class::init_data_structure( Oscillator* osc, string_vec_t arr  )
{
	osc->spectrum = default_spectrum;
	osc->spectrum.osc = osc->typeId;

	osc->Line_interpreter( arr );

	assign_adsr( osc, arr );

}
bool Instrument_class::set_new_name( string name )
{
	if ( name.length() == 0 )
		return false;

	instruments.Create( name );
	select( name );
	return true;
}

void Instrument_class::select( string name )
{
	if( sds_master->mixer_state.composer )
		instrument_file = instruments.Select_strong( name, name );
	else
		instrument_file = instruments.Select( name, default_name );

	Instrument_name.save( name );
	fourierBinFile		= fourier_dir + name + fs->fft_type;
	Info				( "Instrument Name: ", name );
}
void Instrument_class::SetDir( string subdir )
{
	instrument_dir			= fs->SubDir( fs->instrumentdir, subdir );
	fourier_dir				= fs->SubDir( fs->fourierdir, subdir 	);
	instruments.UpdatePath	( { instrument_dir, fs->instrumentdir } );
//	select					( Name );
}

bool Instrument_class::assign_adsr 	( Oscillator* osc, string_vec_t arr )
{
	String	Str			{""};
	adsr_t 	adsr		= default_adsr;
	adsr.decay 			= Str.secure_stoi( arr[9 ]	);
	adsr.bps			= Str.secure_stoi( arr[10] 	);
	adsr.attack			= Str.secure_stoi( arr[11] 	);
	adsr.hall			= Str.secure_stoi( arr[12] 	);
	osc->Set_adsr		( adsr );

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
	Table.AddColumn( "Beats"	, 6 );
	Table.AddColumn( "Hall"		, 6 );
	Table.AddColumn( "Attack"	, 6 );
	Table.AddColumn( "Decay"	, 6 );
	Table.AddColumn( "SlideF"	, 6 );
	Table.AddColumn( "Pmw"		, 6 );
	Table.AddColumn( "SlideV"	, 6 );
	Table.AddColumn( "Follow"	, 6 );  // feature combine
	Table.AddColumn( "Adjust"	, 6 );
	Table.PrintHeader();

	for( Oscillator* osc : Oscgroup.member )
	{
		adsr_t adsr_data = osc->Get_adsr();
		osc->features = sds_p->features[osc->typeId];
		Table.AddRow(	"ADSR", osc->osctype_name,
						(int)adsr_data.bps,
						(int)adsr_data.hall,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)osc->features.frq_slide_value,
						(int)vco->features.PWM,
						(int)sds_master->Master_vol_slide_value,
						(int)osc->features.combine,
						(int)osc->features.adjust );
	}

}


bool Instrument_class::assign_adsr5( OSCID_e oscid, const string_vec_t& arr )
{
	String 	Str							{""};
	sds_p->adsr_arr[oscid].bps			= Str.secure_stoi( arr[2] );
	sds_p->adsr_arr[oscid].hall			= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[oscid].attack		= Str.secure_stoi( arr[4] );
	sds_p->adsr_arr[oscid].decay		= Str.secure_stoi( arr[5] );
	sds_p->features[oscid].frq_slide_value	= Str.secure_stoi( arr[6] );
	sds_p->features[oscid].PWM			= Str.secure_stoi( arr[7] );
	sds_master->Master_vol_slide_value	= Str.secure_stoi( arr[8] ); //Audioserver
	if( arr.size() > 9 )
		sds_p->features[oscid].combine	= (bool)Str.secure_stoi( arr[9] );
	if( arr.size() > 10 )
		sds_p->features[oscid].adjust	= (uint8_t)Str.secure_stoi( arr[10] );

	return true;
}
bool Instrument_class::assign_adsr3( const string_vec_t& arr )
{
	String 	Str			{""};
	sds_p->adsr_arr[OSCID].bps	= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[OSCID].hall	= Str.secure_stoi( arr[2] );
	sds_p->adsr_arr[OSCID].attack= Str.secure_stoi( arr[4] );
	sds_p->adsr_arr[OSCID].decay	= Str.secure_stoi( arr[5] );

	sds_p->adsr_arr[VCOID].bps	= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[VCOID].attack = Str.secure_stoi( arr[6]	);
	sds_p->adsr_arr[VCOID].decay	= Str.secure_stoi( arr[7] 	);

	sds_p->adsr_arr[FMOID].bps	= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[FMOID].attack = Str.secure_stoi( arr[8]	);
	sds_p->adsr_arr[FMOID].decay	= Str.secure_stoi( arr[9] 	);

	sds_p->features[OSCID].frq_slide_value	= Str.secure_stoi( arr[10] );
	if ( file_version == 4 )
	{
		sds_master->Master_vol_slide_value	= Str.secure_stoi( arr[11] ); //Audioserver
		sds_p->features[VCOID].PWM			= Str.secure_stoi( arr[12] );
	}
	else
	{
		sds_p->features[VCOID].PWM	= Str.secure_stoi( arr[11] );
	}

	Oscgroup.SetAdsr( sds_p );
	Oscgroup.SetFeatures( sds_p );

	return true;

}
bool Instrument_class::assign_adsr2( const string_vec_t& arr )
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
	int 			version = -1;
	String 			Str		{ "" };
	string_vec_t 	arr 	{};

	getline( File, Str.Str );
	do
	{
		arr = Str.to_array('=');
		if( strEqual( arr[0], "VERSION" ) )
			version = Str.secure_stoi( arr[1]);

	} while ( ( version < 0 ) and ( getline( File, Str.Str ) ) );


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
	string_vec_t 	arr 		{};
	Oscillator* 	osc 		= nullptr;

	getline( File, Str.Str );
	do
	{
		Str.normalize();

		arr = Str.to_array( ',' );
		keyword = arr[0];
		if ( ( strEqual("OSC", keyword) ) or ( strEqual("TYPE", keyword)) ) // TYPE compatibility
		{
			osc = Oscgroup.GetOsc_byName( arr[1] );
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


bool Instrument_class::assign_sds( fstream& READ )
{

	const set<string> 	valid	= { "ADSR", "SPEV", "SPEF", "SPEW", "CONN" };
	String 				Str		{""};
	string_vec_t 		arr 	{};
	string 				Type	{""};
	OSCID_e		 		oscid 	= NOOSCID;

	Comment( DEBUG, "read_version2 ", Instrument_name.get() );
	getline( READ, Str.Str );

	do
	{
		Str.normalize		();
		arr 				= Str.to_array( ',' );

		Type 				= "invalid";
		if( valid.contains( arr[0] ))
		{
			Type		= arr[0].substr(0,3);
			string Name	= arr[1].substr(0,3);
			oscid 		= Oscgroup.GetOscid_byName( Name );
		}

		if ( ( strEqual("ADS", Type) ) )
		{
			if ( file_version == 2 )
				assign_adsr2( arr );
			if (( file_version == 3 ) or ( file_version == 4 ))
				assign_adsr3( arr );
			if (( file_version == 5 ) or ( file_version == 6 ))
				assign_adsr5( oscid, arr );
		}
		if ( strEqual( "SPE", Type ))
		{
			spectrum_t spec_tmp = osc->Parse_data( arr ) ;
			if ( spec_tmp.adsr )
			{
				sds_p->adsr_arr[oscid].spec = spec_tmp ;
			}
			else
			{
				sds_p->spectrum_arr[oscid] = spec_tmp;
	}
		}
		if ( strEqual( "CON", Type ))
		{
			OSCID_e	secid 	= Oscgroup.GetOscid_byName( arr[2] );
			char 	mode 	= arr[3][0];
			connect( oscid, secid, mode );
		}

	} while( getline( READ, Str.Str));

	return true;
}

bool Instrument_class::read_instrument( )
{
	fstream 	File;

	Comment( INFO, "using instrument file: " , instrument_file);
	File.open(instrument_file, fstream::in );
	if ( not File.is_open())
	{
		Comment(ERROR, "Input file: " + instrument_file + " does not exist" );
		Comment(INFO, "using default instrument file: " + fs->instrument_file);
		instrument_file = fs->instrument_file;
		Instrument_name.set( default_name );
		File.open( instrument_file, fstream::in );
		if ( not File.is_open())
		{
			Comment( ERROR, "Input file: " , fs->instrument_file , " does not exist" );
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
		{
			code = read_version1( File );
			break;
		}
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
		{
			code = assign_sds( File );
			break;
		}
		default :
		{
			code = false;
			break;
		}
	}
	if( code )
		fourier->Load_coeff	( fourierBinFile );

	return code;

}

void Instrument_class::connect( OSCID_e oscid, OSCID_e secid, char mode )
{
	Connect( Oscgroup.member[oscid], Oscgroup.member[secid], mode );
	switch (mode )
	{
		case CONF : { sds_p->connect_arr[oscid].frq = secid; break; }
		case CONV : { sds_p->connect_arr[oscid].vol = secid; break; }
		default : { break; }
	} // switch mode
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

void Instrument_class::Save( string name )
{
	if( not	set_new_name(  name ) )
		return;

	Info( "saving sound to:", instrument_file );

	fstream FILE	{ instrument_file, fstream::out };

	// Instrument file version
	file_version = actual_version;
	FILE 	<< "VERSION=" << file_version << endl;

	// Type ADSR
	showOscfeatures( FILE );
	FILE << Line(80) << endl;

	for ( OSCID_e oscid : oscIds )
	{
		// Type SPE
		osc->Show_spectrum_table( FILE, sds_p->spectrum_arr[oscid] );
		osc->Show_spectrum_table( FILE, sds_p->adsr_arr[oscid].spec );
		// Type CONN
		save_connections( FILE, Oscgroup.member[oscid] );
		FILE << Line(80) << endl;
}

	FILE.close();
	fourier->Save_coeff	( fourierBinFile );

	return ;
}

bool Instrument_class::Set()
{
	Instrument_name.read();
	return Set( Instrument_name.get() );
}

bool Instrument_class::Set( string name )
{
	Info	("receive command <set instrument>", name );
	select	( name );

	Oscgroup.ConnectionReset();

	if ( read_instrument( ) )
	{
		Info( "sucessfully loaded instrument", name );
		file_version = actual_version;
		showOscfeatures();
		Oscgroup.ShowSoundStack();
	}
	else
	{
		Comment(ERROR, "cannot load instrument", name );
		return false;
	}

	Oscgroup.DataReset			();
	Oscgroup.SetInstrument		( sds_p );
	Oscgroup.SetSliderFrequency	( sds_p,
									sds_p->spectrum_arr[OSCID].frqidx[0],
									sds_p->frq_slidermode );

	return true;
}

#include <Oscwaveform.h>
#include <Default.h>
void Instrument_class::Test_Instrument()
{
	TEST_START( className );

	SetDir( "" );
	Write_template_text( fs->testsnd_file, test_instrument_text, OVERWRITE );
	connectId_t connect = Get_connect_state( OSCID );

	Assert_equal( osc->fp.name, typeNames[OSCID] );
	Assert_equal( osc->vp.name, typeNames[OSCID] );
	Assert_equal( connect, default_connect( OSCID ), "connect_state" );

	assert( Set( fs->test_name ) );
	Assert_equal( osc->fp.name, typeNames[FMOID] );
	Assert_equal( osc->vp.name, typeNames[VCOID] );
	ASSERTION( Get_connect_state( OSCID ).vol == sds_p->connect_arr[OSCID].vol , "connect_state",
			(int)Get_connect_state( OSCID ).vol, (int)sds_p->connect_arr[OSCID].vol );
	ASSERTION( Get_connect_state( OSCID ).frq == sds_p->connect_arr[OSCID].frq, "connect_state",
			(int)Get_connect_state( OSCID ).frq, (int)sds_p->connect_arr[OSCID].frq );


	Save( fs->test_name );
	showOscfeatures();
	ASSERTION( file_version == actual_version, "version", file_version , actual_version );

	Oscgroup.osc.Setwp_frames( min_msec );
	float f = frqArray[ Oscgroup.osc.spectrum.frqidx[0] ];
	ASSERTION( f == (float)220, "frequency", f, 220.000000 );
	Assert_equal( (int)sds_p->spectrum_arr[OSCID].frqidx[0], 71, "frequency" );

	Oscgroup.osc.Phase_reset();
	Oscgroup.osc.Set_frequency( 71, FIXED ); // 220 Hz

	Oscgroup.DataReset();
	osc->Connection_reset();
	fmo->Set_spectrum_volume(100);
	vco->Set_spectrum_volume(0);
	Oscgroup.ShowSoundStack();
	Oscgroup.RunOSCs(0);
	uint8_t past = Oscgroup.osc.DynFrequency.GetCurrent().past;
	assert( past == Oscgroup.osc.spectrum.frqidx[0]);
	float amp0 = Oscgroup.osc.MemData(0) ;
	for ( int n = 0; n < 9; n++ )
	{
		Oscgroup.DataReset();
		Oscgroup.RunOSCs( 0 );
		float amp1 = Oscgroup.osc.MemData(0) ;
		cout << amp0 << " " << amp1 << " " << numbers::pi << endl;
		string com = "member" + to_string(n);
		ASSERTION( fcomp( amp0, amp1), com.data(),
					abs ( amp1- amp0 ), "~0" );
	}
	vco->Test();
	assert( Set( fs->test_name ) );
	vco->features.PWM = 98;
	vco->spectrum.wfid[0] = SGNSIN;
	vco->Set_frequency( "A1", FIXED);
	Assert_equal( 	waveform_str_vec[ SGNSIN ],
						vco->Get_waveform_str( vco->spectrum.wfid[0] )) ;

	Save( fs->test_name );
	vco->features.PWM = 0;
	vco->spectrum.wfid[0] = RECTANGLE;
	Assert_equal( 	waveform_str_vec[ RECTANGLE ],
						vco->Get_waveform_str( vco->spectrum.wfid[0] ));


	assert( Set( fs->test_name ) );
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
