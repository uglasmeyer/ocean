/*
 * Appstate.h
 *
 *  Created on: Mar 30, 2025
 *      Author: sirius
 */

#ifndef DATA_APPSTATE_H_
#define DATA_APPSTATE_H_

#include <Logfacility.h>
#include <data/SharedDataSegment.h>

template<typename T>
constexpr string AppIdName( const T& app_id )
{
	switch ( app_id )
	{
		case APPID::AUDIOID		: return "Audioserver";
		case APPID::SYNTHID		: return "Synthesizer";
		case APPID::COMPID		: return "Composer";
		case APPID::GUI_ID		: return "UserInterface";
		case APPID::COMSTACKID	: return "Comstack";
		case APPID::RTSPID		: return "Rtsp";
		case APPID::TESTID		: return "Testprg";
		case APPID::NOID		: return "No Process";
		default 		: 	{
							cout << "WARN: unknown application id: " << app_id << endl;
							return "No Process";
							};
	}
}

class Appstate_class :
	Logfacility_class
{
	string 			className 	= "";
	uint 			AppId;
	interface_t* 	sds = nullptr;

public:
	string		Name ;
	uint8_t* 	ptr = 0;

	Appstate_class( uint id ) :
		Logfacility_class("Appstate_class")
	{
		className 	= Logfacility_class::className;
		AppId 		= id;
		Name 		= AppIdName( id );
	};

	~Appstate_class() = default;

	void 		Setup( interface_t* _sds );
	uint8_t* 	State_pMap( interface_t* sds );
	void 		Announce( );

};



#endif /* DATA_APPSTATE_H_ */
