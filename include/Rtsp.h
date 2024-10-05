/*
 * Rtsp.h
 *
 *  Created on: Sep 29, 2024
 *      Author: sirius
 */

#ifndef RTSP_H_
#define RTSP_H_

#include <App.h>
#include <data/Semaphore.h>
#include <System.h>

const string		Module	= "rtsp";
Logfacility_class	Log( Module );
Dataworld_class		DaTA( RTSPID );
Application_class	App( &DaTA );
DirStructure_class 	Dir;
Semaphore_class*	Sem = DaTA.Sem_p;
Config_class* 		Cfg = DaTA.Cfg_p;
interface_t* 		sds = DaTA.Sds.addr;

const uint MAX_CONFIG = 3;
array<uint, MAX_CONFIG> sds_keys {0,0,0};


#endif /* RTSP_H_ */
