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

class ViewInterface_class :
	virtual 			osc_struct,
	virtual 			interface_struct,
	virtual public		ADSR_class,
	virtual 			sdsstate_struct
{
	string 				className 	= "";
	Interface_class*	Sds_p 		= nullptr;
	interface_t* 		sds 		= nullptr;

public:
	Appstate_class		Appstate	;
	Dataworld_class*	DaTA;

	ViewInterface_class( char appid, Dataworld_class* DaTA )
	:
		ADSR_class(),
		sdsstate_struct(),
		Appstate( appid, nullptr, DaTA->Sds_p->addr, &DaTA->Reg )
	{
		className 		= Logfacility_class::className;
		this->DaTA		= DaTA;
		Sds_p			= DaTA->Sds_p;
	};
	~ViewInterface_class() = default;


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
	string	Decode( uint8_t idx);
};





#endif /* VIEWINTERFACE_H_ */
