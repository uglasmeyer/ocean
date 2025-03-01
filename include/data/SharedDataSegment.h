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
#include <notes/Notesbase.h>

enum {
	 OFFLINE,
	 RUNNING	,
	 FREERUN	,
	 UPDATEGUI 	,
	 SYNC 		,
	 DEFAULT	,
	 EXITSERVER	,
	 KEYBOARD	,
	 RECORDSTART,
	 RECORDSTOP,
	 LASTNUM
};
typedef struct EventPtr_struct
{
	uint8_t 		first 	= 0;
	uint8_t 		last 	= 0;
	uint8_t			length	= 0;
} eventptr_t;

typedef struct process_struct
{
	int8_t			idx		= -1;
	uint8_t			sdsId	= 0;
	uint8_t			type	= NOID;
	pid_t			pid		= -1;
} process_t;

const uint	REGISTER_SIZE 	= MAXCONFIG+1;
const uint 	str_buffer_len 	= 32;
const uint  MAXQUESIZE		= 100;
typedef 	array< uint8_t, MAXQUESIZE>		deque_t ;
typedef		array<process_t, REGISTER_SIZE>	process_arr_t;
typedef 	Note_base::noteline_prefix_t	noteline_prefix_t;
typedef 	Spectrum_class::spectrum_t		spectrum_t;
typedef		Mixer_base::mixer_status_t		mixer_status_t;
typedef		Mixer_base::StA_amp_arr_t		StA_amp_arr_t;
typedef		Mixer_base::StA_state_arr_t		StA_state_arr_t;

typedef struct interface_struct
{
	uint8_t			version						= 0; 						// comstack
	int8_t			SDS_Id						= 0;
	buffer_t		audioframes					= max_frames;

	StA_state_arr_t	StA_state 					{{ StA_status_struct() }};	// comstack
	StA_amp_arr_t	StA_amp_arr					{{0}};
	mixer_status_t 	mixer_status 				= Mixer_base::mixer_status_struct(); // comstack

	char 			Instrument[str_buffer_len] 	{"default"}; //char array // comstack
	char 			Notes	  [str_buffer_len]	{"default"}; //char array for the notes filename // comstack
	char 			Other	  [str_buffer_len]	{"default"}; // e.g. external wave file name // comstack

	uint8_t			Master_Amp 					= 75;// comstack
	uint8_t		 	LOOP_step 					= 0;
	int				LOOP_end 					= 75;

	/* instrument definition starts */
	adsr_t 			OSC_adsr 					= adsr_struct();// comstack

	wave_t			OSC_wp						= wave_struct();
	wave_t			VCO_wp						= wave_struct();
	wave_t			FMO_wp						= wave_struct();

	spectrum_t 		OSC_spectrum				= Spectrum_class::spec_struct();// comstack
	spectrum_t 		VCO_spectrum 				= Spectrum_class::spec_struct();// comstack
	spectrum_t	 	FMO_spectrum 				= Spectrum_class::spec_struct();// comstack
	/* instrument definition ends	 */

	uint8_t			Spectrum_type				= Spectrum_class::spec_struct().osc;
	uint8_t			NotestypeId					= 0; // musicxml
	noteline_prefix_t
					noteline_prefix				{ Note_base::noteline_prefix_struct() };
	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds // comstack

	uint8_t			config						= 0; // reference to the Synthesizer sds
	keys_arr_t		sds_keys					= { 0,0,0,0 };

	uint8_t 		MIX_Amp 					= 0;// comstack
	uint8_t 		MIX_Id						= 0;// comstack


	uint8_t 		AudioServer	    			= OFFLINE;// comstack
	uint8_t 		Synthesizer					= DEFAULT;// indicates that shm is new // comstack
	uint8_t	 		UserInterface				= OFFLINE;// comstack
	uint8_t	 		Composer 					= OFFLINE;// comstack
	uint8_t			Comstack					= OFFLINE;// NA
	uint8_t			Rtsp						= OFFLINE;//

	uint8_t	 		FLAG						= NULLKEY;
	uint8_t 		slidermode					= FIXED;	// comstack

	uint8_t 		RecCounter					= 0;		// handshake data exchange// comstack
	bool			Record						= false; 	// Audioserver recording
	uint8_t 		FileNo						= 0;		// comstack

	uint8_t 		SHMID 						= 0;// comstack
	uint8_t		 	MODE						= FREERUN;// comstack
	bool 			UpdateFlag 					= true;
	uint8_t			time_elapsed 				= 0;
	process_arr_t	process_arr					= { {process_struct()} };

	deque_t			deque						{ NULLKEY };
	eventptr_t		eventptr					= EventPtr_struct();
	wd_status_t		WD_status					= WD_status_struct();
	wd_arr_t		wavedata 					= {0};

} interface_t;




#endif /* DATA_SHAREDDATASEGMENT_H_ */
