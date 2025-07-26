/*
 * Appsymbols.h
 *
 *  Created on: Apr 29, 2025
 *      Author: sirius
 */

#ifndef APPSYMBOLS_H_
#define APPSYMBOLS_H_

#include <App.h>
#include <error.h>


process_t				Process			{};
Logfacility_class		Log				( Process.name );
Exit_class				Exit			{};
file_structure			fs				{};
Config_class			Cfg				{ Process.name };
#ifndef WITHOUT_SHM
	Semaphore_class		Sem				{ Cfg.Config.Sem_key };
	Dataworld_class 	DaTA			{ Process.AppId, &Cfg, &Sem };
	Application_class	App				{ &DaTA };
	interface_t*		sds 			= DaTA.GetSdsAddr();
	interface_t*		sds_master 		= DaTA.sds_master;
	Interface_class*	Sds				= DaTA.GetSds();
	Interface_class*	Sds_master		= DaTA.Sds_master;
#endif




#endif /* APPSYMBOLS_H_ */
