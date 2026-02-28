/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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
 * apptest.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Event.h>
#include <App.h>
auto test_cycle_range = [  ]( range_T<int> range  )
{
	string 	range_str 	= to_string( range.min) + "..." + to_string(range.max) ;
	int
	value = check_cycle( range, 1, range_str );
	Assert_equal( value, 1, range_str );
	value = check_cycle( range, 4, range_str );
	Assert_equal( value, 4 , range_str);
	value = check_cycle( range, 5, range_str );
	Assert_equal( value, 1, range_str );

};
auto test_cycle2_range = [  ]( range_T<int> range  )
{
	ASSERTION( range.len > 0, "range.len", range.len, ">0" );
	string
	range_str = to_string( range.min) + "..." + to_string(range.max) ;
	int value0 = range.min + range.len/2;
	int value1 = check_cycle2(range, value0 , "" );
	Assert_equal( value0 , value1, range_str );
	int value = range.max;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.min, range_str,
				check_cycle2(range, value, "" ), value );
	 value = range.min;
	ASSERTION( 	check_cycle2(range, value , "" ) == value, range_str,
				check_cycle2(range, value, "" ), value );
	 value = range.min-1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.max -1, range_str,
				check_cycle2(range, value, "" ), range.max - 1 );
	 value = range.max + 1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.min +1, range_str,
				check_cycle2(range, value, "" ), range.min+1 );
	 value = range.max + 2*range.len +1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.min + 1, range_str,
				check_cycle2(range, value, "" ), range.min+1 );
	 value = range.min - 2*range.len - 1;
	ASSERTION( 	check_cycle2(range, value , "" ) == range.max - 1, range_str,
				check_cycle2(range, value, "" ), range.max-1 );
	ASSERTION( range.len == abs(range.max-range.min), "", range.len, range.max-range.min);
};

bool 							isopen 		= false;
extern Config_class 			Cfg;
extern Logfacility_class		Log;
extern Dataworld_class 			DaTA;
extern Instrument_class 		Instrument;
extern Mixer_class				Mixer;
extern Note_class 				Notes;
extern Keyboard_class			Keyboard;
extern External_class 			External;
extern Time_class				Timer;
extern Event_class				Event;


void SynthesizerTest()
{
	String 			TestStr	{""};
	Oscillator 		TestOsc	{ INSTRROLE, OSCID, monobuffer_bytes };
	Spectrum_class 	Spectrum{};

	Log.Set_Loglevel( TEST, true );
	DaTA.Test_result = "FAILED";
//	DaTA.Sds_p->Reset_ifd();
	coutf << "SynthesizerTestCases" << endl;

	Cfg.Test();

	DaTA.SHM_l.Test_Memory();
	Notes.TestFrequency();

	Log.Init_log_file();
	Log.Test_Logging();

	range_T<int> range_test { 1,4 };
	test_cycle2_range( range_test );
	test_cycle_range( range_test );

	TestStr.TestString();

	Spectrum.Test_Spectrum();
	Notes.TestNoteBase();
	Notes.Test_Musicxml();

	TestOsc.DynFrequency.TestFrq();
	Mixer.DynVolume.TestVol( );
	Mixer.TestMixer();

	Keyboard.Kbd_pitch_Test();

	External.Test_External();

	Timer.Test();

	DaTA.Sem_p->Test();
	DaTA.Sds_p->Reset_ifd();
	Instrument.Test_Instrument();
	Instrument.Oscgroup.Run_OSCs( 0 );

	System_Test();

	DaTA.Test_Dataworld();
	DaTA.Sds_p->Test_interface();

	Event.TestHandler();

	DaTA.Sds_p->Restore_ifd();
	Log.Set_Loglevel( TEST, true );
	DaTA.Test_result = "SUCCESS";
}

