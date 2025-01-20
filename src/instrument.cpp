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
: Logfacility_class("Instrument")
{
	Setup( ifd );
	wd_p = wd;
	wd->Add_data_ptr( vco.osc_type, oscgrouo_name, vco.Mem.Data);
	wd->Add_data_ptr( fmo.osc_type, oscgrouo_name, fmo.Mem.Data);
	wd->Add_data_ptr( osc.osc_type, oscgrouo_name, osc.Mem.Data);
}

void Instrument_class::Setup( interface_t* ifd )
{
	this->ifd 		= ifd;
	Default_instrument_file = file_structure().Dir.instrumentdir + "default" + instr_ext;
	ifd_spectrum_vec = { &ifd->VCO_spectrum, &ifd->FMO_spectrum, &ifd->MAIN_spectrum};
}

Instrument_class::~Instrument_class()
{
}

void Instrument_class::reuse_GUI_Data()
{
	// use this function to update the Instrument with data from the ifd.
	// the update shall take place if the ifd data shall be reused to reach the
	// commit point of last GUI action.
	// The data shall be ignored if the ifd is initially created. This is indicated
	// by the MODE DEFAULT.

	if ( ifd->MODE == DEFAULT )
	{
		Comment(INFO, "default Shared Data is not reused");
		return;
	}

	Comment(INFO, "using Shared Data");

	osc.adsr				= ifd->Main_adsr;
	osc.wp					= ifd->OSC_wp;
	osc.spectrum			= ifd->MAIN_spectrum;

	vco.wp					= ifd->VCO_wp;
	vco.spectrum			= ifd->VCO_spectrum;

	fmo.wp					= ifd->FMO_wp;
	fmo.spectrum			= ifd->FMO_spectrum;

}

buffer_t Instrument_class::Set_msec( uint16_t msec )
{
	for( Oscillator* osc : osc_vector )
	{
		osc->Set_duration( msec );
	}
	buffer_t frames = msec * frames_per_sec / 1000;
	return frames;
}

void Instrument_class::setup_GUI_Data()
{
	// use this function to update the ifd with the current set of shared information.
	// that are generated during the Set procedure, with the purpose to
	// update the SndlabGUI and to
	// notify comstack about the new data.


	Comment(INFO, "setup GUI data");


	ifd->Main_adsr 		= osc.adsr;
	ifd->OSC_wp			= osc.wp;
	ifd->MAIN_spectrum	= osc.spectrum;

	ifd->VCO_spectrum	= vco.spectrum;
	ifd->VCO_wp			= vco.wp;

	ifd->FMO_wp			= fmo.wp;
	ifd->FMO_spectrum	= fmo.spectrum;

	ifd->UserInterface	= UPDATEGUI; // update Instrument reset flag on GUI side


}

void Instrument_class::show_sound_stack() // show_status
{
	string star 	= "*";
	string nostar 	= ".";
	string active 	;
	string fp_flag  ;
	string vp_flag  ;
	string fp_gen  ;
	string vp_gen  ;
	string Play 	;

	stringstream strs{""};
	strs << "\n Name \tWaveform \tHz \tAmp \tmsec \t  VCO \t  FMO " << endl;
	for ( Oscillator* osc_ptr : osc_vector )
	{
		active  	= nostar;
		fp_flag     = nostar;
		vp_flag     = nostar;
		fp_gen		= nostar;
		vp_gen		= nostar;
		if ( osc_ptr->vp.stored ) 		vp_flag = star;
		if ( osc_ptr->fp.stored ) 		fp_flag = star;
		if ( osc_ptr->vp.generated ) 	vp_gen = star;
		if ( osc_ptr->fp.generating ) 	fp_gen = star;
		strs 	<< active
				<< osc_ptr->osc_type +"\t"
				<< osc_ptr->Get_waveform_str( osc_ptr->spectrum.id ) +"\t"
				<< to_string( osc_ptr->wp.frequency )+"\t"
				<< to_string( osc_ptr->wp.volume )+"\t"
				<< to_string( osc_ptr->wp.msec ) +"\t"

				<< vp_flag
				<< vp_gen
				<< osc_ptr->vp.name +"\t"

				<< fp_flag
				<< fp_gen
				<< osc_ptr->fp.name
				<< endl;
	};
	Comment( INFO, strs.str() );

}

void Instrument_class::Update_spectrum()
{
	Comment(INFO, "receive command <update Spectrum>");
	Oscillator* osc = osc_vector[ ifd->Spectrum_type ];
	osc->Set_spectrum( *ifd_spectrum_vec[ ifd->Spectrum_type ] ) ;
}

void Instrument_class::init_data_structure( Oscillator* osc, vector_str_t arr  )
{

	osc->Line_interpreter( arr );
	if ( osc->osc_id == INSTRID )
		assign_adsr( arr );
	osc->Get_comment( false );
	osc->Set_csv_comment();
	osc->Show_csv_comment( TEST );

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

	osc.adsr.decay 	= Str.secure_stoi( arr[9 ]);
	osc.adsr.bps	= Str.secure_stoi( arr[10] );
	osc.adsr.attack	= Str.secure_stoi( arr[11] );
	osc.adsr.hall	= Str.secure_stoi( arr[12] );
	return true;
}
;

bool Instrument_class::read_instrument( )
{
	String 		Str{""};
	string 		keyword;
	fstream 	File;
	vector_str_t arr 		={};

	Comment( INFO, "using instrument file: " + Instrument_file);
	File.open(Instrument_file, fstream::in );
	if ( not File.is_open())
	{
		Comment(ERROR, "Input file: " + Instrument_file + " does not exist" );
		Comment(INFO, "using default instrument file: " + Default_instrument_file);
		Instrument_file = Default_instrument_file;
		File.open( Instrument_file, fstream::in );
		if ( not File.is_open())
		{
			Comment(0, "Input file: " + Default_instrument_file + " does not exist" );
			Comment(0, "Giving up");
			return false;
		}

	}
	getline( File, Str.Str );
	do
	{
		Str.normalize();

		arr = Str.to_array( ',' );

		keyword = arr[0];
		if ( keyword.compare("OSC") == 0)
		{
			for ( Oscillator* osc : osc_vector )
			{
				if ( osc->osc_type.compare( arr[1] ) == 0 )
					init_data_structure( osc, arr );
			}
		}
		if ( keyword.compare("SPEC") == 0 )
		{
			for ( Oscillator* osc : osc_vector )
			{
				if ( osc->osc_type.compare( arr[1] ) == 0 )
					osc->spectrum =  osc->Parse_data( arr, osc->osc_id );
			}
		}

	} while( getline( File, Str.Str));

	File.close();

	return true;

}
Oscillator* Instrument_class::get_osc_by_name( string name )
{
	for ( Oscillator* osc : osc_vector )
	{
		if ( osc->osc_type.compare( name ) == 0 )
			return osc;
	}

	Exception( "incomplete instrument definition for " + name );
	return nullptr;
}

bool Instrument_class::connect( string osc, string sec, string mode )
{
	if ( sec.compare(osc) == 0)
		return true; // handled by initOSCs
	Oscillator* OSC;
	Oscillator* SEC;
	OSC = get_osc_by_name( osc );
	SEC = get_osc_by_name( sec );
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
	string keyword;
	String 		Str{""};
	vector_str_t arr;
	fstream File;

	Comment( INFO, "Reading oscillator connections");
	File.open( Instrument_file, fstream::in );
	getline( File, Str.Str );
	do
	{
		Str.normalize();
		arr = Str.to_array( ',');
		keyword = arr[0];
		if ( keyword.compare("CONN") == 0 )
			if ( ! connect(arr[1],arr[2], arr[3]) )
				return false;

	} while( getline( File, Str.Str));
	File.close();
	return true;
}

void Instrument_class::Save_Instrument( string str )
{
	set_new_name( str );
	Comment( INFO,  "saving sound to: " + Instrument_file);

	fstream FILE;
	FILE.open(Instrument_file, fstream::out ); // overwrite the file content

	// Instrument file header
	FILE 	<< setfill(' ') << right << "Type,"
			<< setw(10) << "Name"
			<< setw(10) << "Waveform"
			<< setw(8)  << "[Hz]"
			<< setw(8) 	<< "unused"
			<< setw(8)  << "Amp.[%]"
			<< setw(7)  << "unused "
			<< setw(7)  << "unused"
			<< setw(7)  << "unused"
			<< setw(18) << "decay  bps atck hall glide"
			<< setw(10)	<< "PMWs"
			<< endl;

	osc.wp.volume = (int)ifd->Master_Amp;
	for ( Oscillator* osc : osc_vector )
	{

		// Type OSC
		FILE 	<< setfill(' ') << right << " OSC,"
				<< setw(10) <<		 osc->osc_type 		+ ","
				<< setw(10) <<		 osc->Get_waveform_str(osc->spectrum.id) 	+ ","
				<< setw(7 ) << (int) osc->wp.frequency 		<< ","
				<< setw(7 ) << (int) osc->wp.msec 			<< ","
				<< setw(7)  << (int) osc->wp.volume			<< ","
				<< setw(7 ) <<		 "free,"
				<< setw(7)  <<		 "free,"
				<< setw(7)  <<		 "free,"
				<< setw(4) 	<< (int) osc->adsr.decay 	<< ","
				<< setw(4) 	<< (int) osc->adsr.bps 	<< ","
				<< setw(4) 	<< (int) osc->adsr.attack 		<< ","
				<< setw(4)  << (int) osc->adsr.hall 			<< ","
				<< setw(4) 	<< (int) osc->wp.glide_effect 		<< ","
				<< setw(7)  << (int) osc->wp.PMW_dial 		<< ","
				<< endl;

		// Type CONN
		FILE 	<< "CONN,"
				<< setw(9) << osc->osc_type 	<< ","
				<< setw(9) << osc->fp.name 	<< ","
				<< setw(8) 	<< "F,"
				<< endl;
		FILE 	<< "CONN,"
				<< setw(9) << osc->osc_type 	<< ","
				<< setw(9) << osc->vp.name 	<< ","
				<< setw(8) 	<< "V,"
				<< endl;

		// Type SPEC
		FILE	<< osc->Show_this_spectrum( osc->spectrum )
				<< endl;
	}
	FILE.close();

	return ;
}

void Instrument_class::Run_osc_group()
{
	for ( Oscillator* osc : osc_vector )
	{
		osc->OSC( 0 );
	}
}

bool Instrument_class::Set( string name )
{
	set_name( name );
	if ( not read_instrument( ) ) 	return false;
	if ( not init_connections() ) 	return false;
	setup_GUI_Data();
	reuse_GUI_Data();
	Run_osc_group();
	show_sound_stack();
	return true;

}


void Instrument_class::Test_Instrument()
{
	TEST_START( "Instrument" );

	for ( Oscillator* osc : osc_vector	)
		osc->Set_Loglevel( TEST, true);
	ifd->MODE = FREERUN;
	Comment( TEST, "Instrument testing" );

	assert( Set( ".test" ) );
	vco.wp.PMW_dial = 98;
	vco.spectrum.id = SGNSIN;
	assert( vco.waveform_str_vec[ SGNSIN ].compare( vco.Get_waveform_str( vco.spectrum.id )) == 0 );

	Save_Instrument( ".test" );
	vco.wp.PMW_dial = 0;
	vco.spectrum.id = RECTANGLE;
	assert( vco.waveform_str_vec[ RECTANGLE ].compare( vco.Get_waveform_str( vco.spectrum.id )) == 0 );


	assert( Set( ".test" ) );
	assert( vco.wp.PMW_dial == 98 );
	assert( vco.waveform_str_vec[ SGNSIN ].compare( vco.Get_waveform_str( vco.spectrum.id )) == 0 );

	assert( osc.fp.data == fmo.Mem.Data );
	fmo.wp.frequency 	= 0.75;
	fmo.wp.volume		= 0;//31;
	assert( fmo.wp.frequency - 0.75 < 1E-5 );
	assert( fmo.wp.volume == 0);//31 );
	assert( ( sin(1.0) - sin(1.0-2*pi) ) < 1E-6);
	assert( osc.adsr.hall == 0 );

	Data_t datan = 0;
	Data_t data0 = 0;
	fmo.OSC(0);
	osc.OSC(0);
	for ( int n = 0; n <10; n++ )
	{
		datan = osc.Mem.Data[max_frames-1];
		fmo.OSC(0);
		osc.OSC(0);
		data0 = osc.Mem.Data[0];
		cout << "> "  << setw(15) << datan << setw(15) << data0 << setw(15) << abs( abs( datan )- abs(data0 ) )  << endl;
		assert( abs( abs( datan )- abs(data0 ) )   < 400 );
	}

	TEST_END( "Instrument" );
}



