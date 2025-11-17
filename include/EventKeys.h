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
 * keys.h
 *
 *  Created on: Apr 1, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

/*
https://en.wikipedia.org/wiki/ASCII#Character_set
 */


#ifndef INCLUDE_KEYS_H_
#define INCLUDE_KEYS_H_


enum EVENTKEY_e
{
	NULLKEY = 0,

	// OSC frequency
	OSCFREQUENCYKEY,
	VCOFREQUENCYKEY,
	FMOFREQUENCYKEY,

	// OSC volume
	VCOAMPKEY,
	FMOAMPKEY,
	MASTERAMP_KEY,
	MASTERAMP_LOOP_KEY,
	MASTERAMP_MUTE_KEY,

	// OSC adjust
	ADJUST_KEY,

	// OSC Waveform
	SETWAVEFORMFMOKEY,
	SETWAVEFORMVCOKEY,
	SETWAVEFORMMAINKEY,


	// OSC connect
	CONNECTOSC_KEY,
	CONNECTVCO_KEY,
	CONNECTFMO_KEY,

	// ADSR
	ADSR_KEY,
	ADSRALL_KEY,
	FEATURE_KEY,
	PWMDIALKEY,
	SOFTFREQUENCYKEY,

	// DataArray
	STORESOUNDKEY,
	STOPRECORD_KEY,
	SETSTA_KEY,
	MUTEMBKEY,
	MUTEREC_KEY,
	SETSTAPLAY_KEY,
	CLEAR_KEY,
	EXTERNAL_AMPLOOP_KEY,
	RESET_STA_SCANNER_KEY,

	// Notes
	NEWNOTESLINEKEY,
	NEWNOTESLINEFLAG,
	UPDATENOTELINEKEY,
	UPDATENOTESKEY,
	NOTESONKEY,
	NOTESOFFKEY,
	PLAYNOTESREC_ON_KEY,
	PLAYNOTESRECOFF_KEY,
	SETNOTESPERSEC_KEY,
	SETBASEOCTAVE_KEY,
	NOTESSTR_KEY,
	UPDATE_NLP_KEY,
	XMLFILE_KEY,

	// Keyboard
	KBD_EVENT_KEY,
	KBD_SAVE_KEY,

	// Wavedisplay
	SETWAVEDISPLAYKEY,

	// External
	SAVE_EXTERNALWAVFILEKEY,
	RECORDWAVFILEFLAG,
	READ_EXTERNAL_WAVFILE,
	WAVFILESTR_KEY,
	OTHERSTR_KEY,
	STARTRECORD_KEY,
	STOP_STARECORD_KEY,

	// Instrument
	SAVEINSTRUMENTKEY,
	NEWINSTRUMENTKEY,
	NEWINSTRUMENTFLAG,
	SETINSTRUMENTKEY,
	INSTRUMENTSTR_KEY,

	// Spectrum
	UPDATESPECTRUM_KEY,
	GETSPECTRUM_KEY,

	UPDATELOG_EVENT,
	APPSTATE_FLAG,

	EXITKEY
};

#include <vector>

const std::vector<EVENTKEY_e> init_keys =
{
		// avoid toggle commands here
		SETINSTRUMENTKEY,
		SETWAVEDISPLAYKEY,
		ADSRALL_KEY,
		FEATURE_KEY,
		PWMDIALKEY,
		SOFTFREQUENCYKEY,
		OSCFREQUENCYKEY,
		VCOFREQUENCYKEY,
		FMOFREQUENCYKEY,

		SETSTA_KEY
};




#endif /* INCLUDE_KEYS_H__ */
