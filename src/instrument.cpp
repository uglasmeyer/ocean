/*
 * Instrument.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */

#include <Instrument.h>
#include <Config.h>
#include <System.h>


Instrument_class::Instrument_class(interface_t* ifd, Wavedisplay_class* wd )
: Logfacility_class("Instrument_class")
{
	Setup( ifd );

	assert ( Oscgroup.osc.MemData() != nullptr );
	Oscgroup.SetWd( wd );
	wd_p = wd;
}

void Instrument_class::Setup( interface_t* _sds )
{
	this->sds 				= _sds;
	ifd_spectrum_vec 		= { &sds->VCO_spectrum,
								&sds->FMO_spectrum,
								&sds->OSC_spectrum};

	Default_instrument_file = file_structure().Dir.instrumentdir + "default" + instr_ext;
}

Instrument_class::~Instrument_class() = default;

void Instrument_class::reuse_GUI_Data()
{
	// use this function to update the Instrument with data from the ifd.
	// the update shall take place if the ifd data shall be reused to reach the
	// commit point of last GUI action.
	// The data shall be ignored if the ifd is initially created. This is indicated
	// by the MODE DEFAULT.

	if ( sds->MODE == DEFAULT )
	{
		Comment(INFO, "default Shared Data is not reused");
		return;
	}

	Comment(INFO, "using Shared Data");

	Oscgroup.osc.adsr				= sds->OSC_adsr;
	Oscgroup.osc.wp					= sds->OSC_wp;
	Oscgroup.osc.spectrum			= sds->OSC_spectrum;

	Oscgroup.vco.wp					= sds->VCO_wp;
	Oscgroup.vco.spectrum			= sds->VCO_spectrum;

	Oscgroup.fmo.wp					= sds->FMO_wp;
	Oscgroup.fmo.spectrum			= sds->FMO_spectrum;
}

void Instrument_class::Set_msec( buffer_t frames )
{
	uint16_t msec = frames * 1000 / frames_per_sec;
	Oscgroup.Set_Duration( msec );
}

void Instrument_class::setup_GUI_Data()
{
	// use this function to update the ifd with the current set of shared information.
	// that are generated during the Set procedure, with the purpose to
	// update the SndlabGUI and to
	// notify comstack about the new data.

	Comment(INFO, "setup GUI data");

	sds->OSC_adsr 		= Oscgroup.osc.adsr;
	sds->OSC_wp			= Oscgroup.osc.wp;
	sds->OSC_spectrum	= Oscgroup.osc.spectrum;

	sds->VCO_spectrum	= Oscgroup.vco.spectrum;
	sds->VCO_wp			= Oscgroup.vco.wp;

	sds->FMO_wp			= Oscgroup.fmo.wp;
	sds->FMO_spectrum	= Oscgroup.fmo.spectrum;

	sds->UserInterface	= UPDATEGUI; // update Instrument reset flag on GUI side
}

void Instrument_class::show_sound_stack() // show_status
{
	stringstream strs{""};
	strs 	<< setw(4)	<< "Osc."
			<< setw(10)	<< "Waveform"
			<< setw(8)	<< "Hz"
			<< setw(8)	<< "Amp"
			<< setw(8)	<< "VCO"
			<< setw(8)	<< "FMO";
	Comment( INFO, strs.str() );

	for ( Oscillator* osc : Oscgroup.member )
	{
		strs = osc->Get_sound_stack() ;
		Comment( TEST, strs.str() );
	};
}

void Instrument_class::Update_spectrum()
{
	uint 		oscid 	= sds->Spectrum_type;
	Oscillator* osc 	= Oscgroup.member[ oscid ];
	osc->Set_spectrum( *ifd_spectrum_vec[ oscid ] ) ;
}

void Instrument_class::init_data_structure( Oscillator* osc, vector_str_t arr  )
{
	osc->spectrum = Spectrum_class::spec_struct();
	osc->spectrum.osc = osc->osctype_id;

	osc->Line_interpreter( arr );

	if ( osc->is_osc_type  )
		assign_adsr( arr );

}
void Instrument_class::set_new_name( string name )
{
	Name = name;
	Instrument_file 		= file_structure().Dir.instrumentdir + Name + instr_ext;
}

void Instrument_class::set_name( string name )
{
	Instrument_file 		= file_structure().Dir.instrumentdir + name + instr_ext;
	if ( filesystem::exists( Instrument_file ) )
		Name = name;
	else
		Name = "default";
	Instrument_file 		= file_structure().Dir.instrumentdir + Name + instr_ext;

	Comment( INFO, "Instrument Name: " + Name);
}

bool Instrument_class::assign_adsr 	( vector_str_t arr )
{
	String 				Str{""};
	adsr_t adsr{};
	adsr.decay 	= Str.secure_stoi( arr[9 ]);
	adsr.bps	= Str.secure_stoi( arr[10] );
	adsr.attack	= Str.secure_stoi( arr[11] );
	adsr.hall	= Str.secure_stoi( arr[12] );
	Oscgroup.osc.Set_adsr( adsr );
	return true;
}

auto showOscfeatures = [  ]( Oscillator* osc, Oscillator* vco )
{
	Table_class Table {};
	Table.AddColumn( "Attack"	, 6 );
	Table.AddColumn( "Decay"	, 6 );
	Table.AddColumn( "Hall"		, 6 );
	Table.AddColumn( "Beats"	, 6 );
	Table.AddColumn( "SlideF"	, 6 );
	Table.AddColumn( "Pmw"		, 6 );
	Table.PrintHeader();
	Table.AddRow(	(int)osc->adsr.attack, (int)osc->adsr.decay, (int)osc->adsr.hall,
			(int)osc->adsr.bps, (int)osc->wp.glide_effect, (int)vco->wp.PMW_dial);
};
bool Instrument_class::assign_adsr2( const vector_str_t& arr )
{
	String 	Str			{""};
	adsr_t 	adsr		{};
			adsr.decay 	= Str.secure_stoi( arr[2] );
			adsr.bps	= Str.secure_stoi( arr[3] );
			adsr.attack	= Str.secure_stoi( arr[4] );
			adsr.hall	= Str.secure_stoi( arr[5] );
	Oscgroup.osc.Set_adsr( adsr );

	Oscgroup.osc.wp.glide_effect= Str.secure_stoi( arr[6] );
	Oscgroup.vco.wp.PMW_dial	= Str.secure_stoi( arr[7] );
	showOscfeatures( &Oscgroup.osc, &Oscgroup.vco );

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
	Spectrum_class	Spectr;

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
		{
			for( int num : Spectr.spectrumNum )
			{
				if ( strEqual(keyword, Spectr.spectrumTag[num] ) )
				{
					osc->spectrum = osc->Parse_data( arr, osc->osctype_id, num );
					*ifd_spectrum_vec[ osc->osctype_id ] = osc->spectrum;
				}
			}

		}


	} while( getline( *File, Str.Str));

	File->close();
	return true;
}
bool Instrument_class::read_version2( fstream* File )
{
	String 			Str		{""};
	string 			keyword	{""};
	vector_str_t 	arr 	{};
	Oscillator* 	osc 	= nullptr;

	getline( *File, Str.Str );
	do
	{
		Str.normalize();
		arr = Str.to_array( ',' );
		keyword = arr[0];

		if( strEqual( "Type", keyword ))
			{;} // ignore head line
		else
			osc 	= Oscgroup.Get_osc_by_name( arr[1] );


		if ( ( strEqual("ADSR", keyword) ) )
		{
			assign_adsr2( arr );
		}
		if ( strEqual( "SPEV", keyword ))
		{
			osc->spectrum 		= osc->Parse_data( arr, osc->osctype_id, SPEV );
			osc->Set_volume( 	osc->spectrum.volidx[0], FIXED );
		}
		if ( strEqual( "SPEF", keyword ))
		{
			osc->spectrum 		= osc->Parse_data( arr, osc->osctype_id, SPEF );
			osc->Set_frequency( osc->spectrum.frqidx[0], FIXED );
		}
		if ( strEqual( "SPEW", keyword ))
		{
			osc->spectrum 		= osc->Parse_data( arr, osc->osctype_id, SPEW );
			osc->Set_waveform( 	osc->spectrum.wfid );
		}
		if ( strEqual( "CONN_defunc", keyword ))
		{
			Oscillator* sec 	= Oscgroup.Get_osc_by_name( arr[2]);
			char 		mode 	= arr[3][0];
			switch ( mode )
			{
				case 'F' : osc->Connect_fmo_data( sec ); break;
				case 'V' : osc->Connect_vco_data( sec ); break;
				default  : Comment( ERROR, "unknown connection mode ", (char) mode ); break;
			}
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
			code = read_version2( &File );
			break;
		default :
			code = false;
			break;
	}

	if ( code )
		show_sound_stack();

	return code;
}

bool Instrument_class::connect( string osc, string sec, string mode )
{
	if ( sec.compare(osc) == 0)
		return true; // handled by initOSCs
	Oscillator* OSC;
	Oscillator* SEC;
	OSC = Oscgroup.Get_osc_by_name( osc );
	SEC = Oscgroup.Get_osc_by_name( sec );
	if ( mode[0] == 'F' )
	{
		OSC->Connect_fmo_data( SEC );
	}
	if ( mode[0] == 'V' )
	{
		OSC->Connect_vco_data( SEC );
	}

	return true;
}

bool Instrument_class::init_connections( )
{
	String 			Str{""};
	vector_str_t 	arr;

	fstream File;
	File.open( Instrument_file, fstream::in );
	Comment( INFO, "Reading oscillator connections");
	getline( File, Str.Str );
	do
	{
		Str.normalize();
		arr 	= Str.to_array( ',');
		string
		keyword = arr[0];
		if ( strEqual( "CONN", keyword ) )
			if ( ! connect(arr[1], arr[2], arr[3]) )
				return false;

	} while( getline( File, Str.Str));
	return true;
}

void Instrument_class::Save_Instrument( string str )
{
	set_new_name( str );
	Comment( INFO,  "saving sound to: " + Instrument_file);

	fstream FILE;
	FILE.open(Instrument_file, fstream::out ); // overwrite the file content
	Table_class Table{ &FILE, ',' };

	// Instrument file version
	FILE 	<< "VERSION=2" << endl;

	// Instrument file header
	Table.AddColumn("Type",	6 );
	Table.AddColumn("Name",	6 );
	Table.AddColumn("decay",6 );
	Table.AddColumn("bps",	6 );
	Table.AddColumn("atack",6 );
	Table.AddColumn("hall",	6 );
	Table.AddColumn("slide",6 );
	Table.AddColumn("pmw",	6 );
	Table.PrintHeader();

	Table.AddRow( "ADSR", "OSC",
			(int) osc->adsr.decay,
			(int) osc->adsr.bps,
			(int) osc->adsr.attack,
			(int) osc->adsr.hall,
			(int) osc->wp.glide_effect,
			(int) vco->wp.PMW_dial
			);

	for ( Oscillator* osc : Oscgroup.member )
	{
		// Type CONN
		Table.AddRow("CONN",
				osc->osc_type,
				osc->fp.name,
				"F");
		Table.AddRow("CONN",
				osc->osc_type,
				osc->vp.name,
				"V");

		// Type SPEC
		osc->Save_spectrum_table( &FILE, osc->spectrum );
	}

	FILE.close();

	return ;
}

bool Instrument_class::Set( string name )
{
	set_name( name );

	if ( not read_instrument( ) ) 	return false;
	Oscgroup.Data_Reset();
	Oscgroup.Connection_Reset();
	if ( not init_connections() ) 	return false;
	setup_GUI_Data();
//	reuse_GUI_Data();
//	Oscgroup.Run_Oscgroup( 0 );
	show_sound_stack();
	return true;
}


void Instrument_class::Test_Instrument()
{

	TEST_START( className );
	assert( Set( ".test2" ) );

	showOscfeatures( &Oscgroup.osc, &Oscgroup.vco);

	Oscgroup.osc.Set_duration( min_milli_sec );
	float f = Oscgroup.osc.GetFrq( Oscgroup.osc.wp.frqidx );
	ASSERTION( f == (float)220, "frequency", f, 220.000000 );
	ASSERTION( fcomp( 	sds->OSC_spectrum.frqidx[0], 71), "frequency",
						sds->OSC_spectrum.frqidx[0], 71 );

	Oscgroup.osc.Phase_reset();
	Oscgroup.osc.Set_frequency( 71, FIXED ); // 220 Hz
	Comment( TEST, Oscgroup.Show_Spectrum() );

	Oscgroup.Data_Reset();
	Oscgroup.Run_OSCs(0);
	assert( Oscgroup.osc.DynFrequency.current.past == Oscgroup.osc.spectrum.frqidx[0]);
	float amp0 = Oscgroup.osc.MemData(0) ;
	for ( int n = 0; n < 9; n++ )
	{
//		Oscgroup.Data_Reset();
		Oscgroup.Run_OSCs( 0 );
		float amp1 = Oscgroup.osc.MemData(0) ;
		assert( Oscgroup.osc.wp.frqidx == Oscgroup.osc.spectrum.frqidx[0] );
//		cout << amp0 << " " << amp1 << " " << Oscgroup.osc.phase[0] << " " << (int)Oscgroup.osc.wp.frqidx << endl;
		string com = "member" + to_string(n);
		ASSERTION( fcomp( amp0, amp1), com.data(),
					abs ( amp1- amp0 ), "~0" );
	}
	TEST_START( className );
	vco->Test();
	assert( Set( ".test2" ) );
	ASSERTION( file_version == 2, "version", file_version , 2);
	for ( Oscillator* osc : Oscgroup.member	)
		osc->Set_Loglevel( TEST, true);
	sds->MODE = FREERUN;

	Oscgroup.vco.wp.PMW_dial = 98;
	Oscgroup.vco.spectrum.wfid[0] = Oscwaveform_class::SGNSIN;
	Oscgroup.vco.Set_frequency( "A1", FIXED);
	assert( strEqual( 	waveform_str_vec[ Oscwaveform_class::SGNSIN ],
						Oscgroup.vco.Get_waveform_str( Oscgroup.vco.spectrum.wfid[0] )));

	Save_Instrument( ".test2" );
	Oscgroup.vco.wp.PMW_dial = 0;
	Oscgroup.vco.spectrum.wfid[0] = Oscwaveform_class::RECTANGLE;
	assert( strEqual( 	waveform_str_vec[ Oscwaveform_class::RECTANGLE ],
						Oscgroup.vco.Get_waveform_str( Oscgroup.vco.spectrum.wfid[0] )));


	assert( Set( ".test2" ) );
	ASSERTION( sds->VCO_wp.PMW_dial == 98,"Set PMW_dial", (int)sds->VCO_wp.PMW_dial , 98);
	assert( Oscgroup.vco.wp.PMW_dial == 98 );
	string a = waveform_str_vec[ Oscwaveform_class::SGNSIN ];
	string b = Oscgroup.vco.Get_waveform_str( Oscgroup.vco.spectrum.wfid[0] );
	ASSERTION( strEqual( a,b), "SGNSIN", a, b);

	assert( Oscgroup.osc.fp.Mem->Data == Oscgroup.fmo.MemData() );
//	assert( Oscgroup.osc.fp.data == Oscgroup.fmo.Mem.Data );
	Oscgroup.fmo.Set_frequency( C0, FIXED );
	f = Oscgroup.fmo.GetFrq( Oscgroup.fmo.wp.frqidx);
	ASSERTION( fcomp( f, 16.3516 ), "" ,f, 16.3516 );


	assert( ( sin(1.0) - sin(1.0-2*pi) ) < 1E-6);
	assert( Oscgroup.osc.adsr.hall == 0 );
	assert( Oscgroup.osc.adsr.bps == 0 );

	TEST_END( className );
	return;

	Oscgroup.fmo.Set_volume( 0, FIXED );
	Oscgroup.fmo.Set_duration( max_milli_sec );
	Oscgroup.vco.Set_duration( max_milli_sec );
	Oscgroup.vco.Set_volume( 0, FIXED );
	Oscgroup.osc.Set_duration( max_milli_sec );
	Oscgroup.osc.Set_frequency( "A3", FIXED );
	Oscgroup.osc.Set_waveform( {0,0,0,0,0} );
	Oscgroup.osc.Set_volume( 100, FIXED );




}



