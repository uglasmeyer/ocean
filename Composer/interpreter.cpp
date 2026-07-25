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
 * Interpreter.cpp
 *
 *  Created on: Apr 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <Composer/Interpreter.h>
#include <Oscwaveform.h>

const uset_t staidstr_set = { "0", "1", "2", "3", "4", "5", "6", "7" };
const uset_t regidstr_set = { "0", "1", "2", "3", "4", "5", "6", "7" };

using namespace Interpreter_ns;

Interpreter_class::Interpreter_class( Application_class* app )
	: Logfacility_class	( "Interpreter_class" )
	, Interface_base	( app->DaTA_p )
	, Processor_class	( app )
	, Bps				()
	, Musicxml			( app->fs )
	, Variation			( app->DaTA_p, &Musicxml )
{
	this->Cfg_p			= app->DaTA_p->Cfg_p;

	main_view.name		= "Main osc";
	main_view.oscid		= OSCID;
	main_view.wfkey 	= SETWAVEFORMMAINKEY;
	main_view.ampkey 	= STA_VOLUME_KEY; //manage volume of the STA_INSTRUMENT
	main_view.freqkey 	= OSCFREQUENCYKEY;

	vco_view.name		= "VCO";
	vco_view.oscid		= VCOID;
	vco_view.wfkey 		= SETWAVEFORMVCOKEY;
	vco_view.ampkey 	= VCOAMPKEY;
	vco_view.freqkey 	= VCOFREQUENCYKEY;

	fmo_view.name		= "FMO";
	fmo_view.oscid		= FMOID;
	fmo_view.wfkey 		= SETWAVEFORMFMOKEY;
	fmo_view.ampkey 	= FMOAMPKEY;
	fmo_view.freqkey 	= FMOFREQUENCYKEY;
	set_osc_view		( sds_p );


	this->dialog_mode 	= false;
	this->CompilerExit	= false;
	this->error			= 0;
	this->duration 		= 0;

}

Interpreter_class::~Interpreter_class()
{
	DESTRUCTOR( className )
}

void Interpreter_class::set_osc_view( interface_t* sds )
{
	main_view.wf 		= &sds_p->spectrum_arr[OSCID].wfid[0];
	main_view.amp 		= &sds_master->Master_Amp;
	main_view.frqidx 	= &sds_p->spectrum_arr[OSCID].frqidx[0];

	vco_view.wf 		= &sds_p->spectrum_arr[VCOID].wfid[0];
	vco_view.amp 		= &sds_p->spectrum_arr[VCOID].volidx[0];
	vco_view.frqidx 	= &sds_p->spectrum_arr[VCOID].frqidx[0];

	fmo_view.wf 		= &sds_p->spectrum_arr[FMOID].wfid[0];
	fmo_view.amp 		= &sds_p->spectrum_arr[FMOID].volidx[0];
	fmo_view.frqidx 	= &sds_p->spectrum_arr[FMOID].frqidx[0];

}

bool Interpreter_class::Cmpkeyword ( string word )
{
	String	lower {keyword.Str};
			lower.to_lower();
	return ( lower.Str.compare( word ) == 0 );
}

void Interpreter_class::Start_bin( string_vec_t arr )
{
	string cmd{};
	string bin{};
	string opt{};

	expect = { "AudioServer", "Synthesizer", "Rtsp" };
	Intro( arr , 2 );
	SetKeyword		( 1 );

	if ( Cmpkeyword( "rtsp" ) )
	{
		bin = fs->rtsp_bin;
		opt = "-C";
	}
	if ( Cmpkeyword( "synthesizer") )
	{
		bin = fs->synth_bin;
	}
	if ( Cmpkeyword( "audioserver") )
	{
		bin = fs->audio_bin;
	}
	if( bin.length() > 0 )
	{
		if ( opt.length() == 0 )
		{
			expect = { "shared memory key" };
			option_default = "";
			string arg = pop_stack( 0 );
			opt = "-k " + arg;
			if ( arg.length() == 0 )
				opt = "";
		}
		Comment( INFO, "start", bin );

		cmd = Cfg_p->Server_cmd( Cfg_p->Config.Nohup, bin, opt );
		Push_sys( CMD_SYS, cmd );

		return;
	}

	Wrong_keyword(  expect , keyword.Str );
	return ;
}

void Interpreter_class::Stop_bin( string_vec_t arr )
{
	expect = { "AudioServer", "Synthesizer" };
	Intro( arr, 1 );
	SetKeyword		( 1 );

	if ( strEqual( keyword.Str, "Synthesizer" ) )
	{
		Comment( INFO, "terminating " + keyword.Str );
		Push_ifd( 	&sds_p->appstate_arr[SYNTHID].state,
					StateId_e::EXITSERVER, "terminating " + keyword.Str );
		Push_key( EXITKEY, "EXITKEY" + keyword.Str );
		return;
	}
	if ( strEqual( keyword.Str, "AudioServer" ) )
	{
		Comment( INFO, "terminating " + keyword.Str );
		Push_ifd( 	&sds_p->appstate_arr[AUDIOID].state,
					StateId_e::EXITSERVER, "terminating " + keyword.Str  );
		return;
	}

	Wrong_keyword( { "AudioServer, Synthesizer" }, arr[1] );

}

void Interpreter_class::SetKeyword( uint8_t min, uset_t valid )
{
	error 			= 0;
	duration 		= 0;
	string str 		= pop_stack( min );
	keyword.Str 	= str;
	if( min == 0 )
		return;
	if( valid.size() > 0 )
	{
		if( valid.contains( str ) )
			return;
		else
		{
//			keyword.Str = "";
			error = 1;
		}
	}

}
void Interpreter_class::Intro( string_vec_t arr, uint min )
{
	auto arr_to_str = [ arr ]( int no )
	{
		stringstream strs {};
		strs << setw(4) << left << no << ">" << show_type( arr );
		return strs.str();
	};

	option_default	= "";
	set_stack		( arr, min );
	command 		= arr_to_str( Processor_class::txtline );
	Push_text		( command );

}

void Interpreter_class::RecFile( string_vec_t arr )
{
	// record start
	// record stop test
	// record play test

	expect 		= { "stop", "start", "play", "loop" };
	Intro		( arr, 2);
	SetKeyword	( 1, expect );

	if ( Cmpkeyword( "start" ) )
	{	// set state to not recording
		Push_ifd( &sds_master->Record_state, StateId_e::INACTIVE, "recording" );
		Push_key( EXTERNAL_WAVFILEKEY, 	"EXTERNAL_WAVFILEKEY: start record" );

		return;
	}
	if ( Cmpkeyword( "stop") ) // stop record and write file with synthesizer.record_thead_fcn
	{
		expect = { "File name" };
		string wavfile = pop_stack( 1 );
		Push_ifd( &sds_master->FileNo, USEFILENAME,	"record file" ); // trigger record_thead_fcn
		// take filename from the Other-cstring
		Push_str( WAVFILE_STR, wavfile );
		// force recordstop state
		Push_ifd( &sds_master->Record_state, RECORDING, "record stop" );//set by Audioserver
		Push_key( EXTERNAL_WAVFILEKEY, "EXTERNAL_WAVFILEKEY" ); // trigger record_thead_fcn
		return;
	}
	if ( Cmpkeyword( "play") )
	{
		expect 			= { "file name", "replay duration in seconds" };
		string wavfile 	= pop_stack( 1 );
		Push_ifd( &sds_p->StA_state_arr[ STA_EXTERNAL ].start, true, "StA start" );
		Push_str( WAVFILE_STR, wavfile );
		Push_key( READ_WAVFILE_KEY, "READ_EXTERNAL_WAVFILE" );

		return;
	}
	if ( Cmpkeyword( "loop") )
	{
		uint8_t amp = pop_T( volidx_range );
		uint8_t time = pop_T( percent_range );
		amploop( amp, time ); // TODO - working
		return;
	}

	Wrong_keyword( expect , keyword.Str );

}

void Interpreter_class::Random( string_vec_t arr )
{
	// generate random notes
	// random |2C-F- 8 CFFDH- cvcv auto


	expect = { "constant note list"};
	Intro( arr, 5 );
	SetKeyword		( 1 );

	string Constant = keyword.Str;
	Variation.Define_fix( Constant );

	expect = { "rhythm list " };
	string rhythm = pop_stack( 2 );
	Variation.Define_rhythm( rhythm );

	expect = { "random note set" };
	string Variable = pop_stack( 2 );
	Variation.Define_variable ( Variable );

	expect = { "const/random pattern "};
	string Pattern  = pop_stack( 2 );

	expect = { "notes file name " };
	string filename = pop_stack( 1);

	// write randomly generated note sentence to file filename
	string random_noteline = Variation.Gen_noteline( Pattern , filename );
	cout << random_noteline << endl;

	string_vec_t vec = { "notes", "load", filename };
	Notes( vec );

}

void Interpreter_class::Debug( string_vec_t arr )
{
	Intro( arr, 2 );
	expect = { "on", "off" };
	SetKeyword( 1, expect );
	if( Cmpkeyword( "on" ) )
	{
		Push_wait( CMD_COND_WAIT, DEBUGON, "Debug on");
		return;
	}
	if( Cmpkeyword( "off" ) )
	{
		Push_wait( CMD_COND_WAIT, DEBUGOFF, "Debug off");
		return;
	}
	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::Dynamic( string_vec_t arr )
{
	// parameter
	// #1: staid
	// #2: volume
	// #3: value in %
	// #4: frq name
	// commands
	// dyn vol #1 fixed #2
	// dyn vol #1 slide #3

	// dyn frq #1 fixed #4
	// dyn frq combine
	// dyn frq slide #3

	expect 		= { "vol", "frq" };
	Intro		( arr, 1 );
	SetKeyword	( 1, expect );

	if( Cmpkeyword( "vol" ) )
	{
		StAId_e	staid	= expect_staid();
		if( staid == STAID_ALL )
			Exception( "keyword all is not supported in this context" );

		expect 			= { "fixed" };
		SetKeyword		( 1, expect );
		if( Cmpkeyword( "fixed" ) )
		{
			Push_ifd( &sds_p->StA_amp_arr[staid].mode, FIXED, "fixed vol mode" );
			uint8_t volume = pop_T( volidx_range );
			Push_ifd( &sds_p->StA_amp_arr[staid].amp, volume, "volume" );
			Push_key( STA_VOLUME_KEY, "STA_VOLUME_KEY" );
			return;
		}
		if( Cmpkeyword( "slide" ))
		{
			Push_ifd( &sds_p->StA_amp_arr[staid].mode, SLIDE, "dynamic vol mode" );
			uint8_t value 	= pop_T( percent_range );
			Push_ifd( &sds_p->StA_amp_arr[staid].slide, value, "duration" );
			return;
		}
		Wrong_keyword( expect, keyword.Str );
		return;
	}

	// dyn frq fixed #4
	// dyn frq slide #3
	// dyn frq combine
	if( Cmpkeyword( "frq" ) )
	{
		expect = { "fixed", "slide", "combine" };
		SetKeyword	( 1, expect );
		if( Cmpkeyword( "fixed" ) )
		{
			Push_ifd( &sds_p->frq_slidermode, FIXED, "fixed frq mode" );
			expect = { "frequency name" };
			string frq_str { pop_stack(1) };
			uint8_t freq = Frequency.Index( frq_str );
			Push_ifd(  main_view.frqidx, freq, "frq index"  );
			Push_key(  main_view.freqkey, "set frequency"  );
			return;
		}

		if( Cmpkeyword( "slide" ) ) // dyn frq slide 20
		{
			uint8_t value = pop_T( percent_range );
			Push_ifd( &sds_p->frq_slidermode, SLIDE, "dynamic frq slide" );
			Push_ifd( &sds_p->features[OSCID].frq_slide_value, value, "dynamic frequency" );
			Push_ifd( &sds_p->features[FMOID].combine, true, "combine fmo" );
			Push_ifd( &sds_p->features[VCOID].combine, true, "combine vco" );
			Push_key( FREQDYNAMIC_KEY, "FREQDYNAMIC_KEY, set OSC" );
			return;
		}
		if( Cmpkeyword( "combine" ) )
		{
			Push_ifd( &sds_p->features[FMOID].combine, true, "fmo combine" );
			Push_ifd( &sds_p->features[VCOID].combine, true, "vco combine" );
			Push_key(  FEATURE_KEY, "FEATURE_KEY, osc combine with vco and fmo"  );
			return;
		}
		Wrong_keyword( expect, keyword.Str );
		return;
	}
	Wrong_keyword( expect , keyword.Str );

}

void Interpreter_class::Register( string_vec_t arr )
{
	// reg #1 load #2
	// reg #1 dec
	// #1	register id 0..7
	// #2	integer value

	auto get_regid = [ this ](  )
	{
		expect = regidstr_set;
		SetKeyword( 1, expect );
		uint8_t regid = keyword.secure_stoi();
		return regid;
	};

	Intro( arr, 1 );

	uint8_t regid	= get_regid();

	expect = { "load", "dec", "inc", "show" };
	SetKeyword( 1, expect );
	if( Cmpkeyword( "dec" ) )
	{
		Assembler_cmd( CMD_ADD, regid, -1, "dec" );
		return;
	}
	if( Cmpkeyword( "inc" ) )
	{
		Assembler_cmd( CMD_ADD, regid, 1, "inc" );
		return;
	}
	range_T<int> integer_range{	(int)-32756, (int)32756 };
	if( Cmpkeyword( "load" ) )
	{
		expect = { "integer value" };
		int value = pop_T( integer_range );
		Assembler_cmd( CMD_LDX, regid, value, "dec" );

		return;
	}
	if( Cmpkeyword( "show" ) )
	{
		Assembler_cmd( CMD_OUT, regid, 0, "out" );
		return;
	}
	Wrong_keyword( expect, keyword.Str );
}
void Interpreter_class::Ifpos( string_vec_t arr )
{
	Assembler_cmd( CMD_POS, 0, 0, "push" );
}
void Interpreter_class::Endif( string_vec_t arr )
{	// "if" has marked its processor stack pos
	// "endif" shall return to this position if the register number arg[1] is zero
	// else the next processor stack will be executed
	expect = regidstr_set;
	Intro( arr, 1 );
	SetKeyword( 0, expect );
	if( keyword.Str.length() == 0 ) // see pre compiler repeat
	{
		Assembler_cmd( CMD_ADD, 0, -1, "dec" );
		Assembler_cmd( CMD_JMP, 0, 0, "jump" );
		return;
	}
	if ( regidstr_set.contains( keyword.Str ) )
	{
		int regid = keyword.secure_stoi();
		Assembler_cmd( CMD_JMP, regid, 0, "jump" );
		return;
	}
	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::Reset( string_vec_t arr )
{
	expect = { "sds" };
	Intro( arr, 1 );
	SetKeyword		( 1 );

	if( Cmpkeyword( "sds" ) )
	{
		Push_key( RESETSTA_KEY, "reset sds" );
		return;
	}
	Wrong_keyword( expect , keyword.Str );

}

void Interpreter_class::Set( string_vec_t arr )
{	// examples:
	// set main|vco|fmo freq A3
	// set main|vco|fmo amp|wf 0
	// set octave+
	// set dir first
	// set synthesizer 1
	// set rhythm 0

	expect = { "octave+", "octave-", "main", "vco", "fmo", "dir", "synthesizer", "rhythm" };
	Intro( arr, 2 );
	SetKeyword		( 1 );

	if ( Cmpkeyword( "octave+" ) )
	{
		Push_ifd( &sds_p->Noteline_prefix.octave_shift, (int8_t)1, "octave" );
		Push_key(MAINFRQ_KEY, "MAINFRQ_KEY, inc octave");
		return;
	}
	if ( Cmpkeyword( "octave-" ))
	{
		Push_ifd( &sds_p->Noteline_prefix.octave_shift, (int8_t)(-1), "octave" );
		Push_key(MAINFRQ_KEY, "MAINFRQ_KEY, dec octave");
		return;
	}
	if ( Cmpkeyword( "main" ) or Cmpkeyword( "vco" ) or Cmpkeyword( "fmo" ))
	{
		string osc = keyword.Str;
		expect = { "freq", "wf", "amp" };
		keyword.Str = pop_stack( 2 );

		if( Cmpkeyword( "freq") )
		{
			string value{ pop_stack( 1 ) };
			Osc({ "osc", osc, "freq", value });
			return;
		}
		if( Cmpkeyword( "wf") )
		{
			Value value{ pop_T( wfid_range ) };
			Osc({ "osc", osc, "wf", value.str });
			return;
		}
		if( Cmpkeyword( "amp") )
		{
			uint8_t 	amp 	= pop_T( volidx_range );
			Osc({ "osc", osc, "amp", to_string( amp ) });
			return;
		}
		Wrong_keyword( expect , keyword.Str );

	}
	if( Cmpkeyword( "dir" ) )
	{
		string subdir = pop_stack( 1 );
		Push_str( SESSIONSTR,	subdir );
		Push_key( SETSESSIONDIR_KEY, "SETSESSIONDIR_KEY" );
		return;
	}
	if( Cmpkeyword( "synthesizer" ) )
	{
		int config	= pop_T( sds_range );
		sds_p		= DaTA_p->SDS.GetSdsAddr( config );
		set_osc_view( sds_p );
		Push_ifd	( &sds_master->comId, Id_t(config), "set synthesizer " + to_string( config ));
		// if synthesizer is not started apply autostart
		Push_key	( COMMIT_KEY, "COMMIT_KEY " + to_string( config ));
		return;
	}
	if( Cmpkeyword( "rhythm" ) )
	{
		expect = Bps.Bps_str_set;
		int clock = pop_T( Bps.set_range );
		if( Bps.Bps_set.contains( clock ) )
		{
			Push_ifd( &sds_p->instrumentClock, uint8_t(clock), "rhythm" );
			Push_key( BEATCLOCK_KEY, "BEATCLOCK_KEY");
		}
		else
		{
			Wrong_keyword( expect, keyword.Str );
		}
		return;
	}
	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::Notes( string_vec_t arr )
{
	expect = { "load", "save", "play", "set", "gen", "per_second"};
	Intro( arr, 2 );
	SetKeyword( 1, expect );

	Push_ifd( &sds_p->NotesTypeId, NTE_ID, "nte type" );
	Push_ifd( &sds_p->Noteline_prefix.origin, NOTE::COMPOSE, "origin composer" );
	Push_ifd( &sds_p->Noteline_prefix.convention, 0_uint, "convention english" );

	if ( Cmpkeyword( "load") ) 		// notes load tmp
									// load notes from file and start playing
	{
		expect = { "Notes name" };
		string notes_name = pop_stack( 1);
		Comment( INFO, "loading notes " + notes_name );
		check_file( { 	fs->autodir,
						fs->notesdir }, notes_name + fs->nte_type );

		Push_str( NOTENAMESTR, notes_name );
		Push_key( READNOTESFILE_KEY, "READNOTESFILE_KEY " + notes_name );
		Push_key( UPDATENOTESKEY, "UPDATENOTESKEY" );
		return;
	}
	if( Cmpkeyword( "save" ) ) // notes save tmp
	{
		// save noteline, noteline prefix, rhythm line
		// from sds to file in session dir
		string filename = pop_stack( 1 );
		Variation.Save( filename, Variation.Noteline_prefix.data, Variation.Noteline.get() );

		return;
	}
	if ( Cmpkeyword( "set") ) // set properties only, but do not play
	{
		expect		=	{"prefix","octave", "[num]","sentence", "rhythm"};
		keyword.Str = pop_stack(1);

		if ( Cmpkeyword("prefix") )  // notes set prefix 1 0
		{
			expect 		= {"#flats", "#sharps" };
			uint8_t flats	= pop_T( accidental_range );
			uint8_t sharps	= pop_T( accidental_range );
			Variation.Set_note_chars( flats, sharps );

			Push_ifd( &sds_p->Noteline_prefix.flat 	,flats, "update flats" );
			Push_ifd( &sds_p->Noteline_prefix.sharp ,sharps, "update sharps" );
			Push_key( UPDATE_NLP_KEY, "UPDATE_NLP_KEY");
			return;
		}
		if( Cmpkeyword("octave") )	// notes set octave 4
		{
			expect = { "octave value" };
			uint8_t oct = pop_T( octave_range ) ;
			Variation.Set_prefix_octave( oct );
			Push_ifd( &sds_p->Noteline_prefix.Octave ,oct, "update octave" );
			Push_key( UPDATE_NLP_KEY, "UPDATE_NLP_KEY");

			return;
		}
		// notes [num] noteline rythmline
		if ( Cmpkeyword( "num" ) )	// notes set num 0232 8642
		{
			Variation.Noteline.set( pop_stack(1) );
			Variation.Set_note_chars( 1 );
		}
		else						// notes set ACDC 8642
		{
			Variation.Noteline.set( keyword.Str );
			Variation.Set_note_chars( 0 );
		}

		expect = {"rhythm line" };
		Variation.Set_rhythm_line( pop_stack(1) );

		Push_str( NOTELINESTR	, Variation.Noteline.get() );
		Push_str( RHYTHMLINESTR, Variation.Rythmline.get() );

		return;
	}
	if( Cmpkeyword( "gen" ) )			// notes gen amp x%
	{
		expect = { "amp" };
		SetKeyword( 1, expect );
		if( Cmpkeyword( "amp" ) )
		{
			uint8_t amp = pop_T( percent_range );
			Push_ifd( &sds_p->StA_amp_arr[ STA_NOTES ].amp, amp, "set vol" );
			Push_key( GENERATENOTES_KEY, "GENERATENOTES_KEY" );
			Push_key( NOTESONKEY, "NOTESONKEY" );

			return;
		}
	}
	// notes play notes ACDC
	if ( Cmpkeyword( "play") )
	{
		Comment( INFO, "playing notes " );
		expect = { "on", "off", "notes" };
		keyword.Str = pop_stack( 1);
		if ( Cmpkeyword( "on") )		//notes play on
		{
			expect = { "volume [%]" };
			uint8_t 	amp 	= pop_T( volidx_range );
			Push_ifd( &sds_p->StA_amp_arr[STA_NOTES].amp ,amp, "play notes" );
			Push_key( NOTESONKEY, "NOTESONKEY");
			return;
		}
		if ( Cmpkeyword( "off") )		//notes play off
		{
			Comment( INFO, "playing notes off " );
			Push_key( NOTESOFFKEY, "NOTESOFFKEY" );
			return;
		}
		if ( Cmpkeyword( "notes"	) )	// notes play notes ACDC 8642
		{
			expect = { "Notes", "Rhythm" };
			Variation.Noteline.set( pop_stack( 2 ) );
			expect = { "Rhythm" };
			Variation.Rythmline.set( pop_stack( 1 ) );

			Push_str( NOTELINESTR, Variation.Noteline.get() );
			Push_str( RHYTHMLINESTR, Variation.Rythmline.get() );
			Push_key( GENERATENOTES_KEY, "GENERATENOTES_KEY" );
			Push_key( NOTESONKEY, "NOTESONKEY" );

			return;
		}
		Wrong_keyword( expect, keyword.Str );
		return;
	}
	if ( Cmpkeyword( "per_second" ) )		// notes per_second 4
	{
		expect = {"notes per second [ " + show_items( Variation.Noteline_prefix.Bps_vec ) +  " ]"};
		string nps_str = pop_set( Variation.Noteline_prefix.Bps_str_set );
		uint8_t nps = uint8_t( stoi( nps_str ) );
		Variation.Noteline_prefix.data.nps = nps;
		Push_ifd( &sds_p->Noteline_prefix.nps, nps, "notes per second"  );
		Push_key( UPDATE_NLP_KEY, "UPDATE_NLP_KEY" );
		return;
	}
	Wrong_keyword( expect, keyword.Str );
}


void Interpreter_class::Instrument( string_vec_t arr )
{
	expect 		= { "load", "beat" };
	Intro		( arr, 2 );
	SetKeyword	( 1, expect );

	if ( Cmpkeyword( "load") )
	{
				expect	= { "instument name "};
		string	instr	= pop_stack( 1 );
		string	instrdir= fs->SubDir( fs->instrumentdir, sds_master->Session );
		Info( "loading instrument ", instr, "from", instrdir );
		check_file( { instrdir } , instr + fs->snd_type );
		Push_str( INSTRUMENTSTR, instr );
		Push_key( SETINSTRUMENTKEY, "SETINSTRUMENTKEY" );
		return;
	}
	if ( Cmpkeyword( "beat" ) )
	{
				expect 	= { "# of beats per second [" + Bps.Bps_str + "]" };
		uint8_t count	= pop_T( Bps.set_range );
		if( not Bps.Bps_set.contains( count ) )
		{
			wrong_argument( expect, (int)count );
		}
		Info( "Set beat count for the instrument to: ", (int)count );
		Push_ifd( &sds_p->instrumentClock, count, "beat clock" );
		Push_key( BEATCLOCK_KEY, "BEATCLOCK_KEY");
		return;
	}
	Wrong_keyword(  expect , keyword.Str );
}

void Interpreter_class::SubArgs( string varname, string varvalue )
{
	/*
	 call :subtext Hello
	 ...
	 call :subtext World
	 ...
	 :subtext A
	 text A
	 return
	 ...
	*/

	if ( varname.length() == 0 )
		return; // nothing to do


	string_vec_t arr = { varname, "=", varvalue };
	Addvariable( arr );

	return;
}

void Interpreter_class::AddMusic( string_vec_t arr )
{
	expect 		= { "instrument name", "notes name" };
	Intro		( arr, 2 );
	SetKeyword	( 1, expect );

	string 	inst= keyword.Str;
	string	note= pop_stack( 1 );

	arr 		= { "instrument" , "load" , inst }; 	// SETINSTRUMENTKEY
	Instrument	( arr );
	arr 		= {"notes" , "load" , note };			// UPDATENOTESKEY
	Notes		( arr );
}

void Interpreter_class::Osc( string_vec_t arr )
{	// osc [main|vco|fmo] set waveid frequency amplitude duration
	// osc [main|vco|fmo] waveid frequency amplitude

	expect				= { "main", "vco", "fmo" };
	Intro				( arr, 3);
	SetKeyword			( 1, expect );

	view_struct_t view = view_struct();
	if ( Cmpkeyword( "main" ))
	{
		view = main_view;
	}
	if ( Cmpkeyword( "fmo") )
	{
		view = fmo_view;
	}
	if ( Cmpkeyword( "vco") )
	{
		view = vco_view;
	}

	string_vec_t tmp 	= arr;
	keyword.Str = arr[2];
	if ( Cmpkeyword( "set") )
	{
		if ( not check_count( stack, 5) ) return;
		tmp = { "set","wf", arr[3], "0" };
		osc_view(view, tmp);
		tmp = { "set","freq", arr[4], "0" };
		osc_view(view, tmp);
		tmp = { "set","amp", arr[5], arr[6] };
		osc_view(view, tmp);
	}
	else
	{
		arr.erase( arr.begin());
		osc_view( view, arr );	// set wfid, frq, amp
	}

}

void Interpreter_class::osc_view( view_struct_t view, string_vec_t arr )
{
	expect = { "mute", "unmute", "reset", "frq", "loop", "wf", "amp" };
	Intro( arr, 2);
	SetKeyword		( 1 );

	bool loop = false;

	if ( Cmpkeyword( "mute") )
	{
		Comment( INFO, "toggle mute" );

		Push_key( MASTERAMP_MUTE_KEY, "MASTERAMP_MUTE_KEY mute" );
		return;
	}

	if ( Cmpkeyword( "unmute") )
	{
		Comment( INFO, "toggle Mute " );

		Push_key( MASTERAMP_MUTE_KEY, "MASTERAMP_MUTE_KEY un-mute " );
		return;
	}
	if ( Cmpkeyword( "reset" ))
	{
		Comment( INFO, "Reset connections");
		Push_ifd( &sds_p->connect_arr[OSCID].frq, OSCID, "reset fmo connect" );
		Push_ifd( &sds_p->connect_arr[OSCID].vol, OSCID, "reset vco connect" );
		Push_key( CONNECTOSC_KEY , "CONNECTOSC_KEY, reset main"  );
		return;
	}


	if ( Cmpkeyword( "wf") )
	{
		expect = vector2uset( Spectrum.Get_waveform_vec() );
		string waveform = pop_set( waveform_str_set );

		uint8_t wfid = Spectrum.Get_waveform_id( waveform );
		if ( wfid < 0 )
		{
			Wrong_keyword( expect, waveform );
			If_Exception( "wrong keyword" );
		}
		expect 		= { " duration in seconds" };
		option_default = "0";
		string duration = pop_stack(0);
		Comment( INFO, "Set waveform " + waveform + " for " + view.name );

		Push_ifd( view.wf, wfid, "wafeform");
		Push_key( view.wfkey, "set waveform" );
		Pause( { "pause", duration });
		return;
	}

	if ( Cmpkeyword( "loop") )
	{
		keyword.Str = pop_stack( 1);
		loop = true;
		// TODO
	}

	if ( Cmpkeyword( "amp") )
	{
		expect 		= { "volume [%]" };
		uint8_t amp = pop_T( volidx_range );
		Comment( INFO, "Set amplitude " + to_string(amp) + " for " + view.name );

		if ( loop ) // TODO-working
		{
			uint time = pop_T( percent_range );
			amploop( amp, time );
		}
		else
		{
			if( view.ampkey == STA_VOLUME_KEY )
				Push_ifd( &sds_p->StA_Id, STA_INSTRUMENT, "StA id" );
			Push_ifd( view.amp, amp, "volume"  );
			Push_key( view.ampkey, "set volume" );

			expect 		= { " duration in seconds" };
			option_default = "0";
			string duration = pop_stack( 0 );
			Pause( { "pause", duration });
 		}
		return;
	}

	if ( Cmpkeyword( "freq") )
	{
		Push_ifd( &sds_p->connect_arr[OSCID].frq , FMOID, "connect frq->osc" );

		if ( loop )
		{
			Push_ifd( &sds_p->frq_slidermode, SLIDE, "slide mode" );
			Push_key(  FREQDYNAMIC_KEY, "FREQDYNAMIC_KEY"  );
		}

		expect = { "frequency name or index" };
		uset_t frqname_set = array2uset( frqNamesArray );

		string f  = pop_set( frqname_set );//{ pop_stack(1) };
		uint8_t freq = Frequency.Index( f );

		Comment( INFO, "Set frequency " + to_string(freq) + " for " + view.name );
		Push_ifd(  view.frqidx, freq, "frq index"  );
		Push_key(  view.freqkey, "set frequency"  );

		expect 		= { " duration in seconds" };
		option_default = "0";
		string duration = pop_stack( 0 );
		Pause( { "pause", duration });

		return;
	}
	Wrong_keyword( expect, keyword.Str );
}

//void Interpreter_class::Play( string_vec_t arr )
//{ 	// play 1 amp 30
//	// play 1 amp loop 30
//	// play 1 start
//
//	Intro( arr, 2 );
//	expect = { "0", "1", "2", "3", "4", "5", "6", "7" };
//	SetKeyword( 1, expect );
//	StAId_e staid = (StAId_e)keyword.secure_stoi();
//
//	expect 			= { "amp", "start"  };
//	SetKeyword( 1, expect ) ;
//	if ( Cmpkeyword( "amp") )
//	{
//		expect = { "loop", "duration in sec." };
//		string k2 = pop_stack( 1); // loop or duration
//		keyword.Str = k2;
//		if ( Cmpkeyword( "loop") )
//		{
//			expect = { " dest amp"};
//			uint8_t vol = pop_T( percent_range );
//			Push_ifd( &sds_p->StA_Id , staid, "StA id" );
//			Push_ifd( &sds_p->StA_amp_arr[staid].amp , vol, "% slide duration " );
//			Push_ifd( &sds_p->StA_amp_arr[staid].mode , SLIDE, "slide mode" );
//			Push_ifd( &sds_p->StA_state_arr[staid].start , true, "play on" );
//			Push_key( STA_VOLUME_KEY, "STA_VOLUME_KEY, with dynamic volume" );
//			return;
//		}
//		else
//		{
////			option_default		= "10";
//			string duration 	= k2;
//			StA( { "sta", to_string( staid ), "amp",  duration } );
//			return;
//		}
//	}
//	if( Cmpkeyword( "start" ) )
//	{
//		Push_ifd( &sds_p->StA_Id , staid, "StA id" );
//		Push_key( STASTART_KEY, "STASTART_KEY, start read StA" );
//
//		return;
//	}
//	Wrong_keyword( expect , keyword.Str );
//
//}

StAId_e Interpreter_class::expect_staid()
{
	expect 		= { "0", "1", "2", "3", "4", "5", "6", "7", "all" };
	SetKeyword	( 1, expect );

	StAId_e staid = STAID_ALL;
	if( not Cmpkeyword( "all" ) )
	{
		staid = (StAId_e)keyword.secure_stoi();
	}
	Push_ifd( &sds_p->StA_Id, staid, "StA id" );
	return staid;
}

void Interpreter_class::StA( string_vec_t arr )
{
	Intro( arr, 2 );

	// push the sta id (all=STAID_ALL) into the processor stack
	StAId_e staid = expect_staid();

	expect = { "store", "state", "amp", "stop", "play", "mute" , "clear" };
	SetKeyword( 1, expect );


	// sta #1 play [on|off]
	auto setstart_flag = [ this ]( StAId_e id, bool flag )
	{
		bool* addr = ( flag ) ? &sds_p->StA_state_arr[id].start : &sds_p->StA_state_arr[id].play;
		Push_ifd( addr, flag, keyword.Str );
	};
	auto sta_amp_loop = [ this ]( StAId_e id, uint8_t amp0, uint8_t amp, uint8_t slide )
	{
		Push_ifd( &sds_p->StA_amp_arr[id].amp	, amp0, "start volume" );
		Push_ifd( &sds_p->StA_amp_arr[id].slide, uint8_t( 0 ), "Fixed" );
		Push_ifd( &sds_p->StA_amp_arr[id].mode	, FIXED, "FIXED" );
		Push_key( STA_VOLUME_KEY, "STA_VOLUME_KEY" );

		string time_str = to_string( slide ) + "% of " + to_string( max_slide_sec ) + " [sec]";
		Push_ifd( &sds_p->StA_amp_arr[id].amp 	, amp, "destination volumn" );
		Push_ifd( &sds_p->StA_amp_arr[id].slide , slide, "slide " + time_str );
		Push_ifd( &sds_p->StA_amp_arr[id].mode	, SLIDE, "SLIDE" );
		Push_ifd( &sds_p->StA_state_arr[id].start, true, "start true" );
		Push_key( STA_VOLUME_KEY, "SETSTA_KEY" );
	};

	if ( Cmpkeyword( "play") )
	{
		expect 			= { "on", "off" };
		SetKeyword		( 1, expect );
		bool flag 		= Cmpkeyword( "on" ) ;
		if ( staid == STAID_ALL )
		{
			for( StAId_e staid : StAIds )
			{
				setstart_flag( staid, flag );
			}
			Push_key( SETSTA_KEY, "SETSTA_KEY, StA State" );
			return;
		}
		setstart_flag( staid, flag );
		Push_key( SETSTA_KEY, "SETSTA_KEY, StA State" );
		return;
	}

	if ( Cmpkeyword( "clear") )
	{
		Push_key( CLEAR_KEY,  "CLEAR_KEY" );
		return;
	}

	if ( Cmpkeyword( "mute") )
	{
		Push_key(MUTESTA_KEY,  "MUTESTA_KEY, stop sound" );
		return;
	}

	if( staid == STAID_ALL ) // the subsequent cases will not consider staid_all
		If_Exception( "keyword <all> not supported" );

	if( Cmpkeyword( "state" ) )
	{
		string staid_str = to_string( staid );
		Push_sys( CMD_STASTATE, staid_str );
		return;
	}

	if ( Cmpkeyword( "amp") )
	{	// sta 4 amp 100
		// sta 4 amp 0 loop 100 20

		uint8_t amp0		= pop_T( volidx_range );

		expect = { "loop" };
		SetKeyword( 0, expect );
		if ( Cmpkeyword( "loop" ) )
		{	// sta 4 amp 0 loop 100 20
			uint8_t 	end 	= pop_T( volidx_range );
			uint8_t 	time	= pop_T( percent_range );
			sta_amp_loop( staid, amp0, end, time );
			return;
		}
		Push_ifd( &sds_p->StA_amp_arr[ staid ].amp	, amp0, "StA volume" );
		Push_ifd( &sds_p->StA_amp_arr[ staid ].slide, uint8_t( 0 ), "Fixed" );
		Push_ifd( &sds_p->StA_amp_arr[ staid ].mode	, FIXED, "StA mode" );
		Push_key( STA_VOLUME_KEY		, "SETSTA_KEY" );
		return;
	}

	if ( Cmpkeyword( "store") )
	{
		Push_ifd( &sds_p->StA_state_arr[staid].store, true, "store" );
		Push_key( STA_RECORD_KEY, "STA_RECORD_KEY, store sound" );
		return;
	}

	if ( Cmpkeyword( "stop") )
	{	// rec staid stop
		Push_ifd( &sds_p->StA_state_arr[staid].store, false, "store" );
		Push_key( STA_RECORD_KEY, "STA_RECORD_KEY, stop record" );
		return;
	}

	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::Text( string_vec_t arr )
{
	expect = { "text" };
	Intro( arr, 1 );
	SetKeyword		( 1 );

	string text = keyword.Str ;
	for ( string str : stack )
	{
		text.append(" ");
		text.append(str);
	}

	Push_text( text );
}

void Interpreter_class::Adsr( string_vec_t arr )
{
	expect =  { "pmw", "hall", "attack", "decay", "softfreq", "beat" };
	Intro( arr, 2 );
	SetKeyword		( 1 );

	if ( Cmpkeyword( "softfreq") )
	{
		Comment( INFO, "soft frequency is set to: " + stack[0] );
		uint8_t freq = pop_T( percent_range );
		Push_ifd( &sds_p->features[OSCID].frq_slide_value, freq, "soft freq"  );
		Push_key( FREQDYNAMIC_KEY,  "FREQDYNAMIC_KEY" );
		return;
	}
	if ( Cmpkeyword( "beat") )
	{
		Comment( INFO, "beat duration is set to: " + stack[0] );
		expect 		= Bps.Bps_str_set;
		string 	bps_str = pop_stack(1 );
		uint8_t	bps	= char2int( bps_str[0]);
		if ( not Bps.Bps_set.contains( bps ) )
		{
			Wrong_keyword( expect , bps_str );
			If_Exception( "wrong beat duration" );
		}

		Push_ifd( &sds_p->adsr_arr[OSCID].bps, bps, "beat duration" );
		Push_key( ADSR_KEY, "ADSR_KEY" );
		return;
	}
	if ( Cmpkeyword( "attack") )
	{
		Comment( INFO, "beat attack is set to: " + stack[0] );
		uint8_t attack = pop_T( percent_range );
		Push_ifd( &sds_p->adsr_arr[OSCID].attack, attack, "adsr attack" );
		Push_key( ADSR_KEY, "ADSR_KEY" );
		return;
	}
	if ( Cmpkeyword( "decay") )
	{
		Comment( INFO, "beat decay is set to: " + stack[0] );
		uint8_t decay = pop_T( percent_range );
		Push_ifd( &sds_p->adsr_arr[OSCID].decay, decay, "adsr decay" );
		Push_key( ADSR_KEY, "ADSR_KEY" );
		return;
	}
	if ( Cmpkeyword( "hall") )
	{
		Comment( INFO, "hall effect is set to: " + stack[0] );
		uint8_t hall = pop_T( percent_range );
		Push_ifd( &sds_p->adsr_arr[OSCID].hall, hall, "hall"  );
		Push_key( ADSR_KEY,  "ADSR_KEY" );
		return;
	}
	if ( Cmpkeyword( "pmw") )
	{
		Comment( INFO, "PMW is set to: " + stack[0] );
		uint8_t dial = pop_T( percent_range );
		Push_ifd( &sds_p->features[VCOID].PWM, dial, "pmw" );
		Push_key( PWMDIALKEY, "PWMDIALKEY" );
		return;
	}

	Wrong_keyword(expect, keyword.Str );

}

void Interpreter_class::Pause( string_vec_t arr )
{
	if( arr[1].length() == 0 )
		return;
	expect 		= { "duration in seconds [ms] ", "auto", "key", "dynvol", "dynfrq" };
	Intro( arr, 0 );
	SetKeyword		( 1 );

	if ( Cmpkeyword( "key" ))
	{
		Push_wait( CMD_COND_WAIT, KEY, "press <ret> to continue" );
		return;
	};
	if ( Cmpkeyword( "auto" ) )
	{
		Push_wait( CMD_COND_WAIT, AUTO, "auto" ); // 3 = default case
		return;
	}
	if ( keyword.is_number() )
	{
		duration = stoi( keyword.Str );
		if( duration == 0 )
			return;
		expect = { "ms" };
		string unit = pop_stack( 0 );
		if( not strEqual( "ms", unit ) )
		{
			duration = 1000 * duration;
		}
		Push_wait( CMD_WAIT, duration, "[msec]" );
		error = 0;
		return;
	}
	if( Cmpkeyword( "dynvol" ) )
	{
		Push_wait( CMD_COND_WAIT, DYNVOL, "dynvol" );
		return;
	}
	if( Cmpkeyword( "dynfrq" ) )
	{
		Push_wait( CMD_COND_WAIT, DYNFRQ, "dynfrq" );
		return;
	}

	Wrong_keyword( expect, keyword.Str );
}


void Interpreter_class::Addvariable( string_vec_t arr )
{
	auto key =  [] ( string_vec_t a )
		{
			return ( a.size() > 0 ) ? a[0] : "";
		};

	auto is_notesfile = [ this ](string str )
		{
			string filename = fs->notesdir + str + fs->nte_type;
			return filesystem::exists(filename);
		};

	auto add_notesfile = [ this ]( string varname, string filea, string fileb )
		{
			string srca = fs->notesdir + filea + fs->nte_type;
			string srcb = fs->notesdir + fileb + fs->nte_type;
			string dest = fs->autodir + varname + fs->nte_type;
			string cmd = "cat " +  srca + " " + srcb + " > " +  dest;
			System_execute( cmd );
		};

	error = 0;
	expect = { "Syntax: filename = file1 + file2" };
	string varname 	= key(arr); // first string needed

	set_stack( arr, 2);
	keyword	= pop_stack( 1);
	if ( not Cmpkeyword( "=" ))
	{
		Info( keyword.Str , " is not a keyword");
		Info( "command is not an assignment" );
		Wrong_keyword( expect, arr[0] );
	}
	string varvalue 	= pop_stack( 1) ;
	if ( Keywords.contains( varvalue ) )
	{
		if ( not testrun )
			If_Exception( "varvalue " + varvalue + " is keyword"  );//raise( SIGINT);
		testreturn = true;
	};

	if (( arr.size() > 0 ) and ( is_notesfile( varvalue) ))
	{
		expect = { "+", "notesfile name" };
		string op = pop_stack( 2 );
		if ( strEqual( op, "+" ))
		{
			expect = { "notesfile name" };
			string name = pop_stack( 1 );
			if ( is_notesfile( name ))
				add_notesfile( varname, varvalue, name );
			else
			{
				Comment(INFO, "expected note file name, Ignoring option: add file");
			}
		}
		else
		{
			Comment(INFO, "expected +");
			error = 1;
			return;
		}
	}

	Wrong_keyword( expect, keyword.Str );
}


void Interpreter_class::amploop( uint8_t vol, uint8_t time ) // TODO
{	// distinguish between master volumn and sta volumn
	Push_ifd( &sds_master->Master_Amp 		, vol	, "slide volume" );
	Push_ifd( &sds_master->Master_vol_slidemode	, SLIDE	, "volume slide mode");
	Push_ifd( &sds_master->Master_vol_slide_value	, time	, "volume slide time%");
}

bool Interpreter_class::Exit()
{
	Push_sys( CMD_EXIT,"exit" );
	CompilerExit = true;
	return true; // stop compiler
}


bool Interpreter_class::check_count( string_vec_t arr, size_t min_size )
{
	if ( arr.size()  < min_size  )
	{
		Comment(ERROR, "wrong parameter count: expected >= " + to_string(min_size) + " got " + to_string(arr.size()));
		error = 2;
		return false;
	}
	return true;
}

template< typename Arg>
void Interpreter_class::wrong_argument( uset_t expected , Arg given )
{
	error = 1;
	Comment( WARN, "unhandled argument: ", given );
	Comment( INFO, "expected: "  );
	for ( string expect : expected )
		cout << expect << endl;
	if ( dialog_mode ) return;
	If_Exception( "processing failed" );
}

void Interpreter_class::Wrong_keyword( uset_t expected , string given )
{
	error = 1;
	string expected_str = show_type( expected );
	Comment( WARN, "unhandled argument: ", given );
	Comment( INFO, "expected          : ", expected_str  );
	if ( dialog_mode )
		return;
	If_Exception( "processing failed" );
}


bool Interpreter_class::set_stack( string_vec_t arr, uint min )
{
	cmdline = arr[0] + " ";
	if ( not check_count( arr, min ) ) error = min;
	if ( no_error( error ) )
	{
		arr.erase( arr.begin() );
		this->stack = arr;
		return true;
	}
	return false;
}

bool Interpreter_class::Check_input( string keyword )
{
	if( Keywords.contains( keyword ) )
		return true;
	return false;
}

void Interpreter_class::Set_dialog_mode( bool mode )
{
	dialog_mode = mode;
}

void Interpreter_class::If_Exception( string str )
{
	if( dialog_mode )
	{
		Comment( ERROR, "Exception: ", str );
		error = 0;

		return;
	}
	error = 1;
	Exception( command + ": " + str, SIGILL );
}
bool Interpreter_class::no_error( int nr )
{
	if ( error > 0 )
	{
		show_expected( );
		if ( nr > 1 ) cout << " more ..." ;
		cout << endl;
		if ( not dialog_mode )
			If_Exception( "unexpected" );//raise( SIGINT );
		return false;
	}
	return true;

}

string Interpreter_class::pop_stack( int required )
{
	auto get_stack = [ this ]()
		{	string tmp = this->stack[0];
			cmdline.append(tmp);
			cmdline.append(" ");
			this->stack.erase( this->stack.begin() );
			return tmp;
		};

	string tmp = "";
	error = 0;
	if ( required == 0 ) 				// nothing required
	{
		if ( this->stack.size() > 0 ) 	// but something is there
		{
			return get_stack();			// that should be used
		}
		Comment( INFO, "using option default " + option_default );
		tmp = option_default;
		option_default = "";
		return tmp;
	}

	option_default = "";
	if ( not check_count( this->stack, 1 ) )
	{
		error = required;
	}
	if ( no_error( error ) )
	{
		tmp = get_stack();
	}
	return tmp;
}

void Interpreter_class::show_expected(  )
{
	if ( expect.size() > 0 )
	{
		cout << cmdline << " missing: [" + *expect.begin();
		expect.erase( expect.begin() );
		for( string word : expect )
		{
			string print = word;
			cout.flush() << "|" << print  ;
		}
		cout << "]" ;
	}
}

bool Interpreter_class::is_regid( string str )
{
	if( str[0] == '$' )
	{
		const char 	ch	= str[1];
		const int 	id	= char2int( ch );
		return Set_regid( ( regid_t )id );
	}
	else
		return false;
};


template <typename T >
T Interpreter_class::pop_T( range_T<T> range )
{
	String 	Str	{};
	if ( option_default.length() == 0 )
		Str.Str = pop_stack( 1 );
	else
		Str.Str = pop_stack( 0 );

	if ( Str.is_number() )
	{
		T stack_value = (T)Str.secure_stoi();
		if ( not in_range( range, stack_value ) )
		{
			Comment( ERROR, "Value "+ Str.Str + " out of bounds" );
			Info( range_str( range ) );
			if( not LogMask[ TEST ])
				If_Exception( "unexpected, see above" );
		}
		return stack_value;
	}
	else
	{
		if( is_regid( Str.Str ) )
			return range.min; 	// $x-case: in this case the value will be overwritten by
								// the content of Assembler register_arr Regid
		If_Exception( Str.Str + " is not a number" );
	}
	return range.min;
}

string Interpreter_class::pop_set( uset_t Set )
{
	string 	str	{};
	int count = (int)( option_default.length() == 0 ); // count 1 or 0
	str = pop_stack( count );

	if( is_regid( str ) )
		return str; 	// $x-case: in this case the value will be overwritten by
							// the content of Assembler register_arr Regid
	if ( Set.contains( str ) )
		return str;

	If_Exception( "expected" + show_type( Set ) );
	return "";
}

void Interpreter_class::check_file( string_vec_t dirs, string name )
{
	for ( string dir : dirs )
	{
		if ( filesystem::exists( dir + name ) ) return;
	};
	dircontent_t notes			= dircontent_struct( dirs, fs->nte_type );
	coutf << show_type( notes.dir_entry_vec ) << endl;
	if( not notes.Is_empty() ) return;

	dircontent_t instruments	= dircontent_struct( dirs, fs->snd_type );
	coutf << show_type( instruments.dir_entry_vec ) << endl;
	if( not instruments.Is_empty() ) return;

	If_Exception( "no such file " + name );
}


void Interpreter_class::Test(  )
{
	TEST_START( className );
	Comment( TEST, "Interpreter_class test");
	testrun = true;

	Notes( { "notes", "set", "ABCD EFGF", "8" } );
	Notes( { "notes", "set", "prefix", "3", "2" } );
	Notes( { "notes", "set", "octave", "5" } );
	Notes( { "notes", "save", fs->tmpnote_filename } );
	Variation.Read( fs->tmpnote_filename );
	Assert_equal( Variation.Noteline.get(), string("ABCD EFGF") );
	Assert_equal( Variation.Noteline_prefix.data.convention , uint8_t( 0 ) );

	Notes( {"notes", "set", "prefix", "3", "2" } );
	Variation.Read( fs->tmpnote_filename );
	assert( Variation.Noteline_prefix.data.flat == 3 );
	assert( Variation.Noteline_prefix.data.convention == 0 );
	assert( Variation.Noteline.get().compare("ABCD EFGF") == 0 );

	Notes( {"notes", "set", "octave", "5" }) ;
	Variation.Read( fs->tmpnote_filename );
	assert( Variation.Noteline_prefix.data.Octave == 5 );



	range_T<uint8_t> test_range{0,10};

	string str = "1,2,3,4,5,10,11,-1,500";
	String Str = str;
	string_vec_t arr = Str.to_array( ',' );
	set_stack( arr, 5 );
	arr.erase( arr.begin() );
	string_vec_t test_stack = arr;

	for( string val : test_stack )
	{
		pop_T( test_range );

//		uint8_t value = pop_T( test_range );
//		Assert_equal( (int)value, stoi(val) );
	}

	Interpreter_ns::line_data_t row = line_struct( 1, "z = x + iy" );
	Assert_equal( int(row.key), int(Interpreter_ns::CONCAT) );

	row = line_struct( 2, "A1 = c2 # comment" );
	Assert_equal( int(row.key), int(Interpreter_ns::ASSIGN ) );

	row = line_struct( 3, "zab = xcd +iyed" );
	Assert_equal( int(row.key), int(Interpreter_ns::ASSIGN ) );

	row = line_struct( 4, "zab = xcd + iyed  		# comment" );
	Assert_equal( int(row.key), int(Interpreter_ns::CONCAT ) );

	row = line_struct( 5, "zab1 = xcd + iy ed" );
	Assert_equal( int(row.key), int(Interpreter_ns::CONCAT ) );



	TEST_END( className );

}



/**************************************************
 * assignment_struct
 *************************************************/

assignment_struct::assignment_struct()
{
	Test();
}

void assignment_struct::Add( string var, string val )
{
	assignment_vec.push_back( { var, val } );
}

void assignment_struct::Clear()
{
	assignment_vec.clear();
}

string_vec_t assignment_struct::ReplaceVariable( string_vec_t arr )
{
	if( assignment_vec.size() == 0 )
		return arr;		// nothing that could be done

	for ( var_struct_t replace : assignment_vec )
	{
		// do not change the keyword ( start with + 1 )
		std::ranges::for_each( arr.begin() + 1, arr.end(), [ replace ](string& word )
		{
			if ( strEqual( word, replace.name ) )
			{
				word = replace.value;
			}
		}
		); // for_each
	}
	return arr;
}

void assignment_struct::Show_const()
{

	Table_class T {};
	T.AddColumn( "Name", 20 );
	T.AddColumn( "Value", 15 );
	T.Info( "Using contant values as follows:" );
	T.PrintHeader();

	for( var_struct_t var : assignment_vec )
		T.AddRow( var.name, var.value );
}

void assignment_struct::RemoveVar( string varname )
{
	auto find_idx = [ this, varname ](  )
	{
		size_t idx = 0;
		for( var_struct_t var : assignment_vec )
		{
			if( strEqual( var.name, varname ))
				return idx;
			idx++;
		}
		return idx;
	};

	size_t idx = find_idx();
	if( idx < assignment_vec.size() )
		assignment_vec.erase(assignment_vec.begin() + idx );
}

void assignment_struct::Test()
{
	string_vec_t t_arr = { "This", "is", "test", "data" };
	string_vec_t v_arr = { "test" , "=", "4" };
	Clear();

	t_arr = ReplaceVariable( t_arr  ); //check emty varlist;
	assert( t_arr[0].compare("This") == 0);
	Add( v_arr[0], v_arr[2] );
	t_arr = ReplaceVariable( t_arr );

	assert( t_arr[2].compare("4") == 0 );

	Add( "This", "5" );
	t_arr = ReplaceVariable( t_arr );
	Assert_equal( t_arr[0], string("This"), "" );
	Add( "data", "6" );
	t_arr = ReplaceVariable( t_arr );
	Assert_equal( t_arr[3], string("6") );
	Clear();

}
