//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * Interpreter.cpp
 *
 *  Created on: Apr 11, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <Composer/Interpreter.h>

range_T<uint8_t> amp_range{ 0, 100 };
range_T<uint8_t> uint8_range{ 0, 255 };

Interpreter_class::Interpreter_class( Application_class* app )
	: Logfacility_class( "Interpreter_class" )
	, Processor_class( app )
	, Device_class( app->DaTA->sds_master )
	, Variation( app->DaTA->sds_master, app->Cfg )
{
	className			= Logfacility_class::className;
	this->sds 			= app->DaTA->GetSdsAddr();
	this->Sds 			= app->DaTA->Sds_master;
	this->Cfg 			= app->DaTA->Cfg_p;
	this->fs			= app->DaTA->Cfg_p->fs;

	main_view.name		= "Main osc";
	main_view.oscid		= OSCID;
	main_view.wfkey 	= SETWAVEFORMMAINKEY;
	main_view.ampkey 	= SETSTA_KEY;//MASTERAMP_KEY;
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

	main_view.wf 		= &sds->spectrum_arr[OSCID].wfid[0];
	main_view.amp 		= &sds->StA_amp_arr[STA_INSTRUMENT];
	main_view.frqidx 	= &sds->spectrum_arr[OSCID].frqidx[0];

	vco_view.wf 		= &sds->spectrum_arr[VCOID].wfid[0];
	vco_view.amp 		= &sds->spectrum_arr[VCOID].volidx[0];
	vco_view.frqidx 	= &sds->spectrum_arr[VCOID].frqidx[0];

	fmo_view.wf 		= &sds->spectrum_arr[FMOID].wfid[0];
	fmo_view.amp 		= &sds->spectrum_arr[FMOID].volidx[0];
	fmo_view.frqidx 	= &sds->spectrum_arr[FMOID].frqidx[0];

}

Interpreter_class::~Interpreter_class()
{
	DESTRUCTOR( className )
}

bool Interpreter_class::cmpkeyword ( const string& word )
{
	String lower {keyword.Str};
	lower.to_lower();
	return ( lower.Str.compare( word ) == 0 );
}

void Interpreter_class::Start_bin( vector_str_t arr )
{
	string cmd{};
	string exe{};
	string opt{};

	expect = { "AudioServer", "Synthesizer", "Rtsp" };
	intro( arr , 2 );

	if ( cmpkeyword( "rtsp" ) )
	{
		exe = fs->rtsp_bin;
		opt = "-C";
	}
	if ( cmpkeyword( "synthesizer") )
	{
		exe = fs->Synth_bin;
	}
	if ( cmpkeyword( "audioserver") )
	{
		exe = fs->Audio_bin;
	}
	if( exe.length() > 0 )
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
		Comment( INFO, "start " + keyword.Str );

		cmd = Cfg->Server_cmd( Cfg->Config.Nohup, exe, opt );
		Processor_class::Push_cmd( CMD_EXE, cmd );

		return;
	}

	Wrong_keyword(  expect , keyword.Str );
	return ;
}

void Interpreter_class::Stop_bin( vector_str_t arr )
{
	expect = { "AudioServer", "Synthesizer" };
	intro( arr, 1 );

	if ( strEqual( keyword.Str, "Synthesizer" ) )
	{
		Comment( INFO, "terminating " + keyword.Str );
		Processor_class::Push_ifd( &sds->appstate_arr[SYNTHID].state, EXITSERVER, "terminating " + keyword.Str );
		Processor_class::Push_key( EXITKEY, "terminating key" + keyword.Str );
		return;
	}
	if ( strEqual( keyword.Str, "AudioServer" ) )
	{
		Comment( INFO, "terminating " + keyword.Str );
		Processor_class::Push_ifd( &sds->appstate_arr[AUDIOID].state, EXITSERVER, "terminating " + keyword.Str  );
		return;
	}

	Wrong_keyword( { "AudioServer, Synthesizer" }, arr[1] );

}

void Interpreter_class::intro( vector_str_t arr, uint min )
{
	auto arr_to_str = [ arr ]()
	{
		string ret = "   > ";
		return ret + show_type( arr );
	};

	error 			= 0;
	duration 		= 0;
	set_stack( arr, min );
	keyword.Str 	= pop_stack( 1 );
	command 		= arr_to_str();
	Processor_class::Push_text( command );

}

void Interpreter_class::RecFile( vector_str_t arr )
{
	expect = { "stop", "file", "play", "loop" };
	intro( arr, 2);

	if ( cmpkeyword( "file" ) )
	{
		expect = { "Record duration in seconds" };
		option_default = "0";
		string duration = pop_stack( 0 );
		Processor_class::Push_ifd( &sds->appstate_arr[AUDIOID].state, RECORDING, "recording" );
		Processor_class::Push_key( SAVE_EXTERNALWAVFILEKEY, 	"start record" );
		Pause( {"pause", duration } );

		return;
	}
	if ( cmpkeyword( "stop") ) // stop record and write file with synthesizer.record_thead_fcn
	{
		expect = { "File number" };
		uint8_t FileNo = pop_T( uint8_range );
		Processor_class::Push_ifd( &sds->appstate_arr[AUDIOID].state, RECORDSTOP,"record stop" );
		Processor_class::Push_ifd( &sds->FileNo, FileNo, 		string("record file")  ); // trigger record_thead_fcn
		Processor_class::Push_key( SAVE_EXTERNALWAVFILEKEY, 	"stop record" );
		return;
	}
	if ( cmpkeyword( "play") )
	{
		expect 			= { "file no", "replay duration in seconds" };
		string wavfile 	= "synthesizer" + pop_stack( 2 );
		string duration	= pop_stack(1);
		Processor_class::Push_str( READ_EXTERNAL_WAVFILE, OTHERSTR_KEY, wavfile );
		Pause( {"pause", duration } );
		return;
	}
	if ( cmpkeyword( "loop") )
	{
		expect = {"amp"};
		keyword.Str = pop_stack( 1);
		if ( cmpkeyword( "amp") )
		{
			expect = { "loop final volume" };
			uint8_t 	amp 	= pop_T( amp_range );
			Loop( amp, MASTERAMP_KEY ); // TODO - working
			return;
		}
		Wrong_keyword( expect , keyword.Str );
	}

	Wrong_keyword( expect , keyword.Str );

}

void Interpreter_class::Random( vector_str_t arr )
{
	// generate random notes
	// random |2C-F- CFFDH- cvcv auto


	expect = { "constant note list"};
	intro( arr, 6 );

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

	expect = { "notelist name " };
	string filename = pop_stack( 1);

	string random_noteline = Variation.Gen_noteline( Pattern , filename );
	cout << random_noteline << endl;
}

void Interpreter_class::Set( vector_str_t arr )
{
	expect = { "octave+", "octave-", "main", "vco", "fmo" };
	intro( arr, 2 );
	if ( cmpkeyword( "octave+" ))
	{
		Processor_class::Push_ifd( &sds->FLAG, 1_uint, "octave" );
		Processor_class::Push_key(SETBASEOCTAVE_KEY, "inc octave");
		return;
	}
	if ( cmpkeyword( "octave-" ))
	{
		Processor_class::Push_ifd( &sds->FLAG, 0_uint, "octave" );
		Processor_class::Push_key(SETBASEOCTAVE_KEY, "dec octave");
		return;
	}
	if ( cmpkeyword( "main" ) or cmpkeyword( "vco" ) or cmpkeyword( "fmo" ))
	{
		string osc = keyword.Str;
		expect = { "freq", "wf", "amp" };
		keyword.Str = pop_stack( 2 );

		if( cmpkeyword( "freq") )
		{
			string value{ pop_stack( 1 ) };
			Osc({ "osc", osc, "freq", value });
			return;
		}
		if( cmpkeyword( "wf") )
		{
			Value value{ pop_T( wfid_range ) };
			Osc({ "osc", osc, "wf", value.str });
			return;
		}
		if( cmpkeyword( "amp") )
		{
			uint8_t 	amp 	= pop_T( amp_range );
			Osc({ "osc", osc, "amp", to_string( amp ) });
			return;
		}
		Wrong_keyword( expect , keyword.Str );

	}
	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::Notes( vector_str_t arr )
{
	expect = { "load", "play" , "set", "per_second"};
	intro( arr, 2 );

	if ( cmpkeyword( "load") )
	{
		expect = { "Notes name" };
		string notes_name = pop_stack( 1);
		Comment( INFO, "loading notes " + notes_name );
		check_file( { 	fs->autodir,
						fs->notesdir }, notes_name + fs->nte_type );

		Processor_class::Push_str( UPDATENOTESKEY, NOTESSTR_KEY, notes_name );
		return;
	}
	if ( cmpkeyword( "set") )
	{
		noteline_prefix_t nl = Variation.Noteline_prefix;
		Variation.Read("tmp");
		Variation.Noteline_prefix = nl;

		string Noteline = Variation.Get_note_line();

		Comment(INFO, "set notes.");
		expect		=	{"prefix","octave", "[num]","sentence" };
		keyword.Str = pop_stack(1);

		if ( cmpkeyword("prefix") )
		{
			expect 		= {"#flats", "#sharps" };
			uint8_t flats	= pop_T( accidental_range );
			uint8_t sharps	= pop_T( accidental_range );
			Processor_class::Push_ifd( &sds->noteline_prefix.flat ,flats, "update flats" );
			Processor_class::Push_ifd( &sds->noteline_prefix.sharp ,sharps, "update sharps" );
			Processor_class::Push_key( UPDATE_NLP_KEY, "commit");
			Variation.Set_note_chars( flats, sharps );
			Variation.Save( "tmp", Variation.Noteline_prefix, Noteline );
			return;
		}
		if( cmpkeyword("octave") )
		{
			expect = { "octave value" };
			uint8_t oct = pop_T( octave_range ) ;
			Variation.Set_prefix_octave( oct );
			Processor_class::Push_ifd( &sds->noteline_prefix.Octave ,oct, "update octave" );
			Processor_class::Push_key( UPDATE_NLP_KEY, "commit");

			Variation.Save( "tmp", Variation.Noteline_prefix, Noteline );
			return;
		}

		if ( cmpkeyword( "num" ) )
		{
			Noteline = pop_stack(1);
			Variation.Set_note_chars( 1 );
		}
		else
		{
			Noteline = keyword.Str;
			if ( Variation.Noteline_prefix.convention == 0 )
				Variation.Set_note_chars( 0 );

		}
		cout << Noteline << endl;
		expect = {"rhythm line" };
		string Rhythmline = pop_stack(1);
		Variation.Set_rhythm_line( Rhythmline );
		Variation.Save( "tmp", Variation.Noteline_prefix, Noteline );

		Processor_class::Push_str( UPDATENOTESKEY, NOTESSTR_KEY, "tmp" );

		return;
	}
	// notes play notes ACDC
	if ( cmpkeyword( "play") )
	{
		Comment( INFO, "playing notes " );
		expect = { "on", "off" };
		keyword.Str = pop_stack( 1);
		if ( cmpkeyword( "on") )
		{
			expect = { "volume [%]" };
			uint8_t 	amp 	= pop_T( amp_range );
			Processor_class::Push_ifd( &sds->StA_amp_arr[STA_NOTES] ,amp, "play notes" );
			Processor_class::Push_key( NOTESONKEY, "commit");
			if ( stack.size() > 0  )
			{
				keyword.Str  = stack.back();
				Pause( {"pause", keyword.Str } );
			}
			return;
		}
		if ( cmpkeyword( "off") )
		{
			Comment( INFO, "playing notes off " );
			Processor_class::Push_key( NOTESOFFKEY, "notes off" );
			return;
		}
		Wrong_keyword( expect, keyword.Str );
		return;
	}
	if ( cmpkeyword( "per_second" ) )
	{
		expect = {"notes per second [ " + Variation.NPS_string +  " ]"};
		Value nps = pop_T( npsidx_range );
		if ( Variation.Set_notes_per_second( nps.val ) )
		{
			Processor_class::Push_ifd( &sds->noteline_prefix.nps, (uint8_t)nps.val, "notes per second"  );
			Processor_class::Push_key( SETNOTESPERSEC_KEY, "set per_second" );
		}
		else
		{
			Wrong_keyword(expect, nps.str );
		}
		cout << "error: " << error << endl;
		return;
	}
	Wrong_keyword( expect, keyword.Str );
}


void Interpreter_class::Instrument( vector_str_t arr )
{
	expect = { "load" };
	intro( arr, 2 );

	if ( cmpkeyword( "load") )
	{
		expect = { "instument name "};
		string instr = pop_stack( 1);
		Comment( INFO, "loading instrument " + instr );
		check_file( { fs->instrumentdir } , instr + fs->snd_type );

//		Push_text( command );
		Processor_class::Push_str( SETINSTRUMENTKEY, INSTRUMENTSTR_KEY, instr );
//		Processor_class::Push_key( SETINSTRUMENTKEY, "set instrument" );
		return;
	}
	Wrong_keyword(  expect , keyword.Str );
;
}


void Interpreter_class::Add( vector_str_t arr )
{
	expect = {"add", "instrument", "notes", "storage id", "duration|auto "};
	intro( arr, 4 );

	string inst		 	= keyword.Str;
	string note 		= pop_stack( 1);
	string amp 			= "0";
	string ma_id 		= pop_stack( 1 );

	vector_str_t vec;

	vec = { "instrument" , "load" , inst }; 	// SETINSTRUMENTKEY
	Instrument( vec );
	vec = {"notes" , "load" , note };			// UPDATENOTESKEY
	Notes( vec );
//	vec = { "rec", "notes", ma_id };	//PLAYNOTESREC_ON_KEY
//	RecStA( vec );
}

void Interpreter_class::Osc( vector_str_t arr )
{	// osc [main|vco|fmo] set waveid frequency amplitude duration

	expect = { "main", "vco", "fmo" };
	vector_str_t tmp = arr;
	intro( arr, 3);
	view_struct_t view = view_struct();
	if ( cmpkeyword( "main" ))
	{
		view = main_view;
	}
	if ( cmpkeyword( "fmo") )
	{
		view = fmo_view;
	}
	if ( cmpkeyword( "vco") )
	{
		view = vco_view;
	}
	if ( view.name.compare("none") == 0 )
	{
		Wrong_keyword( expect, keyword.Str );
		return;
	}

	keyword.Str = arr[2];
	if ( cmpkeyword( "set") )
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
		osc_view( view, arr );
	}

}

void Interpreter_class::osc_view( view_struct_t view, vector_str_t arr )
{
	expect = { "mute", "unmute", "reset", "freq", "loop", "wf", "amp" };
	intro( arr, 2);

	bool loop = false;

	if ( cmpkeyword( "mute") )
	{
		Comment( INFO, "Master volume is muted " );

		Push_ifd( &sds->mixer_state.mute, false, "false" );
		Push_key( MASTERAMP_MUTE_KEY, "mute master volume" );
		return;
	}

	if ( cmpkeyword( "unmute") )
	{
		Comment( INFO, "Master volume is un-muted " );

		Push_ifd( &sds->mixer_state.mute, true, "true" );
		Push_key( MASTERAMP_MUTE_KEY, "un-mute master volume" );
		return;
	}
	if ( cmpkeyword( "reset" ))
	{
		Comment( INFO, "Reset connections");
		Processor_class::Push_ifd( &sds->connect_arr[OSCID].frq, OSCID, "reset fmo connect" );
		Processor_class::Push_ifd( &sds->connect_arr[OSCID].vol, OSCID, "reset vco connect" );
		Processor_class::Push_key( CONNECTOSC_KEY , "reset main"  );
		return;
	}


	if ( cmpkeyword( "wf") )
	{
		expect = vector2set( Spectrum.Get_waveform_vec() );
		string waveform = pop_stack( 1);

		uint8_t wfid = Spectrum.Get_waveform_id( waveform );
		if ( wfid < 0 )
		{
			Wrong_keyword(expect, waveform);
			Exception( "wrong keyword" );
		}
		expect 		= { " duration in seconds" };
		option_default = "0";
		string duration = pop_stack(0);
		Comment( INFO, "Set waveform " + waveform + " for " + view.name );

		Processor_class::Push_ifd( view.wf, wfid, "wafeform");
		Processor_class::Push_key( view.wfkey, "set waveform" );
		Pause( { "pause", duration });
		return;
	}

	if ( cmpkeyword( "loop") )
	{
		keyword.Str = pop_stack( 1);
		loop = true;
		// TODO
	}

	if ( cmpkeyword( "amp") )
	{
		expect = { "volume [%]" };
		uint8_t 	amp 	= pop_T( amp_range );
		Comment( INFO, "Set amplitude " + to_string(amp) + " for " + view.name );
		Processor_class::Push_ifd( &sds->connect_arr[OSCID].vol , VCOID, "connect vol->osc" );

		if ( loop ) // TODO-working
		{
			Loop( amp, MASTERAMP_LOOP_KEY );
		}
		else
		{
			expect 		= { " duration in seconds" };
			option_default = "0";
			string duration = pop_stack(0 );
			Processor_class::Push_ifd( view.amp, amp, "volume"  );
			Processor_class::Push_key( view.ampkey, "set volume" );
			Pause( { "pause", duration });
 		}
		return;
	}

	if ( cmpkeyword( "freq") )
	{
		expect = { "frequency name or index" };
		uint8_t freq = 1;
		string f { pop_stack(1) };
		freq = Frequency.Index( f );

		Comment( INFO, "Set frequency " + to_string(freq) + " for " + view.name );
		Processor_class::Push_ifd( &sds->connect_arr[OSCID].frq , FMOID, "connect frq->osc" );

		if ( loop )
		{
			Processor_class::Push_ifd( &sds->frq_slidermode, SLIDE, "slide mode" );
			Processor_class::Push_ifd( &sds->features[OSCID].glide_effect , 100_uint, "long frq slide" );
			Processor_class::Push_key(  SOFTFREQUENCYKEY, "set index"  );
		}
			expect 		= { " duration in seconds" };
			option_default = "0";
			string duration = pop_stack(0 );
			Processor_class::Push_ifd(  view.frqidx, freq, "frq index"  );
			Processor_class::Push_ifd( &sds->features[OSCID].glide_effect , 0_uint, "frq slide off" );

			Processor_class::Push_key(  view.freqkey, "set frequency"  );
			Pause( { "pause", duration });

		return;
	}


	Wrong_keyword( expect, keyword.Str );
}

void Interpreter_class::Play( vector_str_t arr )
{ 	// play 1 amp 30
	// play 1 amp loop 30

	expect 			= { "storage id "};
	if( not set_stack( arr, 3 ) ) return;
	STAID_e staId 		= pop_T( staid_range );

	expect 			= { "amp" };
	string k1 		= pop_stack( 1); // amp always
	keyword.Str 	= k1;
	if ( cmpkeyword( "amp") )
	{
		expect = { "loop", "duration in sec." };
		string k2 = pop_stack( 1); // loop or duration
		keyword.Str = k2;
		if ( cmpkeyword( "loop") )
		{
			expect = { " dest amp"};
			uint8_t max = pop_T( percent_range );
			Processor_class::Push_ifd( &sds->MIX_Id , staId, "mixer id" );
			Processor_class::Push_ifd( &sds->StA_amp_arr[staId] , max, "% slide duration " );
			Processor_class::Push_ifd( &sds->vol_slidemode , SLIDE, "slide mode" );
			Processor_class::Push_key( EXTERNAL_AMPLOOP_KEY	, "set loop volume" );
			Loop( max, NULLKEY);
			return;
		}
		else
		{
			option_default = "10";
			string duration 	= k2;
			RecStA( {"rec", "amp", to_string( staId ), duration } );
			return;
		}
	}
	Wrong_keyword( expect , k1);

}

void Interpreter_class::RecStA( vector_str_t arr )
{
	expect = { "store", "amp", "stop", "play", "mute" , "clear", "loop"};
	intro( arr, 3 );

	if ( cmpkeyword( "loop" ))
	{
		STAID_e	staid	= pop_T( staid_range );
		uint8_t end = pop_T( percent_range );

		Processor_class::Push_ifd( &sds->MIX_Id , staid, "mixer id" );
		Processor_class::Push_ifd( &sds->StA_amp_arr[staid] , end, "% slide duration " );
		Processor_class::Push_ifd( &sds->vol_slidemode , SLIDE, "slide mode" );
		Processor_class::Push_key( EXTERNAL_AMPLOOP_KEY	, "set loop volume" );

		return;
	}
	if ( cmpkeyword( "amp") )
	{
		STAID_e	staid	= pop_T( staid_range );
		expect 		= { "volume [%]" };
		uint8_t 	amp 	= pop_T( amp_range );
		Comment( INFO, "set amplitude of " + to_string(staid) + " to " + to_string(amp) + "%" );

		Processor_class::Push_ifd( &sds->MIX_Id , staid, "mixer id" );
		Processor_class::Push_ifd( &sds->StA_amp_arr[ staid ], amp, "mixer volume" );
		Processor_class::Push_ifd( &sds->StA_state_arr[staid].play, true, "true" );
		Processor_class::Push_key( SETSTA_KEY	, "set StAs" );

		Processor_class::Push_key( RESET_STA_SCANNER_KEY	, "reset StAscanner" );

		return;
	}

	// e.g. rec play notes|free
	if ( cmpkeyword( "play") )
	{
		expect = {"free"};
		keyword.Str = pop_stack( 1);
		if ( cmpkeyword( "free") )
		{
			Processor_class::Push_key( PLAYNOTESRECOFF_KEY, "free mode" );
			return;
		}
		Wrong_keyword( expect , keyword.Str );

	}
	if ( cmpkeyword( "notes") )
	{

		expect 			= {"mem id 0..5"};
		STAID_e	staid	= pop_T( staid_range );
		Processor_class::Push_ifd( &sds->MIX_Id, staid, "mixer id");
		Processor_class::Push_key( PLAYNOTESREC_ON_KEY, "notes on");
		return;
	}

	if ( cmpkeyword( "mute") )
	{
		STAID_e	staid	= pop_T( staid_range );
		Comment( INFO, "mute memory array: " + to_string(staid) );
		Processor_class::Push_ifd( &sds->MIX_Id, staid, "sound" );
		Processor_class::Push_key(MUTEREC_KEY,  "stop sound" );
		return;
	}

	if ( cmpkeyword( "store") )
	{
		STAID_e id	= pop_T( staid_range );
		Comment( INFO, "store sound to: " + to_string(id) );
		Processor_class::Push_ifd( &sds->MIX_Id, id, "sound" );
		Processor_class::Push_key( STORESOUNDKEY, "store sound" );
		return;
	}

	if ( cmpkeyword( "clear") )
	{
		STAID_e	staid	= pop_T( staid_range );
		Comment( INFO, "clear " + to_string(staid) );
		Processor_class::Push_ifd( &sds->MIX_Id, staid, "mixer id" );
		Processor_class::Push_key( CLEAR_KEY,  "set clear" );
		return;

	}

	if ( cmpkeyword( "stop") )
	{
		STAID_e	staid	= pop_T( staid_range );
		Comment( INFO, "stop recording to: " + to_string(staid) );
		Processor_class::Push_ifd( &sds->MIX_Id, staid, "sound" );
		Processor_class::Push_key( STOP_STARECORD_KEY, "stop sound" );
		return;
	}


	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::Text( vector_str_t arr )
{
	expect = { "text" };
	intro( arr, 1 );

	string text = keyword.Str ;
	for ( string str : stack )
	{
		text.append(" ");
		text.append(str);
	}

	Processor_class::Push_text( text );
}

void Interpreter_class::Adsr( vector_str_t arr )
{
	expect =  { "pmw", "hall", "attack", "decay", "softfreq", "beat" };
	intro( arr, 2 );

	if ( cmpkeyword( "softfreq") )
	{
		Comment( INFO, "soft frequency is set to: " + stack[0] );
		uint8_t freq = pop_T( percent_range );
		Processor_class::Push_ifd( &sds->features[OSCID].glide_effect, freq, "soft freq"  );
		Processor_class::Push_key( SOFTFREQUENCYKEY,  "set soft freq" );
		return;
	}
	if ( cmpkeyword( "beat") )
	{
		Comment( INFO, "beat duration is set to: " + stack[0] );
		expect 		= bps_struct().Bps_str_set;
		string 	Bps = pop_stack(1 );
		uint8_t	bps	= char2int( Bps[0]);
		if ( not bps_struct().Bps_set.contains( bps ) )
		{
			Wrong_keyword( expect , Bps );
			Exception( "wrong beat duration" );
		}

		Processor_class::Push_ifd( &sds->adsr_arr[OSCID].bps, bps, "beat duration" );
		Processor_class::Push_key( ADSR_KEY, "set beat duration" );
		return;
	}
	if ( cmpkeyword( "attack") )
	{
		Comment( INFO, "beat attack is set to: " + stack[0] );
		uint8_t attack = pop_T( percent_range );
		Processor_class::Push_ifd( &sds->adsr_arr[OSCID].attack, attack, "adsr attack" );
		Processor_class::Push_key( ADSR_KEY, "set adsr attack" );
		return;
	}
	if ( cmpkeyword( "decay") )
	{
		Comment( INFO, "beat decay is set to: " + stack[0] );
		uint8_t decay = pop_T( percent_range );
		Processor_class::Push_ifd( &sds->adsr_arr[OSCID].decay, decay, "adsr decay" );
		Processor_class::Push_key( ADSR_KEY, "set adsr decay" );
		return;
	}
	if ( cmpkeyword( "hall") )
	{
		Comment( INFO, "hall effect is set to: " + stack[0] );
		uint8_t hall = pop_T( percent_range );
		Processor_class::Push_ifd( &sds->adsr_arr[OSCID].hall, hall, "hall"  );
		Processor_class::Push_key( ADSR_KEY,  "set hall" );
		return;
	}
	if ( cmpkeyword( "pmw") )
	{
		Comment( INFO, "PMW is set to: " + stack[0] );
		uint8_t dial = pop_T( percent_range );
		Processor_class::Push_ifd( &sds->features[VCOID].PMW_dial, dial, "pmw" );
		Processor_class::Push_key( PWMDIALKEY, "set pmw" );
		return;
	}

	Wrong_keyword(expect, keyword.Str );

}

void Interpreter_class::Pause( vector_str_t arr )
{

	expect 		= { "duration in seconds", "auto", "key" };
	intro( arr, 1 );

	if ( cmpkeyword( "key" ))
	{
		Processor_class::Push_wait( CMD_COND_WAIT, 1, "press <ret> to continue" );
		return;
	};
	if ( cmpkeyword( "auto" ) )
	{
		Processor_class::Push_wait( CMD_COND_WAIT, 0, "auto" );
		return;
	}
	if ( keyword.is_number(  ) )
	{
		duration = stoi( keyword.Str );
		Processor_class::Push_wait( CMD_WAIT, duration, "[sec]" );
		return;
	}
	Wrong_keyword(expect, keyword.Str);
}

//vector_str_t Interpreter_class::include( vector_str_t arr, vector_str_t& filelines )

void Interpreter_class::Addvariable( vector_str_t arr )
{
	auto key =  [] ( vector_str_t a )
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
	expect = {"expected <var name> = <var int value>"};
	string varname 	= key(arr); // first string needed

	if ( Keywords.contains( varname ) )
	{
		if ( not testrun)
			Exception( "var " + varname + " is keyword" );//raise( SIGINT);
		testreturn = true;
	};

	set_stack( arr, 2);
	keyword	= pop_stack( 1);
	if ( not cmpkeyword( "=" ))
	{
		Exception( "missing equal sign");
	}
	string varvalue 	= pop_stack( 1) ;
	if ( Keywords.contains( varvalue ) )
	{
		if ( not testrun)
			Exception( "varvalue " + varvalue + " is keyword"  );//raise( SIGINT);
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
			Comment(INFO, "expected +, Ignoring option: add file");
		}
	}
	else
	{
		varlist.push_back( { varname, varvalue} );

	}
}

vector_str_t Interpreter_class::InsertVariable( vector_str_t arr )
{
	error = 0;
	if( varlist.size() == 0 ) return arr;
	vector_str_t result{};
//	for_each(	varlist.begin(),
//				varlist.end(),
//				[]( var_struct_t pair ) { cout << pair.name << ":" << pair.value << endl;});
	result = arr;

	for ( var_struct_t pair : varlist )
	{
		arr = result;
		result.clear();
		for ( string word : arr )
		{


			if ( strEqual(word, pair.name) )
			{
				result.push_back( pair.value );
			}
			else
				result.push_back( word );
		}
	}
//	show_items(result);
	return result;
}

void Interpreter_class::Loop( uint8_t max, EVENTKEY_e key )
{
	if ( key == 0 )
	{
		Processor_class::Push_wait( CMD_WAIT, -1, "not yet implemented" );
		return;
	}
	Processor_class::Push_ifd( &sds->slide_duration , max	, "slide duration" );
	Processor_class::Push_ifd( &sds->vol_slidemode	, SLIDE	, "volume slide mode");
	Processor_class::Push_key( key,"set sliding volume" );
}

bool Interpreter_class::Exit()
{
	Processor_class::Push_cmd( CMD_EXIT,"exit" );
	CompilerExit = true;
	return true; // stop compiler
}

int Interpreter_class::Find_position( const program_vec_t& program, vector_str_t arr )
{ // find the line number in filelines starting with keyword

	uint	line_no 		= 0;
	string	call_arg		= arr[1];

	for( line_struct_t prgline : program )
	{
		if ( strEqual( call_arg, prgline.keyw) )
		{
			return line_no;
		}
		line_no++;
	}

	// not found
	std::for_each( program.begin(), program.end(),
			[] ( line_struct_t line )
			{ if ( line.keyw[0] == ':' ) cout << line.keyw << " " ;} );

	Exception( "line >" + arr[1] + " not found" );//raise( SIGINT );
 	return -2;
}

bool Interpreter_class::check_count( vector_str_t arr, size_t min_size )
{
	if ( arr.size()  < min_size  )
	{
		Comment(ERROR, "wrong parameter count: expected >= " + to_string(min_size) + " got " + to_string(arr.size()));
		error = 2;
		return false;
	}
	return true;
}
void Interpreter_class::Wrong_keyword( set<string> expected , string given )
{
	error = 1;
	Comment( WARN, "unhandled keyword: " + given );
	Comment( INFO, "expected: "  );
	for ( string expect : expected )
		cout << expect << endl;
	Exception( "processing failed" );
}


bool Interpreter_class::set_stack( vector_str_t arr, uint min )
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
	if( Keywords.contains( keyword ) ) return true;
	return false;
}

void Interpreter_class::Clear_stack()
{
	Clear_process_stack();
}

void Interpreter_class::Set_dialog_mode( bool mode )
{
	dialog_mode = mode;
}

bool Interpreter_class::no_error( int nr )
{
	if ( error > 0 )
	{
		show_expected( );
		if ( nr > 1 ) cout << " more ..." ;
		cout << endl;
		if ( not dialog_mode )
			Exception( "unexpected" );//raise( SIGINT );
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
			return get_stack();			// that should be used
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

template <typename T >
T Interpreter_class::pop_T( range_T<T> range )
{
	if ( option_default.size() == 0 )
		Str.Str = pop_stack( 1 );
	else
		Str.Str = pop_stack( 0 );

	if ( Str.is_number() )
	{
		T stack_value = (T)Str.secure_stoi(Str.Str);
		if ( not in_range( range, stack_value ) )
		{
			Comment( ERROR, "Value "+ Str.Str + " out of bounds" );
			Comment( INFO, "rejected: ", show_range( range ), ",  ", Str.Str);
			show_expected();
			if( not LogMask[ TEST ])
				Exception( "unexpected" );//raise(SIGINT);
		}
		return stack_value;
	}
	else
	{
		error = 1;
		Exception( Str.Str + " is not a number" );//raise(SIGINT);
	}
	return range.min;
	raise( SIGILL );
}

void Interpreter_class::check_file( vector_str_t dirs, string name )
{
	for ( string dir : dirs )
	{
		if ( filesystem::exists( dir + name ) ) return;
	};

	for ( string dir : dirs )
	{

		Comment( WARN, "list directory " + dir );

		vector<string>
		DirList = List_directory( dir, fs->nte_type );
		cout << show_type( DirList ) << endl;

		DirList = List_directory( dir, fs->snd_type );
		cout << show_type( DirList ) << endl;
	}
	Exception( "no such file " + name );//raise( SIGINT );
}


void Interpreter_class::Test(  )
{
	TEST_START( className );
	Comment( TEST, "Interpreter_class test");
	testrun = true;

	Notes( {"notes", "set", "ABCD EFGF", "8" } );
	Variation.Read("tmp" );
	assert( Variation.Get_note_line().compare("ABCD EFGF") == 0 );
	assert( Variation.Noteline_prefix.convention == 0 );

	Notes( {"notes", "set", "prefix", "3", "2" } );
	Variation.Read("tmp" );
	assert( Variation.Noteline_prefix.flat == 3 );
	assert( Variation.Noteline_prefix.convention == 2 );
	assert( Variation.Get_note_line().compare("ABCD EFGF") == 0 );

	Notes( {"notes", "set", "octave", "5" }) ;
	Variation.Read("tmp" );
	assert( Variation.Noteline_prefix.Octave == 5 );


	vector_str_t t_arr = { "This", "is", "test", "data" };
	vector_str_t v_arr = { "test" , "=", "4" };
	varlist.clear();

	t_arr = InsertVariable( t_arr ); //check emty varlist;
	assert( t_arr[0].compare("This") == 0);
	Addvariable( v_arr );
	t_arr = InsertVariable( t_arr );

	assert( t_arr[2].compare("4") == 0 );

	t_arr = { "call", "=" , "4" };

	testreturn = false;
	Addvariable( t_arr ); //check no keyword
	assert( testreturn );

	t_arr = { "#call", "=" , "exit" };

	testreturn = false;
	Addvariable( t_arr ); //check no keyword
	assert( testreturn );

	cout << show_type( List_directory( fs->instrumentdir, fs->snd_type ) ) << endl;
//	assert ( false );
	varlist.clear();

	range_T<uint8_t> test_range{0,10};

	string str = "1,2,3,4,5,10,11,-1,500";
	String Str = str;
	vector_str_t arr = Str.to_array( ',' );
	set_stack( arr, 5 );
	arr.erase( arr.begin() );
	vector_str_t test_stack = arr;

	for( string val : test_stack )
	{
		pop_T( test_range );

//		uint8_t value = pop_T( test_range );
//		Assert_equal( (int)value, stoi(val) );
	}
	TEST_END( className );

}
