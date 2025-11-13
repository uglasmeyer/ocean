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
	, virtual 			sdsstate_struct
	, virtual 			interface_struct
	, virtual			Device_class
	, virtual public	ADSR_class
	, AppMap_struct
	, wavedisplay_struct

{
	string 				className 	= "";
	int					counter		= 0;
	string				footer		= "";
	Interface_class*	Sds_p 		= nullptr;
	interface_t* 		sds_p 		= nullptr;
	interface_t*		sds_master	= nullptr;

public:
	Appstate_class		Appstate	;
	Dataworld_class*	DaTA;
	Kbd_base			Kbd			{};
	keymap_struct		Keymap		{};

			ViewInterface_class( APPID appid, Dataworld_class* DaTA );
	virtual ~ViewInterface_class() = default;

	KEYCODE	ShowPage( interface_t* sds );
	void 	IncCommitcounter();
	void	SetFooter( const string& _footer );

private:

	void 	showOSCs();
	void 	F2_showProcesses();
	void 	F3_showStates() ;

	void 	show_Que();
	void 	show_Ipc();
	void 	show_spectrum();

	void 	show_Adsr();
	void 	showKeys();
	void 	show_system();


	void	printHeader( KEYCODE keycode );
	void 	printFooter();
	void 	set_sds( interface_t* sds );


	void 	selfTest();

};





#endif /* VIEWINTERFACE_H_ */
