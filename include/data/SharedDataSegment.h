/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
 * SharedDataSegment.h
 *
 *  Created on: Oct 1, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_SHAREDDATASEGMENT_H_
#define DATA_SHAREDDATASEGMENT_H_

#include <data/Sdsbase.h>




// there are MAXCONFIG interface structures in dataworld
// global values are managed in structure 0 only (master interface)
// local  values are SDS interface specific

struct interface_struct // with reasonable defaults
{
	// local (interface specific
	/* SDS specific control elements */

	Id_t			version						= SDS_VERSION;
	Id_t			SDS_Id						= 0;
	bool			eventDebug					= true;

	StA_state_arr_t	StA_state_arr 				= default_StA_state_arr();	//
	StA_amp_arr_t	StA_amp_arr					{ StA_amp_data() };			// Instrument=75%
	mixer_state_t 	mixer_state 				= mixer_state_data(); //

	kbd_state_t		Kbd_state					= kbd_state_data();

	/* instrument definition starts */
	cstring_name	Instrument					{ "default" }; //char array for the instument name //
	feature_arr_t	features					= { default_feature };
	adsr_arr_t		adsr_arr					= { default_adsr };
	spectrum_arr_t	spectrum_arr				= { default_spectrum };
	connect_arr_t	connect_arr					= { default_connectId };
	/* instrument definition ends	 */

	// Notes
	cstring_name 	Notes	  					{ "default" }; //char array for the notes filename //
	cstring_long	Noteline					{""};
	cstring_long	Rythmline					{""};
	nlp_data_t		Noteline_prefix				= nlp_data() ;
	NOTETYPE_e		NotesTypeId					= XML_ID; // musicxml
	bool			NotesVerified				= false;
	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds //

	OSCID_e			SpectrumTypeId				= OSCID_e::OSCID;//default_spectrum.osc;
	StAId_e 		StA_Id						= STA_INSTRUMENT;
	uint8_t			beatClock					= 8; // Kbd notes_per measure (4 per sec)
	uint8_t			instrumentClock				= 1;
	DYNAMIC_e 		frq_slidermode				= INIT;	//
	uint8_t			time_elapsed 				= 0;		// Synth turn around time
	bool			overload					= false;	// Synthesizer
	uint8_t			resolutionIdx				= 0;		// fourier analysis
	appstate_arr_t	appstate_arr				= { appstate_data() };
	deque_t			deque						{ NULLKEY };
	eventptr_t		eventptr					= EventPtr_data();
	cstring_long	eventstr					{ "" };
	WD_data_t		WD_state					= WD_data_struct();
	wd_arr_t		wavedata 					{ 0 }; 		// wave display data
	ad_arr_t		adsrdata					{ 0 }; 		// adsr display data
	fftarr_t		fft_data					{ 0 };		// fft  display data

	/* ****************************************************************************************/
	/* SDS specific control elements end
	*/

	// global values of SDS MASTER_SDSID
	// (addr sds_master - see appstate.h)
	Id_t			comId						= MAIN_SYNTHID; // SDS_ID of the currently communicating
																// muti-run app e.g. Synthesizer or Keyboard
	EVENTFLAG_e 	FLAG						= CLEAR_FLAG; // communicate back from Synth to GUI
	cstring_name	Session						{ "" };
	cstring_name 	WAV_filename	  			{ "default" }; // e.g. external wave file name //
	int8_t			mixer_balance				= 0; // nutral
	uint8_t			Master_Amp 					= osc_default_volume; // Audioserver
	DYNAMIC_e		Master_vol_slidemode		= FIXED; // used by the Audioserver
	uint8_t			Master_vol_slide_value		= 0; // % of 4*max_seconds for the master volume
	bool 			UpdateFlag 					= true;		// update SDSview
	KEYCODE			sdsview_page				= KEYCODE::F1; // SDSview
	Id_t			sdsview_sdsid				= 0;		// SDSview
	bool			overmodulated				= false;	// Audioserver
	uint8_t 		SHMID 						= 0;		// synchronization
	uint8_t 		RecCounter					= 0;		// Progressbar file record
	uint8_t 		FileNo						= 1;		// generate filename
	bool			overwritemode				= true;		// see GUI SaveRecord
	trigger_data_t	Note_start					= trigger_data_struct(); // TODO to be evaluated
	trigger_data_t	Note_end					= trigger_data_struct(); // by AudioServer
	StateId_e		Record_state				= StateId_e::INACTIVE; 	// Audioserver recording state
	StateId_e		Capture_state				= StateId_e::INACTIVE;  // RECORDING, STARTING,
																		// STOPPING, INACTIVE
	// recording activity is managed by the Audioserver
} ;
typedef interface_struct 		interface_t;
typedef vector<interface_t*> 	sds_vec_t;


#endif /* DATA_SHAREDDATASEGMENT_H_ */
