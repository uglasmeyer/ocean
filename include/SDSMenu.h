/*
 * Menu.h
 *
 *  Created on: Oct 26, 2025
 *      Author: sirius
 */

#ifndef SDSMENU_H_
#define SDSMENU_H_

#include <Viewinterface.h>
#include <Kbd.h>

class Menue_class :
	virtual public Logfacility_class
{
	string 				className 	= "";
	Kbd_base			Kbd {};
	ViewInterface_class ViewSds ;
	Dataworld_class*	DaTA;
	interface_t*		sds;
	interface_t*		sds_master;
	Id_t 				sdsid;
	KEYCODE 			page 		= KEYCODE::F1;

	typedef function< kbdInt_t( kbdInt_t)> switch_fnc_t ;
	switch_fnc_t 		Switch;

public:
	Menue_class( Dataworld_class* data, switch_fnc_t _switch, string footer = "" ) ;
	virtual ~Menue_class();
	void Set_sdsid( int delta );
	void Loop();

private:

	kbdInt_t Key_event( );
};



#endif /* SDSMENU_H_ */
