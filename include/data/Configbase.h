//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * Configbase.h
 *
 *  Created on: Oct 17, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_CONFIGBASE_H_
#define DATA_CONFIGBASE_H_

#include <Version.h>
#include <Ocean.h>
#include <String.h>
#include <Table.h>

#define OCEANDIR "OCEANDIR"
#define OCEANDEV "OCEANDEV"
static const string	oceandir_env= OCEANDIR;
static const string	oceandev_env= OCEANDEV;

static const Id_t 	MAXCONFIG 	= 4; 		// number of SDS configration
											// max 4 can be managed by the GUI
typedef	array<key_t,MAXCONFIG>	keys_arr_t;

struct prgarg_struct
{
	uint 		channel 	= 2;  		// -c
	uint 		rate 		= sample_rate;	// -r
	uint 		device 		= 0;		// -d
	uint 		ch_offs 	= 0; 		// -o
	uint 		MAXWAVFILES	= 5; // max numbers of file names generated automatically
	char		clear		= 'n';		// -X clear process array
	char 		test 		= 'n';		// -t run sanity check on classes and exit = 'y'
	char		debug		= 'n';
	char 		dialog		= 'n';		// -d dialog mode of the composer = 'y'
	char 		composer	= 'n';		// start rtsp with option -C
	char 		oceangui	= 'n';		// start rtsp with option -G
	string 		Genre		= "Alternative";
	string 		author		= "U.G.";
	string		title		= "Experimental";
	string		album		= Application;
	string		Term		= "xterm -e ";
	string		Nohup		= "nohup";
	string 		ffmpeg 		= "ffmpeg";
	string		installdir	= "";		 // Setup -I ...
	string		filename	= "";
	key_t		Sem_key		= 0x9999;
	key_t 		SHM_key 	= 0x100; 	// -k
	key_t		SHM_keyl	= SHM_key;
	key_t		SHM_keyr	= SHM_key+1;
	key_t		SDS_key		= 0x6666;
	keys_arr_t 	sdskeys 	{};
	uint		temp_sec	= tmpduration; 	// seconds storage in StA
	uint 		record_sec	= recduration; 	// seconds storage
	uint		kbd_sec		= kbdduration;	// seconds of keyboard stoarage in StA

} ;
typedef prgarg_struct prgarg_t;


enum APPID
{
	AUDIOID = 0, // mandatory
	SYNTHID = 1, // fixed
	COMPID,
	GUI_ID,
	SDSVIEWID,
	RTSPID,
	KEYBOARDID,
	SETUPID,
	TESTPRGID,
	XMLEDITORID,
	NoAPPID
};

#define AUDIOSERVER "AudioServer"
#define SYNTHESIZER	"Synthesizer"
#define COMPOSER	"Composer"
#define OCEANGUI	"OceanGUI"
#define SDSVIEW		"SDSview"
#define RTSP		"Rtsp"
#define SYNTHKBD	"Keyboard"
#define SETUP		"Setup"
#define TESTPRG		"Testprj"
#define XMLEDITOR	"musescore3"


struct AppMap_struct
{

	map<APPID, string> AppIdName_map // @suppress("Invalid arguments")
	{
			{ AUDIOID		, AUDIOSERVER },
			{ SYNTHID		, SYNTHESIZER},
			{ COMPID		, COMPOSER},
			{ GUI_ID		, OCEANGUI},
			{ SDSVIEWID		, SDSVIEW},
			{ RTSPID		, RTSP},
			{ KEYBOARDID	, SYNTHKBD},
			{ SETUPID		, SETUP},
			{ TESTPRGID		, TESTPRG},
			{ XMLEDITORID	, XMLEDITOR},
			{ NoAPPID		, "unknown id"},
	};
	map< string, APPID >AppNameId_map = // @suppress("Invalid arguments")
			imap<APPID, string>( AppIdName_map ); // @suppress("Invalid arguments")
	vector<APPID> AppIds{};



	AppMap_struct()
	{
		AppIds = Iota_T<APPID>( (APPID)0, NoAPPID );
	}
	~AppMap_struct() = default;
	string AppIdName( APPID appid )
	{
		return AppIdName_map[ appid ];
	}
	APPID AppNameId( string name )
	{
		return AppNameId_map[ name ];
	}

};
const vector<APPID> AppIds = AppMap_struct().AppIds;

struct process_properties_struct
{
	int			pid				= NoPID;
	bool		start_once		= true;
	bool		data_process 	= false;
	bool		logowner 		= false;
	bool		keyboard		= false;
};

typedef struct process_struct :
	process_properties_struct,
	AppMap_struct
{
	const string 	name 		= program_invocation_short_name;
	const APPID		AppId 		= AppNameId( name );
	process_struct( ) :
		process_properties_struct()
	{	};

	virtual ~process_struct() 	= default;
	void Show()
	{
		Table_class Table { name +" properties", LOGINDENT };
		Table.AddColumn("Property"		, 20);
		Table.AddColumn("Value"			, 20);
		Table.PrintHeader();
		Table.AddRow("Application id"	, (int)AppId );
		Table.AddRow("Name"				, name );
		Table.AddRow("Process id"		, pid );
		Table.AddRow("Start once"		, start_once );
		Table.AddRow("Data process"		, data_process );
		Table.AddRow("Logfile owner"	, logowner );
		Table.AddRow("Keyboard"			, keyboard );
	}
} process_t;



#endif /* DATA_CONFIGBASE_H_ */
