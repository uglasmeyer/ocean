/*
 * SharedDataSegment.h
 *
 *  Created on: Oct 1, 2024
 *      Author: sirius
 */

#ifndef DATA_SHAREDDATASEGMENT_H_
#define DATA_SHAREDDATASEGMENT_H_

#include <Oscbase.h>
#include <data/Sdsbase.h>
#include <EventKeys.h>
#include <Wavedisplay_base.h>
#include <notes/Notesbase.h>
#include <Keyboard_base.h>


constexpr 	StA_state_arr_t init_StA_state_arr ()
{
	StA_state_arr_t A {};
	for( size_t n = 0; n < A.size(); n++ )
		 A[n] = default_StA_state;
	return A;
};

constexpr connect_arr_t default_connect_expr(  )
{
	connect_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = Connect_struct();
	}
	return arr;
};

constexpr adsr_arr_t default_adsr_expr(  )
{
	adsr_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_adsr;
	}
	return arr;
};

constexpr spectrum_arr_t default_spectrum_expr(  )
{
	spectrum_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_spectrum;
	}
	return arr;
};

constexpr feature_arr_t default_feature_expr(  )
{
	feature_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_feature;
	}
	return arr;
};


// there are MAXCONFIG interface structures in dataworld
// global values are managed in structure 0 only (master interface)
// local  values are interface specific

struct interface_struct // with reasonable defaults
{
	// local (interface specific
	Id_t			version						= DATA_VERSION;
	Id_t			SDS_Id						= 0;
	Id_t			config						= 0; // reference to the Synthesizer sds

	StA_state_arr_t	StA_state 					= init_StA_state_arr();	//
	StA_amp_arr_t	StA_amp_arr					{0,0,0,0,75,0,0,0};			// Instrument=75%
	mixer_state_t 	mixer_state 				= Mixer_base::mixer_state_struct(); //

	kbd_state_t		Kbd_state					= kbd_state_struct();

	char 			Instrument[str_buffer_len] 	{"default"}; //char array //
	char 			Notes	  [str_buffer_len]	{"default"}; //char array for the notes filename //
	char 			Other	  [str_buffer_len]	{"default"}; // e.g. external wave file name //


	/* instrument definition starts */
	feature_arr_t	features					= default_feature_expr();
	adsr_arr_t		adsr_arr					= default_adsr_expr();
	spectrum_arr_t	spectrum_arr				= default_spectrum_expr();
	connect_arr_t	connect_arr					= default_connect_expr();

	/* instrument definition ends	 */

	/* SDS specific control elements */

	OscId_t			Spectrum_type				= OscId_t::OSCID;//default_spectrum.osc;
	uint8_t			NotestypeId					= XML_ID; // musicxml
	noteline_prefix_t
					noteline_prefix				{ Note_base::noteline_prefix_struct() };
	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds //

	uint8_t 		MIX_Id						= STA_INSTRUMENT;

	uint8_t	 		FLAG						= CLEAR_KEY;
	uint8_t 		frq_slidermode				= SLIDE;	//

	bool 			UpdateFlag 					= true;
	uint8_t			time_elapsed 				= 0;
	appstate_arr_t	appstate_arr				{};//= defaultAppstate_expr();

	deque_t			deque						{ NULLKEY };
	eventptr_t		eventptr					= EventPtr_struct();
	char			eventstr[512]				{0};
	wd_status_t		WD_status					= WD_status_struct();

	/* SDS specific control elements end
	*/

	// global values of SDS 0 (sds_master)
	int8_t			mixer_balance				= 0; // nutral
	uint8_t			Master_Amp 					= osc_default_volume;
	uint8_t		 	vol_slidemode 				= SLIDE;
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
	uint8_t 		FileNo						= 0;		//

} ;
typedef interface_struct 		interface_t;
typedef vector<interface_t*> 	sds_vec_t;


#endif /* DATA_SHAREDDATASEGMENT_H_ */
