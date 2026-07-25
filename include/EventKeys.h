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

enum CSTRKEY_e : uint8_t
{
	NOTENAMESTR,
	NOTELINESTR,
	RHYTHMLINESTR,
	INSTRUMENTSTR,
	WAVFILE_STR,		// sds_master
	SESSIONSTR,			// sds_master
	UPDATELOG_STR
};

// receiver is the GUI dispatcher
enum EVENTFLAG_e : uint8_t
{
	CLEAR_FLAG, 				// no action
	READWAVEDATAFLAG,
	NEWNOTESLINEFLAG,
	NEWINSTRUMENTFLAG,
	APPSTATE_FLAG,
	UPDATE_CUTDESK_FLAG,
	UPDATE_FILEDIALOG_FLAG,
	UPDATE_KBDDIALOG_FLAG,
	UPDATE_WAV_FILELIST_FLAG,
	UPDATELOG_FLAG
};

// receiver is  the Synthesizer handler
enum EVENTKEY_e : uint8_t
{
	NULLKEY = 0,
	COMMIT_KEY,

	// OSC frequency
	OSCFREQUENCYKEY,
	VCOFREQUENCYKEY,
	FMOFREQUENCYKEY,
	FREQDYNAMIC_KEY,
	MAINFRQ_KEY,

	// OSC volume
	VCOAMPKEY,
	FMOAMPKEY,
	MASTERAMP_KEY,
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
	FEATURE_KEY,
	PWMDIALKEY,
	BEATCLOCK_KEY,

	// StorageArray
	STA_RECORD_KEY,
	STOPRECORD_KEY,
	STA_VOLUME_KEY,
	SETSTA_KEY,
	MUTESTA_KEY,
	CLEAR_KEY,
	RESET_STA_SCANNER_KEY,
	RESETSTA_KEY,


	// Notes
	GENERATENOTES_KEY,
	UPDATENOTELINEKEY,
	UPDATENOTESKEY,
	NOTESONKEY,
	NOTESOFFKEY,
	PLAYNOTESREC_ON_KEY,
	PLAYNOTESRECOFF_KEY,
	SETNOTESPERSEC_KEY,
	READNOTESFILE_KEY,
	UPDATE_NLP_KEY,
	SAVENOTES_KEY,

	// Keyboard
	KBD_EVENT_KEY,
	KBD_SAVE_KEY,

	// Wavedisplay
	SETWAVEDISPLAYKEY,

	// External
	EXTERNAL_WAVFILEKEY,
	READ_WAVFILE_KEY,
	WAVFILESTR_KEY,
	STARTRECORD_KEY,
	CAPTURE_KEY,
	OTHERSTR_KEY,

	// Instrument
	SAVEDEFAULT_KEY,
	SAVEINSTRUMENTKEY,
	SETINSTRUMENTKEY,

	// Spectrum
	UPDATESPECTRUM_KEY,
	GETSPECTRUM_KEY,

	// Cut Desk
	CUT_KEY,
	CUT_UPDATE_KEY,
	CUT_SETUP_KEY,
	CUT_SAVE_KEY,
	CUT_RESTORE_KEY,
	CUT_STEPWIDTH_KEY,

	// Controls
	SAVESESSION_KEY,
	SETSESSIONDIR_KEY,
	FOURIERRESOLUTION_KEY,
	EXITKEY
};





#endif /* INCLUDE_KEYS_H__ */
