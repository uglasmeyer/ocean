/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

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
