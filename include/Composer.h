/*
 * Composer.h
 *
 *  Created on: Sep 29, 2024
 *      Author: sirius
 */

#ifndef COMPOSER_H_
#define COMPOSER_H_

#include <Interpreter.h>
#include <App.h>
#include <System.h>
#include <Exit.h>


Exit_class				Exit{};
string					Module = "Composer";
Logfacility_class 		Log( Module );
Dataworld_class			DaTA( COMPID );
Application_class		App( &DaTA );
Interpreter_class 		Compiler( &DaTA );
Variation_class 		Variation;
vector<int>				pos_stack {};
String 					Str{""};
vector<line_struct_t> 	Program;
Statistic_class			Statistic{ Log.module };
Config_class*			Cfg = DaTA.Cfg_p;






#endif /* COMPOSER_H_ */
