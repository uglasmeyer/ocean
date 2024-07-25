/*
 * Instrument.cpp
 *
 *  Created on: Feb 11, 2024
 *      Author: sirius
 */

#include <Instrument.h>


Instrument_class::Instrument_class(ifd_t* ifd )
: Logfacility_class("Instrument")
{
	this->ifd 		= ifd;
	Default_instrument_file = dir_struct().instrumentdir + "default.kbd";
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

	main.adsr.bps_id		= ifd->Main_adsr_bps_id;
	main.adsr.decay			= ifd->Main_adsr_decay	;
	main.adsr.attack		= ifd->Main_adsr_attack 	;
	main.adsr.hall			= ifd->Main_adsr_hall 	;
	main.wp.glide_effect	= ifd->Soft_freq 	;
	main.wp.PMW_dial		= 0;// ifd->PMW_dial; only for vco

	main.wp.frequency 		= ifd->Main_Freq  ;
//	main.wp.volume			= ifd->Main_Amp ; mixer.mastermain_amp
	main.wp.msec	 		= max_milli_sec;// ifd->Main_Duration*1000 ; // unused
	main.wp.spectrum		= main.Get_spectrum( ifd->Main_waveform_id );
	main.ID					= ifd->Main_ID;

	vco.wp.frequency		= ifd->VCO_Freq 	;
	vco.wp.volume			= ifd->VCO_Amp 		;
	vco.wp.msec				= max_milli_sec;// unused max_sec*1000	; // unused
	vco.wp.spectrum			= vco.Get_spectrum( ifd->VCO_waveform_id );
	vco.wp.PMW_dial			= ifd->PMW_dial;
	vco.ID					= ifd->VCO_ID;

	fmo.wp.frequency		= ifd->FMO_Freq 		;
	fmo.wp.volume			= ifd->FMO_Amp 		;
	fmo.wp.msec				= max_milli_sec; // max_sec * 1000; // unused
	fmo.wp.spectrum			= fmo.Get_spectrum( ifd->FMO_waveform_id );
	fmo.wp.PMW_dial			= 0;// ifd->PMW_dial only for vco
	fmo.ID					= ifd->FMO_ID;

}

void Instrument_class::setup_GUI_Data()
{
	// use this function to update the ifd with the current set of shared information.
	// that are generated during the Set procedure, with the purpose to
	// update the SndlabGUI and to
	// notify comstack about the new data.


	Comment(INFO, "setup GUI data");


	ifd->Main_adsr_bps_id 	= main.adsr.bps_id;
	ifd->Main_adsr_decay	= main.adsr.decay;
	ifd->Main_adsr_attack 	= main.adsr.attack;
	ifd->Main_adsr_hall 	= main.adsr.hall;
	ifd->PMW_dial 			= (char) vco.wp.PMW_dial;
	ifd->Soft_freq			= main.wp.glide_effect;

	ifd->Main_Freq  		= main.wp.frequency;
//	ifd->Main_Amp 			= //0; //main.wp.volume; // see mixer init_data
//	ifd->Main_Duration 		= main.wp.msec/1000; // unused
	ifd->Main_waveform_id 	= main.wp.spectrum.id;
	ifd->Main_ID			= main.ID;

	ifd->VCO_Freq 			= vco.wp.frequency;
	ifd->VCO_Amp 			= vco.wp.volume;
	ifd->VCO_waveform_id 	= vco.wp.spectrum.id;
	ifd->VCO_ID				= vco.ID;

	ifd->FMO_Freq 			= fmo.wp.frequency;
	ifd->FMO_Amp 			= fmo.wp.volume;
	ifd->FMO_waveform_id 	= fmo.wp.spectrum.id;
	ifd->FMO_ID 			= fmo.ID;

	ifd->MODE				= FREERUN;
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
	for ( auto osc_ptr : osc_vector )
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
				<< osc_ptr->wp.waveform_str +"\t"
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
/*
	if ( Mixer.status.play  )
		Play = "active";
	else
		Play = "locked / unlock with >p<";
	Log.Comment(INFO, "Keyboard is " + Play );
	return;
	*/
}

void Instrument_class::Set_waveform( Oscillator* osc, uint8_t id )
{
	osc->wp.spectrum = osc->Get_spectrum( id );
	osc->wp.waveform_str = osc->wp.spectrum.osc ;
	osc->set_csv_comment();
}

void Instrument_class::init_data_structure( Oscillator* osc, vector_str_t arr  )
{

	osc->line_interpreter( arr );
	cout << osc->Show_spectrum()<<endl;
	int spec_id = osc->Get_waveform_id( osc->wp.waveform_str ); // =arr[2]
	if ( spec_id < 0 )
	{
		Comment( ERROR, "unable to obtain waveform id for waveform: " + osc->wp.waveform_str );
		Comment( INFO, "resulting in undefined behaviour");
		Comment( INFO, "check the instrument file accordingly");
		exit( 1 );

	}
	osc->wp.spectrum = osc->Get_spectrum( spec_id );
	osc->get_comment( false );
	osc->set_csv_comment();
	osc->show_csv_comment( TEST );

}

void Instrument_class::Set_Name( string name )
{
	Instrument_file 		= dir_struct().instrumentdir + name + ".kbd";
	if ( filesystem::exists( Instrument_file ) )
		Name = name;
	else
		Name = "default";
	Instrument_file 		= dir_struct().instrumentdir + Name + ".kbd";

	Comment( INFO, "Instrument Name: " + Name);
	for ( auto osc : osc_vector )
		osc->Set_Spec_Name( Name );
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
			if ( arr[1].compare("MAIN") == 0 )
			{
				init_data_structure( &main, arr );
				assign_adsr( arr );
			}
			if ( arr[1].compare("VCO") == 0 )
				init_data_structure( &vco, arr );
			if ( arr[1].compare("FMO") == 0 )
				init_data_structure( &fmo, arr );
		}

	} while( getline( File, Str.Str));

	File.close();
	return true;

}
Oscillator* Instrument_class::get_osc_by_name( string name )
{
	for ( auto osc : osc_vector )
	{
		if ( osc->osc_type.compare( name ) == 0 )
			return osc;
	}

	Comment(0, "incomplete instrument definition for " + name);
	exit(1);
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
		OSC->connect_fmo_data( SEC );
	}
	if ( mode[0] == 'V' )
	{
		OSC->connect_vco_data( SEC );
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
	Set_Name( str );
	fstream FILE;
	Comment(2,  "saving sound to: " + Instrument_file);

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
	for ( auto osc : osc_vector )
	{
		FILE 	<< setfill(' ') << right << " OSC,"
				<< setw(10) <<		 osc->osc_type 		+ ","
				<< setw(10) <<		 osc->wp.spectrum.osc 	+ ","
				<< setw(7 ) <<dec << osc->wp.frequency 		<< ","
				<< setw(7 ) <<dec << osc->wp.msec 			<< ","
				<< setw(7)  <<dec << osc->wp.volume 			<< ","
				<< setw(7 ) <<		 osc->wp.ops_str_arr[0] 	+ ","
				<< setw(7)  <<		 osc->wp.ops_str_arr[1] 	+ ","
				<< setw(7)  <<		 osc->wp.ops_str_arr[2] 	+ ","
				<< setw(4) 	<<dec << osc->adsr.decay 	<< ","
				<< setw(4) 	<<dec << osc->adsr.bps_id 	<< ","
				<< setw(4) 	<<dec << osc->adsr.attack 		<< ","
				<< setw(4)  <<dec << osc->adsr.hall 			<< ","
				<< setw(4) 	<<dec << osc->wp.glide_effect 		<< ","
				<< setw(7)  <<dec << osc->wp.PMW_dial 		<< ","
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
		FILE	<< osc->Show_spectrum(  );
	}
	FILE.close();

	return ;
}

void Instrument_class::run_oscs()
{
	fmo.OSC ( 0 );
	vco.OSC ( 0 );
	main.OSC( 0 );

}

bool Instrument_class::Set( string name )
{
	Set_Name( name );
	for ( auto osc : osc_vector	)
		if ( not osc->Read( osc->osc_type ) )	return false;
	if ( not read_instrument( ) ) 	return false;
	if ( not init_connections() ) 	return false;
	setup_GUI_Data();
	reuse_GUI_Data();
	run_oscs();
	show_sound_stack();
	return true;

}

void Instrument_class::Test_Instrument()
{
	Set_Loglevel(TEST, true);
	for ( auto osc : osc_vector	)
		osc->Set_Loglevel( TEST, true);

	Comment( TEST, "Instrument testing" );
	assert( Set( ".test" ) );
	vco.wp.PMW_dial = 98;
	main.wp.spectrum = main.Get_spectrum( 2 );
	assert( main.wp.spectrum.osc.compare( main.Get_waveform_str(2)) == 0 );
	Save_Instrument( ".test" );
	vco.wp.PMW_dial = 0;
	main.wp.spectrum = main.Get_spectrum( 3 );
	assert( main.wp.spectrum.osc.compare( main.Get_waveform_str(3)) == 0 );

	assert( Set( ".test" ) );
	assert( vco.wp.PMW_dial == 98 );
	cout << main.wp.spectrum.osc << "=" << main.Get_waveform_str(2) << endl;
	assert( main.wp.spectrum.osc.compare( main.Get_waveform_str(2)) == 0 );

	Comment( TEST, "Instrument test done" );
}



