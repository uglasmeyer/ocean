/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

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
	uint 		MAXWAVFILES	= 5; 		// max numbers of file names generated automatically
	char 		test 		= 'n';		// -t run sanity check on classes and exit = 'y'
	char		debug		= 'n';		// -v verboose output
	char 		dialog		= 'n';		// -d dialog mode of the composer = 'y'
	char 		composer	= 'n';		// start rtsp with option -C
	char 		oceangui	= 'n';		// start rtsp with option -G
	char		archive		= 'n';		// setup creates no binary archive
	char		Deploy		= 'n';		// -Y deployment workflow
	string 		Genre		= "Alternative";
	string 		author		= "U.G.";
	string		title		= "Experimental";
	string		album		= Application;
	string		Term		= "xterm -e ";
	string		Nohup		= "nohup";
	string 		ffmpeg 		= "ffmpeg";
	string		installdir	= "";		 // Setup -I ...
	string		sourcedir	= "";		 // Setup -S ...
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
const vector<APPID> AppIds		= AppMap_struct().AppIds;

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
