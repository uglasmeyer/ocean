/*
 * Appstate.h
 *
 *  Created on: May 4, 2025
 *      Author: sirius
 */

#ifndef VIEWINTERFACE_H_
#define VIEWINTERFACE_H_

#include <data/DataWorld.h>
#include <data/Interface.h>
#include <data/Appstate.h>
#include <Adsr.h>
#include <data/Device.h>

class ViewInterface_class
	: virtual public 	Logfacility_class
	, virtual 			osc_struct
	, virtual 			sdsstate_struct
	, virtual 			interface_struct
	, virtual			Device_class
	, virtual public	ADSR_class
{
	string 				className 	= "";
	Interface_class*	Sds_p 		= nullptr;
	interface_t* 		sds_p 		= nullptr;

public:
	Appstate_class		Appstate	;
	Dataworld_class*	DaTA;

			ViewInterface_class( char appid, Dataworld_class* DaTA );
	virtual ~ViewInterface_class() = default;


	void	ShowPage( interface_t* sds, int nr );

private:

	void 	showOSCs();
	void 	showProcesses();
	void 	showStates() ;

	void 	show_Que();
	void 	show_Ipc();
	void 	show_spectrum();

	void 	show_Adsr();
	void 	showKeys();
	void 	show_manage();


	void	printHeader();
	string	Decode( Id_t idx);
	void 	selfTest();

};





#endif /* VIEWINTERFACE_H_ */
