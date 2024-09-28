/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Ocean.h>

enum {
	 OFFLINE,
	 RUNNING	,
	 RECORD		,
	 STOPRECORD ,
	 SENDDATA 	,
	 FREERUN	,
	 BLOCKDATA	,
	 RELEASEDATA,
	 UPDATEGUI 	,
	 STORESOUND	,   // obsolete
	 SYNC 		,
	 DEFAULT	,
	 EXITSERVER	,
	 KEYBOARD	,
	 UPDATE		,
	 LASTNUM
};
static const uint CODE_MAP_SIZE = LASTNUM;


#include <Config.h>
#include <Notesbase.h>
#include <Keys.h>
#include <Synthmem.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <Version.h>
#include <Mixerbase.h>
#include <Oscbase.h>
#include <Wavedisplay_base.h>
#include <Semaphore.h>

using namespace Storage;
typedef 		Note_base::noteline_prefix_t	noteline_prefix_t;
typedef 		Spectrum_base::spectrum_t		spectrum_t;
typedef			Mixer_base::mixer_status_t		mixer_status_t;
typedef			Mixer_base::StA_amp_arr_t		StA_amp_arr_t;
typedef			Mixer_base::StA_state_arr_t		StA_state_arr_t;

static const 	uint str_buffer_len = 32;

typedef struct interface_struct
{
	uint8_t			version						= 0; 						// comstack
	StA_state_arr_t	StA_state 					{ StA_status_struct() };	// comstack
	StA_amp_arr_t	StA_amp_arr					{ 75 };
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
	pid_t			Audio_pid					= 0;

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
	uint8_t 		SHMID 						= 0;// comstack
	uint8_t		 	MODE						= FREERUN;// comstack
	bool 			UpdateFlag 					= true;
	uint16_t		WD_type_ID 					= FULLID;
	uint8_t 		FileNo						= 0;// comstack
	uint8_t			time_elapsed 				= 0;
	wd_arr_t		wavedata 					= {0};
} ifd_t;





class Interface_class : virtual public Logfacility_class, Config_class
{
public:

	ifd_t 					ifd_data;
	ifd_t* 					addr			= NULL;

	Interface_class();
	virtual ~Interface_class();

	void 	Write_arr( const wd_arr_t& arr );
	void 	Write_str( char, string );
	string 	Read_str( char );
	void 	Commit();
	void 	Update( char );
	void 	Show_interface();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd();
	void 	Announce( uint, uint8_t* );
	void 	Set( bool& key, bool value);
	void 	Set( uint8_t& key, uint8_t value);
	void 	Set( uint16_t& key, uint16_t value);
	void 	Set( float& key, float value);
	string 	Decode( uint8_t idx );

private:
	uint8_t			client_id 		= 0xFF;
	Semaphore_class	SEM{};
	Spectrum_base	GUIspectrum 	{};
	vector<string>	Waveform_vec	{};
	char 			previous_status = OFFLINE;
	array<string, CODE_MAP_SIZE>
					code_str_arr;


	typedef struct shm_info_struct
	{
		buffer_t 	size;
		key_t		key;
		int 		id;
		void* 		addr;
	} shm_info_t;
	shm_info_t				shm_info;
	Spectrum_base			Spectrum 		{};
	void*	buffer( buffer_t, key_t );
	bool 	reject(char status, int id );
	void 	setup_code_arr();
};



#endif /* GUIINTERFACE_H_ */
