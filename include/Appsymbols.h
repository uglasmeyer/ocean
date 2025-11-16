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
