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
 * Appsymbols.h
 *
 *  Created on: Apr 29, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef APPSYMBOLS_H_
#define APPSYMBOLS_H_

#include <App.h>


Exit_class				Exit			{};
Config_class			Cfg				{};
Logfacility_class		Log				( Cfg.prgName );

#ifndef WITHOUT_SHM // Setup
	Semaphore_class		Sem				{ Cfg.Config.Sem_key };
	Dataworld_class 	DaTA			{ &Cfg, &Sem };
	Application_class	App				{ &DaTA };
	interface_t*		sds 			= DaTA.GetSdsAddr();
	interface_t*		sds_master 		= DaTA.sds_master;
	Interface_class*	Sds				= DaTA.GetSds();
	Interface_class*	Sds_master		= DaTA.Sds_master;
#endif




#endif /* APPSYMBOLS_H_ */
