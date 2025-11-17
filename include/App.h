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
 * App.h
 *
 *  Created on: Aug 27, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef APP_H_
#define APP_H_

#include <Ocean.h>
#include <data/Semaphore.h>
#include <data/Statistic.h>
#include <data/DataWorld.h>
#include <Kbd.h>



// https://en.cppreference.com/w/cpp/language/parameter_pack

class Application_class :
	virtual Logfacility_class,
	sdsstate_struct,
	Statistic_class
{
	string 				className 			= "";
public:

	APPID 				AppId				= NoAPPID;
	process_t 			properties 			{};
	Time_class			Timer				{};
	Kbd_base			Kbd					{};
	Dataworld_class*	DaTA				= nullptr;
	Appstate_class*		Appstate			= nullptr;
	interface_t* 		sds					= nullptr;
	interface_t*		sds_master			= nullptr;
	Config_class*		Cfg 				= nullptr;
	Interface_class*	Sds					= nullptr;

	string 				This_Application 	= "";
    const set<APPID>	logowner 			=  { GUI_ID, COMPID, RTSPID };

    					Application_class	( Dataworld_class* );
	virtual 			~Application_class	();

	void 				Start				( int, char* [] );
	void 				Ready				();
	kbdInt_t			KeyboardKey			( bool debug);


private:
	bool 				redirect_stderr 	= false;
	void 				app_properties		();
	void 				deRegister			();
	void 				versionTxt			();

	string 				ProgramName 		= "";
};



#endif /* APP_H_ */
