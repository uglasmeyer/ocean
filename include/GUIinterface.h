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
			"Offline",
			"Running",
			"stop Audio",
			"Running",
			"Recording",
			"Stop record",
			"synchronize",
			"free running",
			"block",
			"release",
			"Update GUI",
			"Store sound",
			"play notes",
			"default mode",
			"Exit server"
};
const uint8_t NOCONTROL 	= 	0;
const uint8_t RUNNING	 	= 	1;
const uint8_t STOPSNDSRV 	=	2;
const uint8_t RUNSNDSRV 	=	3;
const uint8_t RECORD		=	4;
const uint8_t STOPRECORD    =	5;
const uint8_t SENDDATA 		= 	6;
const uint8_t FREERUN		= 	7;
const uint8_t BLOCKDATA		=	8;
const uint8_t RELEASEDATA 	= 	9;
const uint8_t UPDATEGUI 	=  10;
const uint8_t STORESOUND	=  11; // obsolete
const uint8_t NOTES 		=  12;
const uint8_t DEFAULT		=  13;
const uint8_t EXITSERVER 	=  14;

static const uint str_buffer_len = 32;

// Waveform counter
const uint16_t MAINWF_MASK  = 0xFFFF - 0b111;
const uint 	   MAINWF_SHIFT	= 0;
const uint16_t VCOWF_MASK 	= 0xFFFF - 0b111000;
const uint 	   VCOWF_SHIFT 	= 3;
const uint16_t FMOWF_MASK 	= 0xFFFF - 0b111000000;
const uint 	   FMOWF_SHIFT 	= 6;


typedef struct interface_struct
{
	char		version						= 8;
	array<ma_status_t, 8> 	ma_status 		{ {{false, false}}};
	mi_status_t				mi_status 		{ false, false, false, false, false };
	char 		Instrument[str_buffer_len] 	= "default"; //char array
	char 		Notes[str_buffer_len] 		= "default"; //char array for the notes filename
	char 		Other[str_buffer_len]		= ""; // e.g. external wave file name

	uint16_t 	Main_Freq  					= 110;
	char 		Master_Amp 					= 75;
	char 		Main_Duration 				= max_sec;
	char 		Main_ID						= MAINID;
	char 		Main_waveform_id 			= 0;
	char  		Main_adsr_bps_id			= 0;
	char 		Main_adsr_decay  			= 0;
	char 		Main_adsr_attack    		= 0;
	char		Soft_freq					= 0;
	char 		Main_adsr_hall				= 0;

	uint16_t	LOOP_step 					= 0;
	uint16_t	LOOP_end 					= 75;

	uint16_t 	VCO_Freq 					= 0;
	char 		VCO_Amp 					= 0;
	char 		VCO_waveform_id				= 0;
	char		VCO_ID						= VCOID;
	char 		PMW_dial 					= 50; // PMW_dial

	uint16_t 	FMO_Freq 					= 0;
	char 		FMO_Amp 					= 0;
	char		FMO_ID						= FMOID;
	char 		FMO_waveform_id 			= 0;

	char		Noteline_sec 				= 0; // duration of notes to play given in seconds
	pid_t		Audio_pid					= 0;

	char 		MIX_Amp 					= 0;
	char 		MIX_Id						= 0;
	char		Spectrum_type				= MAINID;
	char		Spectrum_id 				= 0;
	char 		Spectrum_value				= 0;
	char 		Spectrum_channel			= 0;

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

typedef struct shm_info_struct
{
	buffer_t 	size;
	key_t		key;
	int 		id;
	void* 		addr;
} shm_info_t;



class GUI_interface_class : virtual Logfacility_class
{
public:

	GUI_interface_class();
	virtual ~GUI_interface_class();

	const key_t 			shm_key			= 5166529;


	ifd_t 					ifd_data;
	ifd_t* 					addr			= NULL;
	shm_info_t				shm_info;

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


	class Counter_class : virtual public Logfacility_class
	{
	public:
		Counter_class()
		: Logfacility_class("Counter")
		{
		};
		Counter_class(uint16_t base, uint8_t bits, uint8_t pos )
		: Logfacility_class("Counter")
		{
			// bits: length of the bit patition
			// pos: position of bits in base
			setup( base, bits, pos);
		};
		Counter_class(uint16_t base, uint8_t bits, uint8_t pos, uint8_t modulo )
		: Logfacility_class("Counter")
		{
			// bits: length of the bit patition
			// pos: position of bits in base
			setup( base, bits, pos, modulo );
		};
		virtual ~Counter_class(){};
		uint16_t	inc_counter( uint16_t );
		void	 	set_counter( uint16_t );
		uint16_t 	get_counter( uint16_t );
		void 		test(  );
		void 		setup( uint16_t, uint8_t, uint8_t );
		void 		setup( uint16_t, uint8_t, uint8_t, uint8_t );

private:
	uint8_t 	modulo{0};
	uint16_t 	value{0};
	uint16_t	mask{0xFFFF};
	uint16_t 	shift{0};


	};
//	Counter_class WF_counter{ifd_data.Waveform_counter, 3, 0};

private:
	uint8_t			client_id 		= 0xFF;
	Spectrum_class	GUIspectrum 	{};
	vector<string>	Waveform_vec	{};

	void*	buffer( buffer_t, key_t );
};



#endif /* GUIINTERFACE_H_ */
