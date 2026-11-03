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
 * instrument.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Config.h>
#include <Instrument.h>
#include <System.h>
#include <fstream>
#include <Fourier.h>
#include <Spectrum.h>
#include <Default.h>

/**************************************************
 * Instrument_class
 *************************************************/
using namespace fourier_ns;

Instrument_class::Instrument_class(	Dataworld_class* data,
									Wavedisplay_class* _wd_p )
	: Logfacility_class	( "Instrument_class")
	, Interface_base	( data )
	, Device_class		( Interface_base::sds_p )
//	, Oscgroup			( INSTRROLE, monobuffer_bytes )
	, Instrument_name	( &sds_p->Instrument, SDSNAME_SIZE )
	, instruments		( { fs->instrumentdir }, fs->snd_type )
{
	this->wd_p 					= _wd_p;
	this->osc					= &Oscgroup.osc;
	this->vco					= &Oscgroup.vco;
	this->fmo					= &Oscgroup.fmo;
	this->fftFilter				= sds_p->fourier.filter;
	this->Instrument_name.read	();
	for_each					( Oscgroup.member, [ this ]( Oscillator* osc )
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

	adsr_data_t adsr = fmo->Get_adsr();
	Assert_equal( adsr.spec.type, spectrum_ns::ADSR );

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
	osc->spectrum.frqidx= sds_p->spectrum_arr[ oscid ].frqidx;
	osc->spectrum.volidx= sds_p->spectrum_arr[ oscid ].volidx;
	osc->spectrum.wfid	= sds_p->spectrum_arr[ oscid ].wfid;
}

void Instrument_class::init_data_structure( Oscillator* osc, string_vec_t arr  )
{
	osc->spectrum = default_spectrum;
	osc->spectrum.osc = osc->typeId;

	osc->Line_interpreter( arr );

	assign_adsr( osc, arr );

}

void Instrument_class::select( string name )
{
	instrument_file		= ( sds_master->mixer_state.composer )
						? instruments.Select_strong( name, name )
						: instruments.Select( name, default_name );

	Instrument_name.save( name );
	fftBinFile			= fourier_dir + name + fs->fft_type;
	Info				( "Instrument Name: ", name );
}
void Instrument_class::SetDir( string subdir )
{
	instrument_dir			= fs->SubDir( fs->instrumentdir, subdir );
	fourier_dir				= fs->SubDir( fs->fourierdir, subdir 	);
	instruments.UpdatePath	( { instrument_dir, fs->instrumentdir } );
}

bool Instrument_class::assign_adsr 	( Oscillator* osc, string_vec_t arr )
{
	String		Str	{""};
	adsr_data_t adsr= default_adsr;
	adsr.decay 		= Str.secure_stoi( arr[9 ]	);
	adsr.bps		= Str.secure_stoi( arr[10] 	);
	adsr.attack		= Str.secure_stoi( arr[11] 	);
	adsr.hall		= Str.secure_stoi( arr[12] 	);
	osc->Set_adsr	( adsr );

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
		adsr_data_t adsr_data = osc->Get_adsr();
		osc->features = sds_p->features[osc->typeId];
		Table.AddRow(	"ADSR", osc->osctype_name,
						(int)adsr_data.bps,
						(int)adsr_data.hall,
						(int)adsr_data.attack,
						(int)adsr_data.decay,
						(int)osc->features.frq_slide_value,
						(int)osc->features.PWM,
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
	Sds->frq_slide.Set					( Str.secure_stoi( arr[6] ));
	sds_p->features[oscid].PWM			= Str.secure_stoi( arr[7] );

	if( arr.size() > 9 )
		sds_p->features[oscid].combine	= (bool)Str.secure_stoi( arr[9] );
	if( arr.size() > 10 )
		sds_p->features[oscid].adjust	= (uint8_t)Str.secure_stoi( arr[10] );

	return true;
	//	sds_master->Master_vol_slide_value	= Str.secure_stoi( arr[8] );
	//	Audioserver is not part of the instrument
}

bool Instrument_class::assign_adsr3( const string_vec_t& arr )
{
	String 	Str								{""};
	sds_p->adsr_arr[OSCID].bps				= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[OSCID].hall				= Str.secure_stoi( arr[2] );
	sds_p->adsr_arr[OSCID].attack			= Str.secure_stoi( arr[4] );
	sds_p->adsr_arr[OSCID].decay			= Str.secure_stoi( arr[5] );

	sds_p->adsr_arr[VCOID].bps				= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[VCOID].attack 			= Str.secure_stoi( arr[6]	);
	sds_p->adsr_arr[VCOID].decay			= Str.secure_stoi( arr[7] 	);

	sds_p->adsr_arr[FMOID].bps				= Str.secure_stoi( arr[3] );
	sds_p->adsr_arr[FMOID].attack			= Str.secure_stoi( arr[8]	);
	sds_p->adsr_arr[FMOID].decay			= Str.secure_stoi( arr[9] 	);

	Sds->frq_slide.Set						( Str.secure_stoi( arr[10] ));
	sds_p->features[VCOID].PWM 				= ( file_version == 4 )
											? Str.secure_stoi( arr[12] )
											: Str.secure_stoi( arr[11] );

	Oscgroup.SetAdsr						( sds_p->adsr_arr );
	Oscgroup.SetFeatures					( sds_p->features );

	return true;
	//	sds_master->Master_vol_slide_value	= Str.secure_stoi( arr[11] ); //Audioserver
	//	Audioserver is not part of the instrument
}
bool Instrument_class::assign_adsr2( const string_vec_t& arr )
{
	String 		Str		{""};
	adsr_data_t	adsr	= default_adsr;
	adsr.decay			= Str.secure_stoi( arr[2] );
	adsr.bps			= Str.secure_stoi( arr[3] );
	adsr.attack			= Str.secure_stoi( arr[4] );
	adsr.hall			= Str.secure_stoi( arr[5] );
	Set_sds_adsr		( OSCID, adsr );
	return true;
}

int Instrument_class::getVersion( fstream& File )
{
	int 			version = -1;
	String 			Str		{ "" };
	string_vec_t	arr 	{};

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
	auto connect = [ this ]( OSCID_e oscid, OSCID_e secid, char mode )
	{
		Connect( Oscgroup.member[oscid], Oscgroup.member[secid], mode );
		switch (mode )
		{
			case CONF : { sds_p->connect_arr[oscid].frq = secid; break; }
			case CONV : { sds_p->connect_arr[oscid].vol = secid; break; }
			default : { break; }
		} // switch mode
	};

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
			switch ( file_version )
			{
				case 2	: { assign_adsr2( arr ); 		break; }
				case 3	:
				case 4	: { assign_adsr3( arr );		break; }
				case 5	:
				case 6	: { assign_adsr5( oscid, arr ); break; }
				default : { break; }
			} // switch  file_version
		}
		if ( strEqual( "SPE", Type ))
		{
			spectrum_t spec_tmp 	= ( arr[0].length() < 4 )
									? osc->Parse_data( arr )
									: osc->ADSR_class::Parse_data( arr );
			spectrum_t& sds_spec	= ( spec_tmp.type == spectrum_ns::ADSR )
									? sds_p->adsr_arr[oscid].spec
									: sds_p->spectrum_arr[oscid];
			sds_spec				= spec_tmp;
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

void Instrument_class::Gen_fftAmplitude()
{
	fourier_data_t*	fft= &sds_p->fourier;

	auto gen_fftAmplitude = [ this, fft  ]( Fourier_class* fourier  )
	{
		fourier->Amplitude		( sds_p->wavedata );	// wave data is input data
		fourier->SetResolution	( sds_p->fourier.residx );
		fourier->GenCoeff		( sds_p->fourier );
	};

	if( fft->trigger ) 	// is true if the spectrum dialog fourier button is activated
	{	// shows fft amplitudes and generates fft coefficients
		fft->trigger 			= false;
		OSCID_e			oscid	= sds_p->WD_state.oscId;
		if( oscid == FMOID )	return;
		Oscillator*		osc		= Oscgroup.member[oscid];
		bool			adsrmode= sds_p->WD_state.adsrmode;
		Fourier_class*	fourier	= adsrmode 	? &osc->ADSR_class::fourier
											: &osc->Oscillator::fourier ;
		if( fft->filter == SHIFT )
		{
			fft->filter			= fftFilter;
			fourier->Shift_Coeff( fft->shift );
		}
		else
		{
			gen_fftAmplitude	( fourier );
			fftFilter			= fft->filter;
		}
		fourier->Write_fftdata	( sds_p->fft_data );
	}
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

	bool readed = false;
	file_version = getVersion( File );
	switch ( file_version )
	{
		case 0 :
		case 1 :
		{
			readed = read_version1( File );
			break;
		}
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
		{
			readed = assign_sds( File );
			break;
		}
		default :
		{
			readed = false;
			break;
		}
	}

	return readed;
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

void Instrument_class::show_connections( ostream& FILE, Oscillator* osc )
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
	auto set_new_name = [ this ]( string name )
	{
		if ( name.length() == 0 )
			return false;

		instruments.Create( name );
		select( name );
		return true;
	};

	if( not	set_new_name(  name ) )
		return;

	Info( "saving sound to:", instrument_file );


	// Instrument file version
	file_version 	= actual_version;
	fstream FILE	{ instrument_file, fstream::out };
	FILE 			<< "VERSION=" << file_version << endl;

	// Type ADSR
	showOscfeatures( FILE );
	FILE << Line(80) << endl;

	for ( OSCID_e oscid : oscIds )
	{
		// Type SPE
		osc->Show_spectrum_table( FILE, sds_p->spectrum_arr[oscid] );
		osc->Show_spectrum_table( FILE, sds_p->adsr_arr[oscid].spec );
		// Type CONN
		show_connections( FILE, Oscgroup.member[oscid] );
		FILE << Line(80) << endl;
	}

	FILE.close();

	Oscgroup.Save_fft_coeff( fftBinFile );
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
	Oscgroup.Read_fft_coeff		( fftBinFile );
	sds_p->fourier.size			= osc->Oscillator::fourier.GetResolution();
	Oscgroup.SetInstrument		( sds_p );
	Oscgroup.SetSliderFrequency	( sds_p );

	return true;
}

void Instrument_class::Test_Instrument()
{
	osc->fourier.TestFourier();

	TEST_START( className );

	SetDir( "" );

	osc->Set_connection( osc, osc );
	Assert_equal( osc->fp.name, typeNames[OSCID] );
	Assert_equal( osc->vp.name, typeNames[OSCID] );

	Save_connect_state( OSCID, default_connect( OSCID ) );
	connectId_t connect = Read_connect_state( OSCID );
	Assert_equal( connect, default_connect( OSCID ), "connect_state" );

	Write_template_text( fs->testsnd_file, test_instrument_text, OVERWRITE );
	assert( Set( fs->test_name ) );
	Assert_equal( osc->fp.name, typeNames[FMOID] );
	Assert_equal( osc->vp.name, typeNames[VCOID] );
	ASSERTION( Read_connect_state( OSCID ).vol == sds_p->connect_arr[OSCID].vol , "connect_state",
			(int)Read_connect_state( OSCID ).vol, (int)sds_p->connect_arr[OSCID].vol );
	ASSERTION( Read_connect_state( OSCID ).frq == sds_p->connect_arr[OSCID].frq, "connect_state",
			(int)Read_connect_state( OSCID ).frq, (int)sds_p->connect_arr[OSCID].frq );


	Remove_file( fs->test_name );
	Save( fs->test_name );
	Set ( fs->test_name );
	ASSERTION( file_version == actual_version, "version", file_version , actual_version );
	Assert_equal( (int)sds_p->spectrum_arr[OSCID].wfid[1], 2 );

	Oscgroup.osc.Setwp_frames( min_msec );
	float f = frqArray[ Oscgroup.osc.spectrum.frqidx[0] ];
	ASSERTION( f == (float)220, "frequency", f, 220.000000 );
	Assert_equal( (int)sds_p->spectrum_arr[OSCID].frqidx[0], 71, "frequency" );

	Oscgroup.osc.Phase_reset();
	Oscgroup.osc.Set_frequency( 71, FIXED ); // 220 Hz

	Oscgroup.DataReset();
	osc->Reset_connections();
	fmo->Set_volume(100);
	vco->Set_volume(0);
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
		cout << amp0 << " " << amp1 << " " << pi << endl;
		string com = "member" + to_string(n);
		ASSERTION( fcomp( amp0, amp1), com.data(),
					abs ( amp1- amp0 ), "~0" );
	}
	vco->Test();
	assert( Set( fs->test_name ) );
	vco->features.PWM = 98;
	vco->spectrum.wfid[0] = SGNSIN;
	vco->Set_frequency( "A1", FIXED);
	Assert_equal( 	waveform->Get_str( SGNSIN ),
						waveform->Get_str( vco->spectrum.wfid[0] )) ;

	Save( fs->test_name );
	vco->features.PWM = 0;
	vco->spectrum.wfid[0] = RECTANGLE;


	assert( Set( fs->test_name ) );
	ASSERTION( 	sds_p->features[VCOID].PWM == 98,"Set PMW_dial",
			(int)sds_p->features[VCOID].PWM , 98);
	Assert_equal( vco->features.PWM, uint8_t(98) );
	string a = waveform->Get_str( TRIANGLE );
	string b = waveform->Get_str( vco->spectrum.wfid[0] );
	Assert_equal( a, b );

	assert( Oscgroup.osc.fp.Mem->Data == fmo->MemData_p() );
	fmo->Set_frequency( C0, FIXED );
	f = frqArray[ fmo->spectrum.frqidx[0] ];
	ASSERTION( fcomp( f, 16.3516 ), "" ,f, 16.3516 );


	assert( ( sin(1.0) - sin(1.0-2*pi) ) < 1E-6);

	adsr_data_t adsr = Oscgroup.osc.Get_adsr();
	ASSERTION( 	 adsr.hall == 20, "Oscgroup.osc.adsr_data.hall",
			(int)adsr.hall, 20 );
	ASSERTION( 	 adsr.bps == 1, "Oscgroup.osc.adsr_data.bps",
			(int)adsr.bps, 1);

	for( uint8_t frq : { A3, A2, A4 })//, A2 } )
	{
		spectrum_t	spectrum	= osc->TestSpectrum;
		adsr_data_t test_adsr	= default_adsr;
		test_adsr.bps			= 0; // switch off adsr
		spectrum.frqidx[0]		= frq;
		spectrum.volidx			= { 100,50,0,50,0 };
		osc->Set_spectrum		( spectrum, FIXED );
		sds_p->spectrum_arr[OSCID]= spectrum; // configure wd data
		osc->Set_feature		( default_feature );
		osc->Reset_connections	();
		osc->Set_adsr			( test_adsr );
		osc->Set_beatcursor		( 0 );
		sds_p->adsr_arr[OSCID]	= osc->Get_adsr();
		osc->OSC				( 0 );

		vco->Set_spectrum		( spectrum, FIXED );
		sds_p->spectrum_arr[VCOID]= spectrum; // configure wd data
		vco->Set_feature		( default_feature );
		vco->Reset_connections	();
		vco->Set_adsr			( test_adsr );
		vco->Set_beatcursor		( 0 );
		sds_p->adsr_arr[VCOID]	= vco->Get_adsr();
		vco->OSC				( 0 );

		Assert_equal( vco->vp.Mem->Data[100], osc->vp.Mem->Data[100] );
		Assert_equal( vco->Mem.Data[100], osc->Mem.Data[100] );

		WD_data_t	Wd			= WD_data_struct();
		Wd.spectrum_view		= true;


		sds_p->WD_state			= Wd;
		sds_p->fourier			= fourier_ns::test_data;
		sds_p->fourier.residx		= 0;	// resolution 8
		wd_p->Set_WdData		();
		wd_p->WriteData			();
		Gen_fftAmplitude		();
		Assert_equal( osc->fourier.coeff_vec[0].idx, 0 );
//		Assert_equal( osc->fourier.coeff_vec[1].idx, 2 );
//		Assert_equal( osc->fourier.coeff_vec[2].idx, 8 );
	//	Assert_equal( osc->fourier.coeff_vec[3].idx, 6 );
	}

	sds_p->WD_state.oscId	= VCOID;
	sds_p->fourier.trigger	= true;//			= fourier_ns::test_data;
	wd_p->Set_WdData		();
	wd_p->WriteData			();
	Gen_fftAmplitude		();

	Oscgroup.Save_fft_coeff	( fftBinFile );

	spectrum_t spectrum		= osc->TestSpectrum;
	spectrum.wfid[0]		= WFID_e::FOURIER;
	spectrum.osc			= OSCID;
	osc->Set_spectrum		( spectrum, FIXED );
	spectrum.osc			= VCOID;
	vco->Set_spectrum		( spectrum, FIXED );
	vco->OSC				( 0 );
	osc->OSC				( 0 );

	TEST_END( className );

}
