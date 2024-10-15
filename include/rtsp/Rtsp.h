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
#include <rtsp/ShmMixer.h>


const string		Module	= "rtsp";
Logfacility_class	Log( Module );
Dataworld_class		DaTA( RTSPID );
Application_class	App( &DaTA );
ShmMixer_class		ShmMixer{ &DaTA };
DirStructure_class 	Dir;
Semaphore_class*	Sem = DaTA.Sem_p;
Config_class* 		Cfg = DaTA.Cfg_p;
interface_t* 		sds = DaTA.Sds.addr;


#endif /* RTSP_RTSP_H_ */
