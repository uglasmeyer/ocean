/*
 * Interpreter.cpp
 *
 *  Created on: Apr 11, 2024
 *      Author: sirius
 */

#include "Interpreter.h"


Interpreter_class::Interpreter_class( Interface_class* gui) :
Logfacility_class( "Interpreter" ),
Processor_class(gui)
{
	this->GUI = gui;
	ifd = gui->addr;

	main_view.name		= "Main osc";
	main_view.wfkey 	= SETWAVEFORMMAINKEY;
	main_view.ampkey 	= SETMBAMPPLAYKEY;//MASTERAMP_KEY;
	main_view.freqkey 	= MAINFREQUENCYKEY;

	vco_view.name		= "VCO";
	vco_view.wfkey 		= SETWAVEFORMVCOKEY;
	vco_view.ampkey 	= VCOAMPKEY;
	vco_view.freqkey 	= VCOFREQUENCYKEY;

	fmo_view.name		= "FMO";
	fmo_view.wfkey 		= SETWAVEFORMFMOKEY;
	fmo_view.ampkey 	= FMOAMPKEY;
	fmo_view.freqkey 	= FMOFREQUENCYKEY;

	main_view.wf 	= &ifd->MAIN_spectrum.id;
	main_view.amp 	= &ifd->StA_amp_arr[MbIdInstrument];
	main_view.freq 	= &ifd->Main_Freq;

	vco_view.wf 	= &ifd->VCO_spectrum.id;
	vco_view.amp 	= &ifd->VCO_Amp;
	vco_view.freq 	= &ifd->VCO_Freq;

	fmo_view.wf 	= &ifd->FMO_spectrum.id;
	fmo_view.amp 	= &ifd->FMO_Amp;
	fmo_view.freq 	= &ifd->FMO_Freq;

}

Interpreter_class::~Interpreter_class() {}

bool Interpreter_class::cmpkeyword ( const string word )
{
	String lower {keyword.Str};
	lower.to_lower();
	return ( lower.Str.compare( word ) == 0 );
}

void Interpreter_class::start_bin( vector_str_t arr )
{
	string cmd{};
	string exe{};
	expect = { "AudioServer", "Synthesizer" };
	intro( arr , 2 );

	if ( cmpkeyword( "synthesizer") )
	{
		exe = Synthesizer;
	}
	if ( cmpkeyword( "audioserver") )
	{
		exe = Audio_Srv;
	}
	if( exe.length() > 0 )
	{
		expect = { "shared memory key" };
		string shmkey = pop_stack( 1);

		Comment( INFO, "start " + keyword.Str );
		cmd = Server_struct().cmd( exe, "-k " + shmkey);
		expect 		= { "start delay in seconds" };
		option_default = "key";
		string duration = pop_stack( 0 );
		Processor_class::Push_cmd( CMD_EXE, cmd );
		pause( { "pause", duration } );
		return;
	}

	Wrong_keyword(  expect , keyword.Str );
	return ;
}

void Interpreter_class::stop_bin( vector_str_t arr )
{
	expect = { "AudioServer", "Synthesizer" };
	intro( arr, 1 );

	if ( cmpstr( keyword.Str, "Synthesizer" ) )
	{
		Comment( INFO, "terminating " + keyword.Str );
		Processor_class::Push_ifd( &ifd->Synthesizer, EXITSERVER, "terminating " + keyword.Str );
		Processor_class::Push_key( EXITKEY, "terminating key" + keyword.Str );
		return;
	}
	if ( cmpstr( keyword.Str, "AudioServer" ) )
	{
		Comment( INFO, "terminating " + keyword.Str );
		Processor_class::Push_ifd( &ifd->AudioServer, EXITSERVER, "terminating " + keyword.Str  );
		return;
	}

	Wrong_keyword( { "AudioServer, Synthesizer" }, arr[1] );

}

void Interpreter_class::intro( vector_str_t arr, uint min )
{
	error = 0;
	duration = 0;
	set_stack( arr, min );
	keyword.Str = pop_stack( 1 );
}

void Interpreter_class::record( vector_str_t arr )
{
	expect = { "stop", "file", "play", "loop" };
	intro( arr, 2);

	if ( cmpkeyword( "file" ) )
	{
		expect = { "Record duration in seconds" };
		option_default = "0";
		string duration = pop_stack( 0 );
		Processor_class::Push_ifd( &ifd->StA_amp_arr[MbIdExternal], 100, "mixer amp");
		Processor_class::Push_ifd( &ifd->Composer, RECORD, "composer record data" );
		Processor_class::Push_key( RECORDWAVFILEKEY, "start record"  );
		pause( {"pause", duration } );
		return;
	}
	if ( cmpkeyword( "stop") ) // stop record and write file with synthesizer.record_thead_fcn
	{
		expect = { "File number" };
		int FileNo = pop_int(0, 255 ) ;
		Processor_class::Push_ifd( &ifd->Composer,STOPRECORD, 	"composer stoprecord data" );
		Processor_class::Push_ifd( &ifd->FileNo, FileNo, 		"record file"  ); // trigger record_thead_fcn
		Processor_class::Push_key( RECORDWAVFILEKEY, 			"stop record" );
		return;
	}
	if ( cmpkeyword( "play") )
	{
		expect 			= { "file no", "replay duration in seconds" };
		string wavfile 	= "synthesizer" + pop_stack( 2 );
		string duration	= pop_stack(1);
		Processor_class::Push_str( SETEXTERNALWAVEFILE, OTHERSTR_KEY, wavfile );
		pause( {"pause", duration } );
		return;
	}
	if ( cmpkeyword( "loop") )
	{
		expect = {"amp"};
		keyword.Str = pop_stack( 1);
		if ( cmpkeyword( "amp") )
		{
			expect = { "loop final volume" };
			int amp = pop_int(0, 100);
			Loop( amp, EXTERNAL_AMPLOOP_KEY );
			return;
		}
		Wrong_keyword( expect , keyword.Str );

	}

	Wrong_keyword( expect , keyword.Str );

}

void Interpreter_class::random( vector_str_t arr )
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

void Interpreter_class::set( vector_str_t arr )
{
	expect = { "octave+", "octave-", "main", "vco", "fmo" };
	intro( arr, 2 );
	if ( cmpkeyword( "octave+" ))
	{
		Processor_class::Push_ifd( &ifd->FLAG, 1, "octave" );
		Processor_class::Push_key(SETBASEOCTAVE_KEY, "inc octave");
		return;
	}
	if ( cmpkeyword( "octave-" ))
	{
		Processor_class::Push_ifd( &ifd->FLAG, 0, "octave" );
		Processor_class::Push_key(SETBASEOCTAVE_KEY, "dec octave");
		return;
	}
	if ( cmpkeyword( "main" ) or cmpkeyword( "vco" ) or cmpkeyword( "fmo" ))
	{
		string osc = keyword.Str;
		expect = { "freq", "wf", "amp" };
		keyword.Str = pop_stack( 2 );
		show_items(arr);

		if( cmpkeyword( "freq") )
		{
			Value value{ pop_int(0, Variation.max_frequency) };
			cout << osc << "freq"<<value.str <<endl;
			Osc({ "osc", osc, "freq", value.str });
			return;
		}
		if( cmpkeyword( "wf") )
		{
			Value value{ pop_int(0, 8) };
			Osc({ "osc", osc, "wf", value.str });
			return;
		}
		if( cmpkeyword( "amp") )
		{
			Value value{ pop_int(0, 100 ) };
			Osc({ "osc", osc, "amp", value.str });
			return;
		}
		Wrong_keyword( expect , keyword.Str );

	}
	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::notes( vector_str_t arr )
{
	expect = { "load", "play" , "set", "per_second"};
	intro( arr, 2 );

	if ( cmpkeyword( "load") )
	{
		expect = { "Notes name" };
		string notes_name = pop_stack( 1);
		check_file( { dir_struct().autodir, dir_struct().notesdir }, notes_name + ".kbd" );

		Comment( INFO, "loading notes " + notes_name );
		Processor_class::Push_str( UPDATENOTESKEY, NOTESSTR_KEY, notes_name );
		return;
	}
	if ( cmpkeyword( "set") )
	{
		Variation.Read("tmp");
		string Noteline = Variation.Get_note_line();

		Comment(INFO, "set notes.");
		expect		=	{"prefix","octave", "[num]","sentence" };
		keyword.Str = pop_stack(1);

		if ( cmpkeyword("prefix") )
		{
			expect 		= {"#flats", "#sharps" };
			uint flats 	= pop_int(0,7);
			uint sharps = pop_int(0,7);
			Variation.Set_note_chars( flats, sharps );
			Variation.Save( "tmp", Variation.Noteline_prefix, Noteline );
			return;
		}
		if( cmpkeyword("octave") )
		{
			expect = { "octave value" };
			uint oct = pop_int( Variation.min_octave, Variation.max_octave ) ;
			Variation.Set_prefix_octave( oct );
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
			int amp 	= pop_int(0,100);
			Processor_class::Push_ifd( &ifd->StA_amp_arr[MbIdNotes] ,amp, "play notes" );
			Processor_class::Push_key( NOTESONKEY, "commit");
			if ( stack.size() > 0  )
			{
				keyword.Str  = stack.back();
				pause( {"pause", keyword.Str } );
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
		Value nps = pop_int(1,16);
		if ( Variation.Set_notes_per_second( nps.i ) )
		{
			Processor_class::Push_ifd( &ifd->noteline_prefix.nps, nps.i, "notes per second"  );
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


void Interpreter_class::instrument( vector_str_t arr )
{
	expect = { "load" };
	intro( arr, 2 );

	if ( cmpkeyword( "load") )
	{
		expect = { "instument name "};
		string instr = pop_stack( 1);
		check_file( { dir_struct().instrumentdir } , instr + ".kbd" );
		Comment( INFO, "loading instrument " + instr );
		Processor_class::Push_str( SETINSTRUMENTKEY, INSTRUMENTSTR_KEY, instr );
		return;
	}
	Wrong_keyword(  expect , keyword.Str );
;
}


void Interpreter_class::add( vector_str_t arr )
{
	expect = {"add", "instrument", "notes", "storage id", "duration|auto "};
	intro( arr, 4 );

	string inst		 	= keyword.Str;
	string note 		= pop_stack( 1);
	string amp 			= "0";
	string ma_id 		= pop_stack( 1 );

	vector_str_t vec;

	vec = { "instrument" , "load" , inst }; 	// SETINSTRUMENTKEY
	instrument( vec );
	vec = {"notes" , "load" , note };			// UPDATENOTESKEY
	notes( vec );
	vec = { "rec", "notes", ma_id };	//PLAYNOTESREC_ON_KEY
	rec( vec );
}

void Interpreter_class::Osc( vector_str_t arr )
{	// osc [main|vco|fmo] set waveid frequency amplitude duration

	expect = { "main", "vco", "fmo" };
	vector_str_t tmp = arr;
	intro( arr, 3);
	osc_struct_t view = osc_struct();
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

void Interpreter_class::osc_view( osc_struct_t view, vector_str_t arr )
{
	expect = { "mute", "unmute", "reset", "freq", "loop", "wf", "amp" };
	intro( arr, 2);

	bool loop = false;

	if ( cmpkeyword( "mute") )
	{
		Comment( INFO, "Master volume is muted " );

		Push_ifd( &ifd->mixer_status.mute, false, "false" );
		Push_key( MASTERAMP_MUTE_KEY, "mute master volume" );
		return;
	}

	if ( cmpkeyword( "unmute") )
	{
		Comment( INFO, "Master volume is un-muted " );

		Push_ifd( &ifd->mixer_status.mute, true, "true" );
		Push_key( MASTERAMP_MUTE_KEY, "un-mute master volume" );
		return;
	}
	if ( cmpkeyword( "reset" ))
	{
		Comment( INFO, "Reset connections");
		Processor_class::Push_key( RESETMAINKEY , "reset main"  );
		return;
	}

	if ( cmpkeyword( "loop") )
	{
		keyword.Str = pop_stack( 1);
		loop = true;
	}

	if ( cmpkeyword( "wf") )
	{
		expect = Spectrum.Get_waveform_vec();
		string waveform = pop_stack( 1);

		int wfid = Spectrum.Get_waveform_id( waveform );
		if ( wfid < 0 )
		{
			Wrong_keyword(expect, waveform);
			Exception();
//			raise( SIGINT );
		}
		expect 		= { " duration in seconds" };
		option_default = "0";
		string duration = pop_stack(0);
		Comment( INFO, "Set waveform " + waveform + " for " + view.name );

		Processor_class::Push_ifd( view.wf, wfid, "wafeform");
		Processor_class::Push_key( view.wfkey, "set waveform" );
		pause( { "pause", duration });
		return;
	}


	if ( cmpkeyword( "amp") )
	{
		expect = { "volume [%]" };
		int amp = pop_int(0,100);
		Comment( INFO, "Set amplitude " + to_string(amp) + " for " + view.name );
		if ( loop )
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
			pause( { "pause", duration });
 		}
		return;
	}


	if ( cmpkeyword( "freq") )
	{
		expect = { "frequency" };
		int freq = pop_int(0, 0xFFFF );
		Comment( INFO, "Set frequency " + to_string(freq) + " for " + view.name );
		if ( loop )
		{
			expect = {"max frequency" };
			freq = pop_int(0, Variation.max_frequency );
			Loop( freq, 0 );
		}
		else
		{
			expect 		= { " duration in seconds" };
			option_default = "0";
			string duration = pop_stack(0 );
			Processor_class::Push_ifd(  view.freq, freq, "frequency"  );
			Processor_class::Push_key(  view.freqkey, "set frequency"  );
			pause( { "pause", duration });
		}
		return;
	}


	Wrong_keyword( expect, keyword.Str );
}

void Interpreter_class::play( vector_str_t arr )
{ 	// play 1 amp 30
	// play 1 amp loop 30

	expect 			= { "storage id "};
	if( not set_stack( arr, 3 ) ) return;
	char mb_id 		= pop_int( 0, MbSize -1);

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
			Processor_class::Push_ifd( &ifd->MIX_Id, mb_id, "mixer id");
			expect = { " dest amp"};
			int max = pop_int(0,100);
			Loop( max, 0);
			return;
		}
		else
		{
			option_default = "10";
			string duration 	= k2;
			rec( {"rec", "amp", to_string( mb_id ), duration } );
			return;
		}
	}
	Wrong_keyword( expect , k1);

}

void Interpreter_class::rec( vector_str_t arr )
{
	expect = { "store", "amp", "stop", "play", "mute" , "clear"};
	intro( arr, 3 );
	uint max_id = MbSize -1;


	if ( cmpkeyword( "amp") )
	{
		int id 	= pop_int(0, max_id);
		expect = { "volume [%]" };
		int amp = pop_int(0,100);
		Comment( INFO, "set amplitude of " + to_string(id) + " to " + to_string(amp) + "%" );

		Processor_class::Push_ifd( &ifd->MIX_Id , id, "mixer id" );
		Processor_class::Push_ifd( &ifd->StA_amp_arr[ id ], amp, "mixer volume" );
		Processor_class::Push_ifd( &ifd->StA_status[id].play, true, "true" );
		Processor_class::Push_key( SETMBAMPPLAYKEY	, "set volume" );

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

		expect = {"mem id 0..5"};
		int ma_id = pop_int(0, MbSize-1 );
		Processor_class::Push_ifd( &ifd->MIX_Id, ma_id, "mixer id");
		Processor_class::Push_key( PLAYNOTESREC_ON_KEY, "notes on");
		return;
	}

	if ( cmpkeyword( "mute") )
	{
		int id = pop_int(0, max_id);
		Comment( INFO, "mute memory array: " + to_string(id) );
		Processor_class::Push_ifd( &ifd->MIX_Id, id, "sound" );
		Processor_class::Push_key(MUTEREC_KEY,  "stop sound" );
		return;
	}

	if ( cmpkeyword( "store") )
	{
		int id = pop_int(0, max_id);
		Comment( INFO, "store sound to: " + to_string(id) );
		Processor_class::Push_ifd( &ifd->MIX_Id, id, "sound" );
		Processor_class::Push_key( STORESOUNDKEY, "store sound" );
		return;
	}

	if ( cmpkeyword( "clear") )
	{
		int id = pop_int(0, max_id);
		Comment( INFO, "clear " + to_string(id) );
		Processor_class::Push_ifd( &ifd->MIX_Id, id, "mixer id" );
		Processor_class::Push_key( CLEAR_KEY,  "set clear" );
		return;

	}

	if ( cmpkeyword( "stop") )
	{
		int id = pop_int(0, max_id);
		Comment( INFO, "stop recording to: " + to_string(id) );
		Processor_class::Push_ifd( &ifd->MIX_Id, id, "sound" );
		Processor_class::Push_key( STOPRECORD_KEY, "stop sound" );
		return;
	}


	Wrong_keyword( expect , keyword.Str );

}
void Interpreter_class::text( vector_str_t arr )
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

void Interpreter_class::adsr( vector_str_t arr )
{
	expect =  { "pmw", "hall", "attack", "decay", "softfreq", "beat" };
	intro( arr, 2 );

	if ( cmpkeyword( "softfreq") )
	{
		Comment( INFO, "soft frequency is set to: " + stack[0] );
		int freq = pop_int(0,100);
		Processor_class::Push_ifd( &ifd->Soft_freq, freq, "soft freq"  );
		Processor_class::Push_key( SOFTFREQUENCYKEY,  "set soft freq" );
		return;
	}
	if ( cmpkeyword( "beat") )
	{
		Comment( INFO, "beat duration is set to: " + stack[0] );
		expect 		= { "0 1 2 4 5 8" };
		string Bps = pop_stack(1 );
		int bps_id = bps_struct().getbps_id( Bps );
		if ( bps_id < 0 )
		{
			Comment(ERROR, "wrong beat duration" );
			Exception();
			//raise( SIGINT );
		}
		Processor_class::Push_ifd( &ifd->Main_adsr.bps_id, bps_id, "beat duration" );
		Processor_class::Push_key( ADSR_KEY, "set beat duration" );
		return;
	}
	if ( cmpkeyword( "attack") )
	{
		Comment( INFO, "beat attack is set to: " + stack[0] );
		int attack = pop_int(0,100);
		Processor_class::Push_ifd( &ifd->Main_adsr.attack, attack, "adsr attack" );
		Processor_class::Push_key( ADSR_KEY, "set adsr attack" );
		return;
	}
	if ( cmpkeyword( "decay") )
	{
		Comment( INFO, "beat decay is set to: " + stack[0] );
		uint8_t decay = pop_int(0,100);
		Processor_class::Push_ifd( &ifd->Main_adsr.decay, decay, "adsr decay" );
		Processor_class::Push_key( ADSR_KEY, "set adsr decay" );
		return;
	}
	if ( cmpkeyword( "hall") )
	{
		Comment( INFO, "hall effect is set to: " + stack[0] );
		int hall = pop_int(0,100);
		Processor_class::Push_ifd( &ifd->Main_adsr.hall, hall, "hall"  );
		Processor_class::Push_key( ADSR_KEY,  "set hall" );
		return;
	}
	if ( cmpkeyword( "pmw") )
	{
		Comment( INFO, "PMW is set to: " + stack[0] );
		int dial = pop_int(0,100);
		Processor_class::Push_ifd( &ifd->PMW_dial, dial, "pmw" );
		Processor_class::Push_key( PMWDIALKEY, "set pmw" );
		return;
	}

	Wrong_keyword(expect, keyword.Str );

}

void Interpreter_class::pause( vector_str_t arr )
{

	expect 		= { "duration in seconds", "auto", "key" };
	intro( arr, 1 );

	if ( cmpkeyword( "key" ))
	{
		Processor_class::Push_wait( CMD_WAIT, -1, "press <ret> to continue" );

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

void Interpreter_class::addvariable( vector_str_t arr )
{
	auto key =  [] ( vector_str_t a )
			{ return ( a.size() > 0 ) ? a[0] : ""; };
	auto var_is_keyword = [  ]( string name, vector_str_t keywords )
			{
				for ( String Word : keywords )
				{
					Word.to_lower();
					if ( cmpstr( Word.Str, name) )
					{
						cout << "ERROR: " << name << " is a keyword" << endl;
						return true;
					}
				}
				return false;
			};
	auto is_notesfile = [](string str )
		{
			string filename = dir_struct().notesdir + str + ".kbd";
			return filesystem::exists(filename);
		};

	auto add_notesfile = []( string varname, string filea, string fileb )
		{
			string srca = dir_struct().notesdir + filea + ".kbd";
			string srcb = dir_struct().notesdir + fileb + ".kbd";
			string dest = dir_struct().autodir + varname + ".kbd";
			string cmd = "cat " +  srca + " " + srcb + " > " +  dest;
			system_execute( cmd );
		};

	error = 0;
	expect = {"expected <var name> = <var int value>"};
	string varname 	= key(arr); // first string needed

	if ( var_is_keyword ( varname, Keywords ) )
	{
		if ( not testrun) Exception();//raise( SIGINT);
		testreturn = true;
	};

	set_stack( arr, 2);
	keyword	= pop_stack( 1);
	if ( not cmpkeyword( "=" ))
	{
		Comment( ERROR, "missing equal sign");
		error = 1;
		return;
	}
	string varvalue 	= pop_stack( 1) ;
	if ( var_is_keyword ( varvalue, Keywords ) )
	{
		if ( not testrun) Exception( );//raise( SIGINT);
		testreturn = true;
	};



	if (( arr.size() > 0 ) and ( is_notesfile( varvalue) ))
	{
		expect = { "+", "notesfile name" };
		string op = pop_stack( 2 );
		if ( cmpstr( op, "+" ))
		{
			expect = { "notesfile name" };
			string name = pop_stack( 1 );
			if ( is_notesfile( name ))
				add_notesfile( varname, varvalue, name );
		}
	}
	else
	{
		varlist.push_back( { varname, varvalue} );

	}
}

vector_str_t Interpreter_class::insvariable( vector_str_t arr )
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


			if ( cmpstr(word, pair.name) )
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

void Interpreter_class::Loop( int max, int key )
{
	if ( key == 0 )
	{
		Processor_class::Push_wait( CMD_WAIT, -1, "not yet implemented" );
		return;
	}
	Processor_class::Push_ifd( &ifd->LOOP_end , max, "loop end" );
	Processor_class::Push_ifd( &ifd->LOOP_step, 1  , "loop step");
	Processor_class::Push_key( key,"set amp loop" );
}

void Interpreter_class::exit_interpreter()
{
	Processor_class::Push_cmd( CMD_EXIT,"exit" );
}

int Interpreter_class::find_position( vector<line_struct_t>* program, vector_str_t arr )
{ // find the line number in filelines starting with keyword

	expect = { "line tag" };
	set_stack( arr, 1 );
	keyword.Str = pop_stack( 1 );
	keyword.Str.append(" ");

	uint line_no = 0;
	for( line_struct_t prgline : *program )
	{
		string word = prgline.line;
		if ( cmpstr(keyword.Str, word) )
		{
			return line_no;
		}

		line_no++;
	}
	Comment( ERROR, "line >" + keyword.Str + " not found" );
	vector<line_struct_t> p = *program;
	std::for_each( p.begin(), p.end(),
			[] ( line_struct_t line )
			{ if ( line.keyw[0] == ':' ) cout << line.keyw << " " ;} );
	Exception( );//raise( SIGINT );
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
void Interpreter_class::Wrong_keyword( vector<string> expected , string given )
{
	error = 1;
	Comment( WARN, "unhandled keyword: " + given );
	Comment( INFO, "expected: "  );
	for ( string expect : expected )
		cout << expect << endl;
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

void Interpreter_class::clear_stack()
{
	Clear_process_stack();
}

void Interpreter_class::set_dialog_mode( bool mode )
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
			Exception( );//raise( SIGINT );
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
		cout << cmdline << " missing: [" + expect[0];
		expect.erase(expect.begin());
		for( string word : expect )
		{
			string print = word;
			cout.flush() << "|" << print  ;
		}
		cout << "]" ;
	}
}

int Interpreter_class::pop_int( int min, int max )
{
//	String Str;
	if ( option_default.size() == 0 )
		Str.Str = pop_stack( 1 );
	else
		Str.Str = pop_stack( 0 );

	if ( Str.is_number() )
	{
		int stack_value = Str.secure_stoi(Str.Str);
		if (( stack_value < min ) or ( stack_value > max ))
		{
			Comment( ERROR, "Value "+ Str.Str + " out of bounds" );
			Comment( INFO, "rejected: " + to_string(min) + "<" + Str.Str + "<" + to_string(max));
			show_expected();
			Exception( );//raise(SIGINT);
		}
		return stack_value;
	}
	else
	{
		error = 1;
		return 0;
	}
}

void Interpreter_class::run()
{
	Processor_class::Execute();
}


void Interpreter_class::check_file( vector_str_t dirs, string name )
{
	for ( string dir : dirs )
	{
		if ( filesystem::exists( dir + name ) ) return;
	};
	Comment( ERROR, "no such file " + name );
	for ( string dir : dirs )
	{
		show_items( List_directory( dir, ".kbd" ) );
	}
	Exception( );//raise( SIGINT );
}

void Interpreter_class::test(  )
{
	Set_Loglevel( TEST, true);
	Comment( TEST, "Interpreter_class test");
	testrun = true;

	notes( {"notes", "set", "ABCD EFGF", "8" } );
	Variation.Read("tmp" );
	assert( Variation.Get_note_line().compare("ABCD EFGF") == 0 );
	assert( Variation.Noteline_prefix.convention == 0 );

	notes( {"notes", "set", "prefix", "3", "2" } );
	Variation.Read("tmp" );
	assert( Variation.Noteline_prefix.flat == 3 );
	assert( Variation.Noteline_prefix.convention == 2 );
	assert( Variation.Get_note_line().compare("ABCD EFGF") == 0 );

	notes( {"notes", "set", "octave", "5" }) ;
	Variation.Read("tmp" );
	assert( Variation.Noteline_prefix.Octave == 5 );


	vector_str_t t_arr = { "This", "is", "test", "data" };
	vector_str_t v_arr = { "test" , "=", "4" };
	varlist.clear();

	t_arr = insvariable( t_arr ); //check emty varlist;
	assert( t_arr[0].compare("This") == 0);
	addvariable( v_arr );
	t_arr = insvariable( t_arr );

	show_items( t_arr );
	assert( t_arr[2].compare("4") == 0 );

	t_arr = { "call", "=" , "4" };

	testreturn = false;
	addvariable( t_arr ); //check no keyword
	assert( testreturn );

	t_arr = { "#call", "=" , "exit" };

	testreturn = false;
	addvariable( t_arr ); //check no keyword
	assert( testreturn );

	show_items( List_directory( dir_struct().instrumentdir, ".kbd" ) );
//	assert ( false );
	varlist.clear();

}
