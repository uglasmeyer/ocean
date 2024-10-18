/*
 * SharedDataSegment.h
 *
 *  Created on: Oct 1, 2024
 *      Author: sirius
 */

#ifndef DATA_SHAREDDATASEGMENT_H_
#define DATA_SHAREDDATASEGMENT_H_
#include <Ocean.h>
#include <Mixerbase.h>
#include <Oscbase.h>
#include <Wavedisplay_base.h>
#include <Notesbase.h>
#include <Keys.h>

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

typedef struct process_struct
{
	uint8_t			sdsId	= 0;
	uint8_t			type	= NOID;
	buffer_t		size	= 0;
} process_t;

const uint	REGISTER_SIZE 	= MAXCONFIG+1;
const uint 	str_buffer_len 	= 32;

typedef		array<process_t, REGISTER_SIZE>	process_arr_t;
typedef 	Note_base::noteline_prefix_t	noteline_prefix_t;
typedef 	Spectrum_base::spectrum_t		spectrum_t;
typedef		Mixer_base::mixer_status_t		mixer_status_t;
typedef		Mixer_base::StA_amp_arr_t		StA_amp_arr_t;
typedef		Mixer_base::StA_state_arr_t		StA_state_arr_t;

typedef struct interface_struct
{
	uint8_t			version						= 0; 						// comstack
	int				SDS_Id						= -1;
	StA_state_arr_t	StA_state 					{{ StA_status_struct() }};	// comstack
	StA_amp_arr_t	StA_amp_arr					{{ 75 }};
	mixer_status_t 	mixer_status 				= Mixer_base::mixer_status_struct(); // comstack
	char 			Instrument[str_buffer_len] 	= "default"; //char array // comstack
	char 			Notes[str_buffer_len] 		= "default"; //char array for the notes filename // comstack
	char 			Other[str_buffer_len]		= ""; // e.g. external wave file name // comstack

	float 			Main_Freq  					= 110;// comstack
	uint8_t			Master_Amp 					= 75;// comstack
	uint8_t			Main_Duration 				= max_sec;// comstack
	adsr_t 			Main_adsr 					= adsr_struct();// comstack
	noteline_prefix_t
					noteline_prefix				= Note_base::noteline_prefix_struct();
	uint8_t			Soft_freq					= 0;// comstack

	uint8_t		 	LOOP_step 					= 0;
	float			LOOP_end 					= 75;

	float	 		VCO_Freq 					= 0;// comstack
	uint8_t 		VCO_Amp 					= 0;// comstack
	uint8_t 		PMW_dial 					= 50; // PMW_dial // comstack

	float	 		FMO_Freq 					= 0;// comstack
	uint8_t 		FMO_Amp 					= 0;// comstack

	uint8_t			Noteline_sec 				= 0; // duration of notes to play given in seconds // comstack
	uint8_t			config						= 0;
	keys_arr_t		sds_keys					= {0,0,0,0};

	uint8_t 		MIX_Amp 					= 0;// comstack
	uint8_t 		MIX_Id						= 0;// comstack
	spectrum_t 		MAIN_spectrum				= Spectrum_base::spec_struct();// comstack
	spectrum_t 		VCO_spectrum 				= Spectrum_base::spec_struct();// comstack
	spectrum_t	 	FMO_spectrum 				= Spectrum_base::spec_struct();// comstack
	uint8_t			Spectrum_type				= MAINID;

	uint8_t	 		Wavedisplay_Id				= AUDIOOUT; // Audio out
	uint8_t 		AudioServer	    			= OFFLINE;// comstack
	uint8_t 		Synthesizer					= DEFAULT;// indicates that shm is new // comstack
	uint8_t	 		UserInterface				= OFFLINE;// comstack
	uint8_t	 		Composer 					= OFFLINE;// comstack
	uint8_t			Comstack					= OFFLINE;// NA
	uint8_t			Rtsp						= OFFLINE;//

	uint8_t	 		FLAG						= NULLKEY;
	uint8_t 		KEY 						= NULLKEY;// comstack
	uint8_t 		RecCounter					= 0;	// handshake data exchange// comstack
	bool			Record						= false; // Audioserver recording
	uint8_t 		SHMID 						= 0;// comstack
	uint8_t		 	MODE						= FREERUN;// comstack
	bool 			UpdateFlag 					= true;
	uint16_t		WD_type_ID 					= FULLID;
	uint8_t 		FileNo						= 0;// comstack
	uint8_t			time_elapsed 				= 0;
	process_arr_t	process_arr					= { {process_struct()} };

	wd_arr_t		wavedata 					= {0};
} interface_t;






#endif /* DATA_SHAREDDATASEGMENT_H_ */
