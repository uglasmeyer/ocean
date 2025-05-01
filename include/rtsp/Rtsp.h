/*
 * Rtsp.h
 *
 *  Created on: Sep 29, 2024
 *      Author: sirius
 */

#ifndef RTSP_RTSP_H_
#define RTSP_RTSP_H_

#include <App.h>
#include <data/Semaphore.h>
#include <System.h>
#include <Exit.h>

#include <Appsymbols.h>

Appstate_class*		Appstate			= &DaTA.Appstate;
Semaphore_class*	Sem_p = DaTA.Sem_p;
Config_class* 		Cfg_p = DaTA.Cfg_p;
//interface_t* 		sds = DaTA.GetSdsAddr();
//interface_t*		sds_master = DaTA.sds_master;


#endif /* RTSP_RTSP_H_ */
