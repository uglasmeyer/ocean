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
// local  values are interface specific

struct interface_struct // with reasonable defaults
{
	// local (interface specific
	Id_t			version						= DATA_VERSION;
	Id_t			SDS_Id						= 0;
	Id_t			config						= 0; // reference to the Synthesizer sds

	StA_state_arr_t	StA_state_arr 				= default_StA_state_arr();	//
	StA_amp_arr_t	StA_amp_arr					{0,0,0,0,75,0,0,0};			// Instrument=75%
	mixer_state_t 	mixer_state 				= mixer_state_struct(); //

	kbd_state_t		Kbd_state					= kbd_state_struct();

	cstring_t		Instrument				 	{"default"}; //char array //
	cstring_t 		Notes	  					{"default"}; //char array for the notes filename //
	cstring_t 		Other	  					{"default"}; // e.g. external wave file name //


	/* instrument definition starts */
	feature_arr_t	features					= default_feature_expr();
	adsr_arr_t		adsr_arr					= default_adsr_expr();
	spectrum_arr_t	spectrum_arr				= default_spectrum_expr();
	connect_arr_t	connect_arr					= default_connect_expr();

	/* instrument definition ends	 */

	/* SDS specific control elements */

	OSCID_e			Spectrum_type				= OSCID_e::OSCID;//default_spectrum.osc;
	NOTETYPE_e		NotestypeId					= XML_ID; // musicxml
	noteline_prefix_t
					noteline_prefix				{ Note_base::noteline_prefix_struct() };
	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds //

	STAID_e 		MIX_Id						= STA_INSTRUMENT;

	uint8_t	 		FLAG						= CLEAR_KEY;
	DYNAMIC 		frq_slidermode				= SLIDE;	//

	bool 			UpdateFlag 					= true;
	uint8_t			time_elapsed 				= 0;
	appstate_arr_t	appstate_arr				{};//= defaultAppstate_expr();

	deque_t			deque						{ NULLKEY };
	eventptr_t		eventptr					= EventPtr_struct();
	char			eventstr[512]				{0};
	WD_data_t		WD_status					= WD_data_struct();

	/* SDS specific control elements end
	*/

	// global values of SDS 0 (sds_master)
	int8_t			mixer_balance				= 0; // nutral
	uint8_t			Master_Amp 					= osc_default_volume;
	DYNAMIC		 	vol_slidemode 				= SLIDE;
	uint8_t			slide_duration 				= 50; // % of 4*max_seconds
	KEYCODE			sdsview_page				= KEYCODE::F1;
	buffer_t		audioframes					= audio_frames;
	wd_arr_t		wavedata 					= {};
	uint8_t 		SHMID 						= 0;//
	uint8_t 		RecCounter					= 0;		// handshake data exchange//
	trigger_data_t	Note_start					= trigger_data_struct(); // TODO to be evaluated
	trigger_data_t	Note_end					= trigger_data_struct(); // by AudioServer

	StateId_t		Record_state				= sdsstate_struct::INACTIVE;
	// current recording activity managed by the Audioserver
	// Audioserver recording state
	// RECORDING, STARTING, STOPPING, INACTIVE

												// sta-scanner, note_itr_end
	uint8_t 		FileNo						= 1;// generate filename

} ;
typedef interface_struct 		interface_t;
typedef vector<interface_t*> 	sds_vec_t;


#endif /* DATA_SHAREDDATASEGMENT_H_ */
