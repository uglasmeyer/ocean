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
 * Menu.h
 *
 *  Created on: Oct 26, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef SDSMENU_H_
#define SDSMENU_H_

#include <Viewinterface.h>
#include <Kbd.h>

class Menue_class :
	virtual public Logfacility_class
{
	string 				className 	= "";
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
