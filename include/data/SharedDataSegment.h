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

constexpr process_arr_t init_process_arr()
{
	process_arr_t arr {};
	for( uint n = 0; n < REGISTER_SIZE; n++ )
	{
		arr[n] = register_process_struct();
	}
	return arr;
}
constexpr connect_arr_t default_connect_expr(  )
{
	connect_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_connect( oscid );
	}
	return arr;
};
const connect_arr_t default_connect_arr = default_connect_expr(  );

constexpr adsr_arr_t default_adsr_expr(  )
{
	adsr_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_adsr;
	}
	return arr;
};
const adsr_arr_t default_adsr_arr = default_adsr_expr(  );

constexpr spectrum_arr_t default_spectrum_expr(  )
{
	spectrum_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_spectrum;
	}
	return arr;
};
const spectrum_arr_t default_spectrum_arr = default_spectrum_expr(  );

constexpr feature_arr_t default_feature_expr(  )
{
	feature_arr_t arr{};
	for ( char oscid = 0; oscid < OSCIDSIZE; oscid++ )
	{
		arr[oscid] = default_feature;
	}
	return arr;
};
const feature_arr_t default_feature_arr = default_feature_expr(  );


// there are MAXCONFIG interface structures in dataworld
// global values are managed in structure 0 only (master interface)
// local  values are interface specific
typedef struct interface_struct // with reasonable defaults
{
	// local (interface specific
	uint8_t			version						= 24;
	int8_t			SDS_Id						= 0;
	uint8_t			config						= 0; // reference to the Synthesizer sds

	StA_state_arr_t	StA_state 					= default_sta_state_arr;	// comstack
	StA_amp_arr_t	StA_amp_arr					{0,0,0,0,75,0,0,0};			// Instrument=75%
	mixer_status_t 	mixer_status 				= Mixer_base::mixer_state_struct(); // comstack

	kbd_state_t		Kbd_state					= kbd_state_struct();

	char 			Instrument[str_buffer_len] 	{"default"}; //char array // comstack
	char 			Notes	  [str_buffer_len]	{"default"}; //char array for the notes filename // comstack
	char 			Other	  [str_buffer_len]	{"default"}; // e.g. external wave file name // comstack

	uint8_t			Master_Amp 					= 75;// comstack
	int8_t			mixer_balance				= 0; // nutral
	uint8_t		 	vol_slidemode 				= SLIDE;
	uint8_t			slide_duration 				= 50; // % of 4*max_seconds

	/* instrument definition starts */
	feature_arr_t	features					= default_feature_expr();
	adsr_arr_t		adsr_arr					= default_adsr_arr;
	spectrum_arr_t	spectrum_arr				= default_spectrum_arr;
	connect_arr_t	connect_arr					= default_connect_arr;

	/* instrument definition ends	 */

	/* SDS specific control elements */

	uint8_t			Spectrum_type				= default_spectrum.osc;
	uint8_t			NotestypeId					= XML_ID; // musicxml
	noteline_prefix_t
					noteline_prefix				{ Note_base::noteline_prefix_struct() };
	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds // comstack

	uint8_t 		MIX_Id						= 0;// comstack

	uint8_t	 		FLAG						= CLEAR_KEY;
	uint8_t 		frq_slidermode				= SLIDE;	// comstack

	bool 			UpdateFlag 					= true;
	uint8_t			time_elapsed 				= 0;

	deque_t			deque						{ NULLKEY };
	eventptr_t		eventptr					= EventPtr_struct();
	char			eventstr[512]				{0};
	wd_status_t		WD_status					= WD_status_struct();

	uint8_t 		Synthesizer					= sdsstate_struct::DEFAULT;// indicates that shm is new // comstack
	uint8_t			Keyboard					= sdsstate_struct::OFFLINE; // if tty and synthesizer process
	/* SDS specific control elements end
	*/

	// global values of SDS 0
	uint8_t 		AudioServer	    			= sdsstate_struct::OFFLINE;
	uint8_t	 		UserInterface				= sdsstate_struct::OFFLINE;
	uint8_t	 		Composer 					= sdsstate_struct::OFFLINE;
	uint8_t			Comstack					= sdsstate_struct::OFFLINE;
	uint8_t			Rtsp						= sdsstate_struct::OFFLINE;
	uint8_t			Setup						= sdsstate_struct::OFFLINE; // if tty and synthesizer process

	buffer_t		audioframes					= audio_frames;
	wd_arr_t		wavedata 					= {};
	process_arr_t	process_arr					= init_process_arr(); //{ {register_process_struct()} };
	uint8_t 		SHMID 						= 0;// comstack
	uint8_t 		RecCounter					= 0;		// handshake data exchange// comstack
	uint8_t			Record						= sdsstate_struct::RECORDSTOP; 	// Audioserver recording
	uint8_t 		FileNo						= 0;		// comstack

} interface_t;



#endif /* DATA_SHAREDDATASEGMENT_H_ */
