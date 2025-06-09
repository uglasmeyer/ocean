/*
 * Instrument.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */

#include <Instrument.h>
#include <Config.h>
#include <System.h>


Instrument_class::Instrument_class(interface_t* ifd, Wavedisplay_class* _wd_p )
: Logfacility_class("Instrument_class")
{
	this->sds 				= ifd;
	ifd_spectrum_vec 		= { &sds->VCO_spectrum,
								&sds->FMO_spectrum,
								&sds->OSC_spectrum};

	Default_instrument_file = file_structure().instrumentdir + "default" + instr_ext;

	assert ( Oscgroup.osc.MemData_p() != nullptr );

	this->wd_p 				= _wd_p;
	Oscgroup.SetWd( this->wd_p, &Oscgroup.osc.wp.frames );
}


Instrument_class::~Instrument_class() = default;


void Instrument_class::Set_msec( buffer_t frames )
{
	uint16_t msec = frames / frames_per_msec;
	Oscgroup.Set_Duration( msec );
}


void Instrument_class::Update_sds()
{
	// use this function to update the ifd with the current set of shared information.
	// that are generated during the Set procedure, with the purpose to
	// update the SndlabGUI and to
	// notify comstack about the new data.

	Comment(INFO, "Update SDS data");

	sds->OSC_adsr 		= Oscgroup.osc.adsr;
	sds->OSC_wp			= Oscgroup.osc.wp;
	sds->OSC_spectrum	= Oscgroup.osc.spectrum;

	sds->VCO_spectrum	= Oscgroup.vco.spectrum;
	sds->VCO_wp			= Oscgroup.vco.wp;

	sds->FMO_wp			= Oscgroup.fmo.wp;
	sds->FMO_spectrum	= Oscgroup.fmo.spectrum;

	Update_sds_connect();
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
	osc->spectrum.osc = osc->typeId;

	osc->Line_interpreter( arr );

	if ( osc->is_osc_type  )
		assign_adsr( arr );

}
void Instrument_class::set_new_name( string name )
{
	Name = name;
	Instrument_file 		= file_structure().instrumentdir + Name + instr_ext;
}

void Instrument_class::set_name( string name )
{
	Instrument_file 		= file_structure().instrumentdir + name + instr_ext;
	if ( filesystem::exists( Instrument_file ) )
		Name = name;
	else
		Name = "default";
	Instrument_file 		= file_structure().instrumentdir + Name + instr_ext;

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
	Table_class Table { "Features" };
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
					osc->spectrum = osc->Parse_data( arr, osc->typeId, num );
					*ifd_spectrum_vec[ osc->typeId ] = osc->spectrum;
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
//	Oscillator_base::connect_t connect = Oscillator_base::connect_struct();

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
			osc->spectrum 		= osc->Parse_data( arr, osc->typeId, SPEV );
			osc->Set_volume( 	osc->spectrum.volidx[0], FIXED );
		}
		if ( strEqual( "SPEF", keyword ))
		{
			osc->spectrum 		= osc->Parse_data( arr, osc->typeId, SPEF );
			osc->Set_frequency( osc->spectrum.frqidx[0], SLIDE );
		}
		if ( strEqual( "SPEW", keyword ))
		{
			osc->spectrum 		= osc->Parse_data( arr, osc->typeId, SPEW );
			osc->Set_waveform( 	osc->spectrum.wfid );
		}
		if ( strEqual( "CONN", keyword ))
		{
			Oscillator* sec 	= Oscgroup.Get_osc_by_name( arr[2]);
			char 		mode 	= arr[3][0];
			switch ( mode )
			{
				case 'F' : osc->Connect_frq_data( sec ); break;
				case 'V' : osc->Connect_vol_data( sec ); break;
				default  : Comment( ERROR, "unknown connection mode ", (char) mode ); break;
			}
		}

	} while( getline( *File, Str.Str));


	return true;
}

void Instrument_class::Update_sds_connect( )
{
	for( Oscillator* osc : Oscgroup.member )
		sds->connect[osc->typeId] = osc->connect;
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
		Oscgroup.Show_sound_stack();

	return code;
}

void Instrument_class::Connect( Oscillator* osc, Oscillator* sec, char mode )
{
	Oscillator_base::connect_t connect = sds->connect[osc->typeId];
	switch ( mode )
	{
		case 'F' 	: { ( connect.frq ) ? osc->Connect_frq_data( sec ):osc->Reset_frq_data(); break; }
		case 'V' 	: { ( connect.vol ) ? osc->Connect_vol_data( sec ):osc->Reset_vol_data(); break; }
		default 	: { ; break; }
	}
}
bool Instrument_class::connect_by_name( string osc, string sec, char mode )
{
	if ( strEqual( sec, osc ) )
		return true; // handled by initOSCs

	bool ret = true;
	Oscillator* OSC = Oscgroup.Get_osc_by_name( osc );
	Oscillator* SEC = Oscgroup.Get_osc_by_name( sec );;

	switch ( mode )
	{
		case 'F' 	: { OSC->Connect_frq_data( SEC ); break; }
		case 'V' 	: { OSC->Connect_vol_data( SEC ); break; }
		default 	: { ret = false; break; }
	}
	if ( ret )
		Update_sds_connect();
	return ret;
}

bool Instrument_class::init_connections( )
{
	String 			Str		{""};
	vector_str_t 	arr		{};

	fstream 		File	{};
	File.open( Instrument_file, fstream::in );
	Comment( INFO, "Reading oscillator connections");

	getline( File, Str.Str );
	do
	{
		Str.normalize();
		arr 	= Str.to_array( ',');

		if ( strEqual( "CONN", arr[0] ) )
			if ( not connect_by_name(arr[1], arr[2], arr[3][0]) )
				return false;

	} while( getline( File, Str.Str));

	return true;
}

void Instrument_class::save_features( fstream& FILE )
{
	tableopt_t opt = defaultopt;
	opt.FILE = &FILE;
	opt.Separator = ',';
	Table_class Table{ opt };
	Table.AddColumn("Type",	6 );
	Table.AddColumn("Name",	6 );
	Table.AddColumn("decay",6 );
	Table.AddColumn("bps",	6 );
	Table.AddColumn("atack",6 );
	Table.AddColumn("hall",	6 );
	Table.AddColumn("slide",6 );
	Table.AddColumn("pmw",	6 );
	Table.PrintHeader();

	// Type Features
	Table.AddRow( "ADSR", "OSC",
			(int) osc->adsr.decay,
			(int) osc->adsr.bps,
			(int) osc->adsr.attack,
			(int) osc->adsr.hall,
			(int) osc->wp.glide_effect,
			(int) vco->wp.PMW_dial
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
	Comment( INFO,  "saving sound to: " + Instrument_file);

	fstream FILE;
	FILE.open(Instrument_file, fstream::out ); // overwrite the file content

	// Instrument file version
	FILE 	<< "VERSION=2" << endl;

	// Type ADSR
	save_features( FILE );

	for ( Oscillator* osc : Oscgroup.member )
	{
		// Type SPEC
		osc->Save_spectrum_table( &FILE, osc->spectrum );
		// Type CONN
		save_connections( FILE, osc );
	}

	FILE.close();

	return ;
}

bool Instrument_class::Set( string name )
{
	set_name( name );

	Oscgroup.Data_Reset();
	Oscgroup.Connection_Reset();

	if ( not read_instrument( ) ) 	return false;

//	if ( not init_connections() ) 	return false;
	Update_sds();
//	reuse_GUI_Data();
//	Oscgroup.Run_Oscgroup( 0 );
	return true;
}

#include <Osc.h>
void Instrument_class::Test_Instrument()
{

	TEST_START( className );
	assert( Set( ".test2" ) );

	showOscfeatures( &Oscgroup.osc, &Oscgroup.vco);

	Oscgroup.osc.Setwp_frames( min_msec );
	float f = Oscgroup.osc.GetFrq( Oscgroup.osc.wp.frqidx );
	ASSERTION( f == (float)220, "frequency", f, 220.000000 );
	ASSERTION( fcomp( 	sds->OSC_spectrum.frqidx[0], 71), "frequency",
						sds->OSC_spectrum.frqidx[0], 71 );

	Oscgroup.osc.Phase_reset();
	Oscgroup.osc.Set_frequency( 71, FIXED ); // 220 Hz
	Comment( TEST, Oscgroup.Show_Spectrum() );

	Oscgroup.Data_Reset();
	osc->Connection_reset();
	fmo->Set_volume(100,FIXED);
	vco->Set_volume(0,FIXED);
	Oscgroup.Show_sound_stack();
	Oscgroup.Run_OSCs(0);
	assert( Oscgroup.osc.DynFrequency.current.past == Oscgroup.osc.spectrum.frqidx[0]);
	float amp0 = Oscgroup.osc.MemData(0) ;
	for ( int n = 0; n < 9; n++ )
	{
		Oscgroup.Data_Reset();
		Oscgroup.Run_OSCs( 0 );
		float amp1 = Oscgroup.osc.MemData(0) ;
		assert( Oscgroup.osc.wp.frqidx == Oscgroup.osc.spectrum.frqidx[0] );
//		cout << amp0 << " " << amp1 << " " << Oscgroup.osc.phase[0] << " " << (int)Oscgroup.osc.wp.frqidx << endl;
		string com = "member" + to_string(n);
		ASSERTION( fcomp( amp0, amp1), com.data(),
					abs ( amp1- amp0 ), "~0" );
	}
	vco->Test();
	assert( Set( ".test2" ) );
	ASSERTION( file_version == 2, "version", file_version , 2);
	sds->MODE = sdsstate_struct::FREERUN;

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

	assert( Oscgroup.osc.fp.Mem->Data == Oscgroup.fmo.MemData_p() );
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
	Oscgroup.fmo.Setwp_frames( max_msec );
	Oscgroup.vco.Setwp_frames( max_msec );
	Oscgroup.vco.Set_volume( 0, FIXED );
	Oscgroup.osc.Setwp_frames( max_msec );
	Oscgroup.osc.Set_frequency( "A3", FIXED );
	Oscgroup.osc.Set_waveform( {0,0,0,0,0} );
	Oscgroup.osc.Set_volume( 100, FIXED );




}



