/*
 * Appstate.h
 *
 *  Created on: Mar 30, 2025
 *      Author: sirius
 */

#ifndef DATA_APPSTATE_H_
#define DATA_APPSTATE_H_

#include <bits/stdint-uintn.h>
#include <data/SharedDataSegment.h>
#include <Logfacility.h>
#include <Ocean.h>
#include <sys/types.h>
#include <Table.h>
#include <array>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <data/Register.h>




template<typename T>
constexpr char AppNameId( const T& name )
{
	char appid = APPID::NOID;

	if ( strEqual( name, AUDIOSERVER	) ) appid = APPID::AUDIOID;
	if ( strEqual( name, SYNTHESIZER 	) )	appid = APPID::SYNTHID;
	if ( strEqual( name, COMPOSER 		) )	appid = APPID::COMPID;
	if ( strEqual( name, OCEANGUI	 	) ) appid = APPID::GUI_ID;
	if ( strEqual( name, COMSTACK	 	) )	appid = APPID::COMSTACKID;
	if ( strEqual( name, SYNTHKBD 		) )	appid = APPID::KBDID;
	if ( strEqual( name, SETUP		 	) )	appid = APPID::SETUPID;
	if ( strEqual( name, RTSP	 		) )	appid = APPID::RTSPID;
	if ( strEqual( name, TESTPRG 		) )	appid = APPID::TESTID;

	if ( appid == APPID::NOID )
	{
		cout << "ERROR: unknown application name: " << name << endl;
		exit( 0 );
	}
	if ( ( not is_atty ) and ( appid == APPID::KBDID ) )
	{
		appid = APPID::SYNTHID;
	}
	return appid;
}

struct process_properties_struct
{
	bool		start_once		= true;
	bool		data_process 	= false;
	bool		logowner 		= false;
	bool		keyboard		= false;
};

typedef struct process_struct :
		process_properties_struct
{
	const string 	name 		= program_invocation_short_name;
	const char		AppId 		= AppNameId( program_invocation_short_name );
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
		Table.AddRow("Start once"		, start_once );
		Table.AddRow("Data process"		, data_process );
		Table.AddRow("Logfile owner"	, logowner );
		Table.AddRow("Keyboard"			, keyboard );
	}
} process_t;

class Appstate_class :
	virtual public Logfacility_class,
	virtual public sdsstate_struct

{
public:
	string 			className 			= "";
	string			Name 				= "";
	uint 			AppId				;
	Register_class* Reg_p				;
	interface_t* 	sds 				= nullptr;
	interface_t* 	sds_master			= nullptr;
	const set<int> 	startonceIds 		{ AUDIOID, GUI_ID, RTSPID, COMPID, COMSTACKID, KBDID } ;
	const range_T<uint>
					appId_range 		{0, NOID };
	array< uint, NOID>
					backup_state		{ };
					Appstate_class		( 	char appid,
											interface_t* _sds,
											interface_t* _sds_master,
											Register_class* reg
										);

	virtual			~Appstate_class		() = default;

	void 			Setup				( interface_t* _sds, interface_t* _sds_master );
	void 			Announce			( );
	bool 			StartOnceOk			( interface_t* sds );

	void			Set					( interface_t* sds, uint appid, int state );
	int				Get					( interface_t* sds, uint appid );
	string			GetStr				( interface_t* sds, uint appid );

	bool			IsRunning  			( interface_t* sds, uint appid );
	bool			IsOffline  			( interface_t* sds, uint appid );
	bool 			IsExitserver		( interface_t* sds, uint appid );
	bool			IsKeyboard			( );

	void 			SetRunning			( );
	void 			SetOffline			( );
	void 			SetExitserver		( interface_t* sds, uint appid );

	void			SaveState			( );
	void			RestoreState		( );
	bool 			IsInconsistent		( interface_t* sds, char appid );

	void			Shutdown_all		(  vector<interface_t*> sds_vec );

private:
	void  			shutdown			( interface_t* sds, char appid );
	uint8_t* 		appAddr				( interface_t* sds, uint appid );
};



#endif /* DATA_APPSTATE_H_ */
