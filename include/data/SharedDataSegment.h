/*
 * SharedDataSegment.h
 *
 *  Created on: Oct 1, 2024
 *      Author: sirius
 */

#ifndef DATA_SHAREDDATASEGMENT_H_
#define DATA_SHAREDDATASEGMENT_H_


#include <EventKeys.h>
#include <Ocean.h>
#include <Mixerbase.h>
#include <Oscbase.h>
#include <Wavedisplay_base.h>
#include <Configbase.h>
#include <notes/Notesbase.h>
#include <Keyboard_base.h>

struct sdsstate_struct
{
	enum {
		 OFFLINE	,
		 RUNNING	,
		 FREERUN	,
		 UPDATEGUI 	,
		 SYNC 		,
		 DEFAULT	,
		 EXITSERVER	,
		 KEYBOARD	,
		 RECORDSTART,
		 RECORDSTOP ,
		 LASTNUM
		};

	vector<string> state_map
	{
		{"OFFLINE" },
		{"RUNNING" },
		{"FREERUN" },
		{"UPDATEGUI" },
		{"SYNC" },
		{"DEFAULT" },
		{"EXITSERVER" },
		{"KEYBOARD" },
		{"RECORDSTART" },
		{"RECORDSTOP" },
	};


	~sdsstate_struct() = default;
};

typedef struct EventPtr_struct
{
	uint8_t 		first 	= 0;
	uint8_t 		last 	= 0;
	uint8_t			length	= 0;
} eventptr_t;

typedef struct register_process_struct
{
	int8_t			idx		= -1;
	uint8_t			sdsId	= 0;
	uint8_t			type	= NOID;
	pid_t			pid		= -1;
} register_process_t;


const uint			REGISTER_SIZE 	= MAXCONFIG+1;
const uint 			str_buffer_len 	= 32;
const uint  		MAXQUESIZE		= 100;
typedef 			array< uint8_t, MAXQUESIZE>					deque_t ;
typedef				array<register_process_t, REGISTER_SIZE>	process_arr_t;
typedef 			Note_base::noteline_prefix_t				noteline_prefix_t;
typedef 			Spectrum_class::spectrum_t					spectrum_t;
typedef				Mixer_base::mixer_status_t					mixer_status_t;
typedef				Mixer_base::StA_amp_arr_t					StA_amp_arr_t;
typedef				Mixer_base::StA_state_arr_t					StA_state_arr_t;
typedef				array<Oscillator_base::connect_t, 3>		osc_connect_t;

constexpr process_arr_t init_process_arr()
{
	process_arr_t arr {};
	for( uint n = 0; n < REGISTER_SIZE; n++ )
	{
		arr[n] = register_process_struct();
	}
	return arr;
}
// there are MAXCONFIG interface structures in dataworld
// global values are managed in structure 0 only (master interface)
// local  values are interface specific
typedef struct interface_struct // with reasonable defaults
{
	// local (interface specific
	uint8_t			version						= 6; 						// comstack
	int8_t			SDS_Id						= 0;
	uint8_t			config						= 0; // reference to the Synthesizer sds

	StA_state_arr_t	StA_state 					{{ StA_state_struct() }};	// comstack
	StA_amp_arr_t	StA_amp_arr					{0,0,0,0,75,0,0,0};			// Instrument=75%
	mixer_status_t 	mixer_status 				= Mixer_base::mixer_status_struct(); // comstack

	bool			Keyboard					= false; // if tty and synthesizer process
	kbd_state_t		Kbd_state					= kbd_state_struct();

	char 			Instrument[str_buffer_len] 	{"default"}; //char array // comstack
	char 			Notes	  [str_buffer_len]	{"default"}; //char array for the notes filename // comstack
	char 			Other	  [str_buffer_len]	{"default"}; // e.g. external wave file name // comstack

	uint8_t			Master_Amp 					= 75;// comstack
	int8_t			mixer_balance				= 0; // nutral
	uint8_t		 	vol_slidemode 				= SLIDE;
	uint8_t			slide_duration 				= 100; // % of 4*max_seconds

	/* instrument definition starts */
	adsr_t 			OSC_adsr 					= adsr_struct();// comstack

	wave_t			OSC_wp						= wave_struct();
	wave_t			VCO_wp						= wave_struct();
	wave_t			FMO_wp						= wave_struct();

	spectrum_t 		OSC_spectrum				= Spectrum_class::spec_struct();// comstack
	spectrum_t 		VCO_spectrum 				= Spectrum_class::spec_struct();// comstack
	spectrum_t	 	FMO_spectrum 				= Spectrum_class::spec_struct();// comstack

	osc_connect_t	connect						= { { Oscillator_base::connect_struct() } };
	/* instrument definition ends	 */

	uint8_t			Spectrum_type				= Spectrum_class::spec_struct().osc;
	uint8_t			NotestypeId					= XML_ID; // musicxml
	noteline_prefix_t
					noteline_prefix				{ Note_base::noteline_prefix_struct() };
	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds // comstack

	uint8_t 		MIX_Amp 					= 0;// comstack
	uint8_t 		MIX_Id						= 0;// comstack


	uint8_t 		Synthesizer					= sdsstate_struct::DEFAULT;// indicates that shm is new // comstack

	uint8_t	 		FLAG						= CLEAR_KEY;
	uint8_t 		frq_slidermode				= SLIDE;	// comstack

	uint8_t		 	MODE						= sdsstate_struct::FREERUN;// comstack
	bool 			UpdateFlag 					= true;
	uint8_t			time_elapsed 				= 0;

	deque_t			deque						{ NULLKEY };
	eventptr_t		eventptr					= EventPtr_struct();
	char			eventstr[512]				{0};
	wd_status_t		WD_status					= WD_status_struct();

	// common
	buffer_t		audioframes					= audio_frames;
	wd_arr_t		wavedata 					= {};
	process_arr_t	process_arr					= init_process_arr(); //{ {register_process_struct()} };
	uint8_t 		SHMID 						= 0;// comstack
	uint8_t 		RecCounter					= 0;		// handshake data exchange// comstack
	bool			Record						= false; 	// Audioserver recording
	uint8_t 		FileNo						= 0;		// comstack
	uint8_t 		AudioServer	    			= sdsstate_struct::OFFLINE;// comstack
	uint8_t	 		UserInterface				= sdsstate_struct::OFFLINE;// comstack
	uint8_t	 		Composer 					= sdsstate_struct::OFFLINE;// comstack
	uint8_t			Comstack					= sdsstate_struct::OFFLINE;// NA
	uint8_t			Rtsp						= sdsstate_struct::OFFLINE;//



} interface_t;



#endif /* DATA_SHAREDDATASEGMENT_H_ */
