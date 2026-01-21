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

	// ADSR
	ADSR_KEY,
	ADSRALL_KEY,
	FEATURE_KEY,
	PWMDIALKEY,
	SOFTFREQUENCYKEY,

	// DataArray
	STARECORD_START_KEY,
	STOPRECORD_KEY,
	STA_VOLUME_KEY,
	SETSTA_KEY,
	MUTEMBKEY,
	MUTEREC_KEY,
	CLEAR_KEY,
	RESET_STA_SCANNER_KEY,

	// Notes
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
	STARECORD_STOP_KEY,

	// Instrument
	SAVEINSTRUMENTKEY,
	NEWINSTRUMENTFLAG,
	SETINSTRUMENTKEY,
	INSTRUMENTSTR_KEY,

	// Spectrum
	UPDATESPECTRUM_KEY,
	GETSPECTRUM_KEY,

	// OceanGUI
	UPDATE_KBDDIALOG_FLAG,
	UPDATELOG_EVENT,
	APPSTATE_FLAG,

	// Cutter
	CUT_KEY,
	CUT_UPDATE_KEY,
	CUT_SETUP_KEY,
	CUT_SAVE,
	CUT_RESTORE_KEY,
	CUT_UPDATE_DISPLAY_FLAG,

	EXITKEY
};





#endif /* INCLUDE_KEYS_H__ */
