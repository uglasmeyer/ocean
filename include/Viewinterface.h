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
 * Appstate.h
 *
 *  Created on: May 4, 2025
 *      Author: Ulrich.Glasmeyer@web.de
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
