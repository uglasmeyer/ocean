/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Notesbase.h>
#include <Oscbase.h>
#include <keys.h>
#include <synthmem.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <version.h>
#include <common.h>
#include <Synthesizer.h>


const vector_str_t uint8_code_str =
{
	"Offline"		,
	"Running"		,
	"Recording"		,
	"Stop record"	,
	"synchronize"	,
	"free running"	,
	"block"			,
	"release"		,
	"Update GUI"	,
	"Store sound"	,
	"sync mode"		,
	"default mode"	,
	"Exit server"	,
	"Keyboard"		,
	"Update mode"
};

enum {
	 OFFLINE 	,
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

typedef 		Spectrum_base::spectrum_t
					spec_struc_t;
static const 	uint str_buffer_len = 32;

typedef struct interface_struct
{
	uint8_t		version						= 1;
	array<StA_status_t, 8>
				StA_status 					{ StA_status_struct() };
	mixer_status_t
				mixer_status 				= mixer_status_struct();
	char 		Instrument[str_buffer_len] 	= "default"; //char array
	char 		Notes[str_buffer_len] 		= "default"; //char array for the notes filename
	char 		Other[str_buffer_len]		= ""; // e.g. external wave file name

	float 		Main_Freq  					= 110;
	uint8_t 		Master_Amp 					= 75;
	uint8_t 		Main_Duration 				= max_sec;
	adsr_t Main_adsr 					= adsr_struct();
	Note_base::noteline_prefix_t
				noteline_prefix				= Note_base::noteline_prefix_struct();
	uint8_t		Soft_freq					= 0;

	uint8_t 		LOOP_step 					= 0;
	float		LOOP_end 					= 75;

	float	 	VCO_Freq 					= 0;
	uint8_t 		VCO_Amp 					= 0;
	uint8_t 		PMW_dial 					= 50; // PMW_dial

	float	 	FMO_Freq 					= 0;
	uint8_t 		FMO_Amp 					= 0;

	uint8_t		Noteline_sec 				= 0; // duration of notes to play given in seconds
	pid_t		Audio_pid					= 0;

	uint8_t 		MIX_Amp 					= 0;
	uint8_t 		MIX_Id						= 0;
	spec_struc_t MAIN_spectrum				= Spectrum_base::spec_struct();
	spec_struc_t VCO_spectrum 				= Spectrum_base::spec_struct();
	spec_struc_t FMO_spectrum 				= Spectrum_base::spec_struct();
	uint8_t		Spectrum_type				= MAINID;

	uint8_t 		Wavedisplay_Id				= 0;
	uint8_t 		AudioServer	    			= OFFLINE;
	uint8_t 		Synthesizer					= DEFAULT; // indicates that shm is new
	uint8_t 		UserInterface				= OFFLINE;
	uint8_t 		Composer 					= OFFLINE;
	uint8_t		Comstack					= OFFLINE;

	uint8_t 		FLAG						= NULLKEY;
	uint8_t 		KEY 						= NULLKEY;
	uint8_t 		RecCounter					= 0;	// handshake data exchange
	uint8_t 		SHMID 						= 0;
	uint8_t 		MODE						= FREERUN;
	bool 		UpdateFlag 					= true;
	uint16_t	WD_type_ID 					= FULLID;
	uint8_t 		FileNo						= 0;
	array<Data_t, wavedisplay_len>
				wavedata 					= {0};
} ifd_t;





class Interface_class : virtual Logfacility_class
{
public:


	const key_t 			shm_key			= 5166529;


	ifd_t 					ifd_data;
	ifd_t* 					addr			= NULL;

	Interface_class();
	virtual ~Interface_class();

	void 	Write_str( char, string );
	string 	Read_str( char );
	void 	Commit();
	void 	Update( char );
	void 	Show_interface();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd();
	void 	Announce( string, bool );
	void 	Set( bool& key, bool value);
	void 	Set( uint8_t& key, uint8_t value);
	void 	Set( uint16_t& key, uint16_t value);
	void 	Set( float& key, float value);

private:
	uint8_t			client_id 		= 0xFF;
	Spectrum_class	GUIspectrum 	{};
	vector<string>	Waveform_vec	{};

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
};



#endif /* GUIINTERFACE_H_ */
