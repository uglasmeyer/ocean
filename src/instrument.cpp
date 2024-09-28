/*
 * Instrument.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */

#include <Instrument.h>
#include <Config.h>
#include <System.h>


Instrument_class::Instrument_class(ifd_t* ifd )
: Logfacility_class("Instrument")
{
	this->ifd 		= ifd;
	Default_instrument_file = file_structure().Dir.instrumentdir + "default.kbd";
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
	// be the MODE DEFAULT.

//	ifd_t* ifd = GUI->addr;
	if ( ifd->MODE == DEFAULT )
	{
		Comment(INFO, "default Shared Data is not reused");
		return;
	}

	Comment(INFO, "using Shared Data");

	main.adsr				= ifd->Main_adsr;
	main.wp.glide_effect	= ifd->Soft_freq 	;
	main.wp.PMW_dial		= 0;// ifd->PMW_dial; only for vco

	main.wp.frequency 		= ifd->Main_Freq  ;
	main.wp.msec	 		= max_milli_sec;// ifd->Main_Duration*1000 ; // unused
	main.spectrum			= ifd->MAIN_spectrum;
	main.wp.touched			= true;

	vco.wp.frequency		= ifd->VCO_Freq 	;
	vco.wp.volume			= ifd->VCO_Amp 		;
	vco.wp.msec				= max_milli_sec;// unused max_sec*1000	; // unused
	vco.spectrum			= ifd->VCO_spectrum;
	vco.wp.PMW_dial			= ifd->PMW_dial;
	vco.wp.touched			= true;

	fmo.wp.frequency		= ifd->FMO_Freq 		;
	fmo.wp.volume			= ifd->FMO_Amp 		;
	fmo.wp.msec				= max_milli_sec; // max_sec * 1000; // unused
	fmo.spectrum			= ifd->FMO_spectrum;
	fmo.wp.PMW_dial			= 0;// ifd->PMW_dial only for vco
	fmo.wp.touched			= true;
}

void Instrument_class::setup_GUI_Data()
{
	// use this function to update the ifd with the current set of shared information.
	// that are generated during the Set procedure, with the purpose to
	// update the SndlabGUI and to
	// notify comstack about the new data.


	Comment(INFO, "setup GUI data");


	ifd->Main_adsr 			= main.adsr;
	ifd->PMW_dial 			= vco.wp.PMW_dial;
	ifd->Soft_freq			= main.wp.glide_effect;

	ifd->MAIN_spectrum		= main.spectrum;
	ifd->Main_Freq  		= main.wp.frequency;

	ifd->VCO_Freq 			= vco.wp.frequency;
	ifd->VCO_Amp 			= vco.wp.volume;
	ifd->VCO_spectrum		= vco.spectrum;

	ifd->FMO_Freq 			= fmo.wp.frequency;
	ifd->FMO_Amp 			= fmo.wp.volume;
	ifd->FMO_spectrum		= fmo.spectrum;

	ifd->UserInterface		= UPDATEGUI; // update Instrument reset flag on GUI side


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
	if ( osc->osc_id == MAINID )
		assign_adsr( arr );
	osc->Get_comment( false );
	osc->Set_csv_comment();
	osc->Show_csv_comment( TEST );

}
void Instrument_class::set_new_name( string name )
{
	Name = name;
	Instrument_file 		= file_structure().Dir.instrumentdir + Name + ".kbd";
}

void Instrument_class::set_name( string name )
{
	Instrument_file 		= file_structure().Dir.instrumentdir + name + ".kbd";
	if ( filesystem::exists( Instrument_file ) )
		Name = name;
	else
		Name = "default";
	Instrument_file 		= file_structure().Dir.instrumentdir + Name + ".kbd";

	Comment( INFO, "Instrument Name: " + Name);
}

bool Instrument_class::assign_adsr 	( vector_str_t arr )
{
	String 				Str{""};

	main.adsr.decay 	= Str.secure_stoi( arr[9 ]);
	main.adsr.bps_id	= Str.secure_stoi( arr[10] );
	main.adsr.attack	= Str.secure_stoi( arr[11] );
	main.adsr.hall		= Str.secure_stoi( arr[12] );
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

	FILE 	<< setfill(' ') << right << "#OSC,"
			<< setw(10) << "Name"
			<< setw(10) << "Waveform"
			<< setw(8)  << "[Hz]"
			<< setw(8) 	<< "unused"
			<< setw(8)  << "Amp.[%]"
			<< setw(7)  << "unused "
			<< setw(7)  << "unused"
			<< setw(7)  << "unused"
			<< setw(18) << "delay  bps atck hall glide"
			<< setw(10)	<< "PMWs"
			<< endl;

	main.wp.volume = (int)ifd->Master_Amp;
	for ( Oscillator* osc : osc_vector )
	{
		FILE 	<< setfill(' ') << right << " OSC,"
				<< setw(10) <<		 osc->osc_type 		+ ","
				<< setw(10) <<		 osc->Get_waveform_str(osc->spectrum.id) 	+ ","
				<< setw(7 ) <<dec << osc->wp.frequency 		<< ","
				<< setw(7 ) <<dec << osc->wp.msec 			<< ","
				<< setw(7)  <<dec << osc->wp.volume			<< ","
				<< setw(7 ) <<		 "free,"
				<< setw(7)  <<		 "free,"
				<< setw(7)  <<		 "free,"
				<< setw(4) 	<< (int) osc->adsr.decay 	<< ","
				<< setw(4) 	<< (int) osc->adsr.bps_id 	<< ","
				<< setw(4) 	<< (int) osc->adsr.attack 		<< ","
				<< setw(4)  << (int) osc->adsr.hall 			<< ","
				<< setw(4) 	<< (int) osc->wp.glide_effect 		<< ","
				<< setw(7)  << (int) osc->wp.PMW_dial 		<< ","
				<< endl;

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
		FILE	<< osc->Show_this_spectrum( osc->spectrum )
				<< endl;
	}
	FILE.close();

	return ;
}

void Instrument_class::Run_osc_group()
{
	bool touched = false;
	for ( Oscillator* osc : osc_vector	)
	{
		if ( osc->wp.touched ) touched = true;
	}
	if ( touched )
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
	TEST_START();

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
	cout << "spec id" << dec << (int)vco.spectrum.id << endl;
	assert( vco.waveform_str_vec[ SGNSIN ].compare( vco.Get_waveform_str( vco.spectrum.id )) == 0 );

	assert( main.fp.data == fmo.Mem.Data );
	fmo.wp.frequency 	= 0.75;
	fmo.wp.volume		= 0;//31;
	assert( fmo.wp.frequency - 0.75 < 1E-5 );
	assert( fmo.wp.volume == 0);//31 );
	assert( ( sin(1.0) - sin(1.0-2*pi) ) < 1E-6);
	assert( main.adsr.hall == 0 );

	Data_t datan = 0;
	Data_t data0 = 0;
	fmo.OSC(0);
	main.OSC(0);
	for ( int n = 0; n <10; n++ )
	{
		datan = main.Mem.Data[max_frames-1];
		fmo.OSC(0);
		main.OSC(0);
		data0 = main.Mem.Data[0];
		cout << "> "  << setw(15) << datan << setw(15) << data0 << setw(15) << abs( abs( datan )- abs(data0 ) )  << endl;
		assert( abs( abs( datan )- abs(data0 ) )   < 400 );
	}

	TEST_END();
}



