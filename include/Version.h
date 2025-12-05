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
 * version.h
 *
 *  Created on: Jan 19, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_VERSION_H_
#define INCLUDE_VERSION_H_


#include <string>
using namespace std;

#define MAYOR 4
#define MINOR 1
#define PATCH 1
#define DATA_VERSION 40

const string Version_No  = 	to_string(MAYOR) + "." +
							to_string(MINOR) + "." +
							to_string(PATCH) + "-" +
							to_string(DATA_VERSION);

const string Version_str = "Version: " + Version_No;

const string Application = "Ocean Sound Lab.";

const string License_text = R"( 
    Copyright (C) 2025  Ulrich.Glasmeyer@web.de
    This program comes with ABSOLUTELY NO WARRANTY; for details read the license conditions.
    This is free software, and you are welcome to redistribute it
    under the  conditions of the MIT license <https://www.gnu.org/licenses/>)";

// Git comment:
/*
version 4.1.1	cb_external unknown syymbol
version 4.1.0	Ocean GUI - Cut Desk
				DATA_VERSION 40 Wd_state record information
				Wavedisplay Focus on action
				Save Storage Areas on exit
				Play WAV file does not work
				feature autovolume
version 4.0.5	Synchronization of StAs recording with beat trigger
				BeatTrigger DATA_VERSION 39
version 4.0.4	Composer dialog mode - preparation
				remove absolute path from make_moc.sh
				Cannot shutdown running Synthesizer on high SDS by using SDSview
version 4.0.3	update install.txt, license informations in files and terminal programs				update license informations in files and terminal programs
version 4.0.2	x86_64 binary distribution ocean_sound_lab_4.0.2-37.tar.gz
version 4.0.1	wavedisplay external with dynamic size
				DATA_VERSION 37
version 4.0.0	github version
				use scanner for Stereo data
version 3.12.2	SDSMenu_class
				sdsview_page DATA_VERSIOM 35
				process_arr_t replaced by appstate_arr_t DATA_VERSION 34
				xmleditorid
				process_pid_t DATA_VERSION 33
version 3.12.1 Synchronization issue
				kbd_stat.chord_type DATA_VERSION 32
				octaveshift DATA_VERSION 31
				XmlFile2notelist
				Longnote
version 3.12.0 Keyboard dialog class
				trigger definition
				DATA_VERSION
version 3.11.3 record stopping by note_itr
				file number handling
version 3.11.2 connect fixes
version 3.11.1 Generate_volatile_data
version	3.11.0 scanner reorganized
version 3.10.0 Introduction of Device_class
version 3.9.0 Save Keyboard Notes
version 3.8.8 ADSR Waveform fixes
version 3.8.7 ADSR Waveform redesign
version 3.8.6 Adsr waveform
version 3.8.5 Separate feature from adsr
				Show ADSR waveform
version 3.8.4 ADSR for Fmo/Vco
version 3.8.3 Integration of Setup
version 3.8.2
				oscgroup.Set_Note_Frequency
				keyboard as a process
version 3.8.1 Keyboard F1 layout
				reduced Shm size (audio_frame)
				float Shm
				Audioserver dynamic_limit
version 3.8.0 Keyboard features
				SetDs for storage_class
version 3.7.4 AudioVolume_class
version 3.7.3 Fast notes
				Keyboard layout for 3 Octaves
				min_milli_sec		= 200;
version 3.7.2 Dataworld refactoring
				Appstate inroduction

version 3.7.1 Keyboard fixes
				logmask bitset
				cmomstack viewinterface
version 3.7.0 connection feature rework
				sem timeout during valgrind test
				simplify dynamic
				dynamic frequency for fmo and vco
version 3.6.6 rework hall effect feature
version 3.6.5 rework Dynamic_class
version 3.6.4 reduction of redundancies
version 3,6.3 Test rework
version 3,6.2 formatted Table
version 3.6.1 Volume_class, FrqNamesArray
version 3.6.0 sliding volume
version 3.5.1 EventQue_class and Test_interface()
version 3.5.0 introduction of an eventque
version 3.4.4 waveformvector external
version 3.4.3 oscwavefunction class
version 3.4.2 file dialog path_t structure
version 3.4.1 structure and workflow changes
version 3.4.0 spectrum harmonic adjustments
version 3.3.3 Frequency_class introduced
version 3.3.2 multi oscillators and ASSERTION macro
version 3.3.1 re-integrate composer and rtsp
version 3.3.0 integration of fft
version 3.2.0 introduction of oscgroup
version 3.1.0 variable audioframes
version 3.0.5 notes test, audioout, adsr optimization + fixes
version 3.0.4 OceanGUI core dump
version 3.0.3 fixes sync mode timing
version 3.0.2 fixes
version 3.0.1 data exchange rate adopted to 2 sec
version 3.0.0 Introduction of MusicXML
version 2.4.1 end
version 2.4.0 deployment
version 2.3.4 refractoring nps, bps, synthesizer.h
version 2.3.3 Multiple Synthesizer Recording
version 2.3.2 Multiple Synthesizer sdsid mgnt.
version 2.3.1 Multiple Synthesizer fixes
version 2.3.0 Multiple Synthesizer
version 2.2.2 RTSP version 0
version 2.2.1 optomize startup sequence
version 2.2.0 introduction of semaphores
version 2.1.2 reactivate Keyboard fixes with decay ccounter
version 2.1.1 reactivate Keyboard integrate
version 2.1.0 config facility
version 2.0.4 replace exit_proc by classs destructors
version 2.0.3 Feature and Refactoring
version 2.0.2 Deact Keyboard integrate noteline_prefix into OceanGUI
version 2.0.1 Reactivate Keyboard
version 2.0.0 Ocean initial commit
version 1.31.0 spectrum for fmo and vco
version 1.30.0 change back to std=c++17
version 1,29.1 long notes fixes
version 1,29.0 long notes
version 1,28.0 New waves
version 1,27.0 Notes features
version 1,26.5 Refactoring interpreter/processor
version 1.26.4 Refactorimg spectrum
version 1.26.3 Refactorimg adsr
version 1.26.2 free run mode fixes
version 1.26.1 LFO redesign
version 1.26.0 declare data before function in classes
version 1.25.0 documentation added
version 1.24.0 variation refactored
version 1.23.0 random variation between 3 octaves
version 1.22.4 refractoring interpreter
version 1.22.3 refactoring waveform
version 1.22.2 spectrum fixes
version 1.22.1 volume loops and fixes
version 1,22.0 Composer important fixes
version 1.21.1 Spectrum completed
version 1.20.0 Spectrum
version 1.19.0 polyphone notes
version 1.18.4 composer include files
version 1.18.3 conditional wait
version 1.18.2 composer final adjustments
version 1.18.1 composer syntax adjustments
version 1.18.0 Composer
version 1.17.0 Counter class
version 1.16.2 mixer rework
version 1.16.1 small adjustments
version 1.16.0 Music Hall effect
version 1.15.3 a lot of small adjustments
version 1.15.2 adsr rework
version 1.15.1 stereo rework completed
version 1.15.0 record file rename
version 1.14.0 stereo
version 1.13.1 external and notes in memory bank
version 1.13.0 replay wav file
version 1.12.0 IO Performance
version 1.11.0 VCO modulation changed
version 1.10.1 osc adjustments
version 1.10.0 synchronization finalized
version 1.9.1 many bug fixes
version 1.9.0 Wavedisplay
version 1.8.4 optimize synchronization
version 1.8.3 synchronization improvements
version 1.8.2 persistent memory
version 1.8.1 fix volume change doesn't work
version 1.8.0 change noteline
version 1.7.0 file director
version 1.6.2 Instrument re-factoring
version 1.6.1 all in
version 1.5.1 playnote fixes more or less finalized
version 1.5.0 ADSR / delay
*/


#endif /* INCLUDE_VERSION_H_ */
