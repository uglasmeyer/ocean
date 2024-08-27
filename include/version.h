/*
 * version.h
 *
 *  Created on: Jan 19, 2024
 *      Author: sirius
 */

#ifndef INCLUDE_VERSION_H_
#define INCLUDE_VERSION_H_

#include <string>

#ifdef RTAUDIO
#include <string>
#else
#include <string.h>
#endif

using namespace std;

#define MAYOR 2
#define MINOR 1
#define PATCH 0

const string Version_No  = 	to_string(MAYOR) + "." +
							to_string(MINOR) + "." +
							to_string(PATCH);

const string Version_str = " Version: " + Version_No;

const string Application = "Ocean Sound ";


// Git comment:
/*

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
