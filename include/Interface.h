/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Oscbase.h>
#include <keys.h>
#include <synthesizer.h>
#include <synthmem.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <version.h>


const vector_str_t uint8_code_str =
{
	"Offline"	,
	"Running"	,
	"stop Audio",
	"Running"	,
	"Recording"	,
	"Stop record",
	"synchronize",
	"free running",
	"block"		,
	"release"	,
	"Update GUI",
	"Store sound",
	"play notes",
	"default mode",
	"Exit server"
};

enum {
	 NOCONTROL 	,
	 RUNNING	,
	 STOPSNDSRV ,
	 RUNSNDSRV 	,
	 RECORD		,
	 STOPRECORD ,
	 SENDDATA 	,
	 FREERUN	,
	 BLOCKDATA	,
	 RELEASEDATA,
	 UPDATEGUI 	,
	 STORESOUND	,   // obsolete
	 NOTES 		,
	 DEFAULT	,
	 EXITSERVER
};


static const uint str_buffer_len = 32;



typedef struct interface_struct
{
	char		version						= 0;
	array<ma_status_t, 8> 	ma_status 		{ {{false, false}}};
	mi_status_t				mi_status 		{ false, false, false, false, false };
	char 		Instrument[str_buffer_len] 	= "default"; //char array
	char 		Notes[str_buffer_len] 		= "default"; //char array for the notes filename
	char 		Other[str_buffer_len]		= ""; // e.g. external wave file name

	float 		Main_Freq  					= 110;
	char 		Master_Amp 					= 75;
	char 		Main_Duration 				= max_sec;
	char  		Main_adsr_bps_id			= 0;
	char 		Main_adsr_decay  			= 0;
	char 		Main_adsr_attack    		= 0;
	char		Soft_freq					= 0;
	char 		Main_adsr_hall				= 0;

	char 		LOOP_step 					= 0;
	float		LOOP_end 					= 75;

	float	 	VCO_Freq 					= 0;
	char 		VCO_Amp 					= 0;
	char 		PMW_dial 					= 50; // PMW_dial

	float	 	FMO_Freq 					= 0;
	char 		FMO_Amp 					= 0;

	char		Noteline_sec 				= 0; // duration of notes to play given in seconds
	pid_t		Audio_pid					= 0;

	char 		MIX_Amp 					= 0;
	char 		MIX_Id						= 0;
	spec_struct_t MAIN_spectrum 			= spec_struct();
	spec_struct_t VCO_spectrum 				= spec_struct();
	spec_struct_t FMO_spectrum 				= spec_struct();
	char		Spectrum_type				= MAINID;

	char 		Wavedisplay_Id				= 0;
	char 		AudioServer	    			= NOCONTROL;
	char 		Synthesizer					= DEFAULT; // indicates that shm is new
	char 		UserInterface				= NOCONTROL;
	char 		Composer 					= NOCONTROL;
	char		Comstack					= NOCONTROL;

	char 		FLAG						= NULLKEY;
	char 		KEY 						= NULLKEY;
	char 		RecCounter					= 0;	// handshake data exchange
	char 		SHMID 						= 0;
	char 		MODE						= FREERUN;
	bool 		UpdateFlag 					= true;
	uint16_t	WD_type_ID 					= 0x0;
	char 		FileNo						= 0;
	array<Data_t, wavedisplay_len>
				wavedata 					= {0};
} ifd_t;



class GUI_interface_class : virtual Logfacility_class
{
public:

	GUI_interface_class();
	virtual ~GUI_interface_class();

	const key_t 			shm_key			= 5166529;


	ifd_t 					ifd_data;
	ifd_t* 					addr			= NULL;

	void 	write_str( char, string );
	string 	read_str( char );
	void 	commit();
	void 	update( char );
	void 	show_GUI_interface();
//	string 	decode(uint );
	void	dump_ifd();
	bool 	restore_ifd();
	void 	reset_ifd();
	void 	announce( string, bool );
	void 	Set( bool& key, bool value);
	void 	Set( char& key, char value);
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
