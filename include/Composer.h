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

string					Module = "Composer";
Logfacility_class 		Log( Module );
Variation_class 		Variation;
Dataworld_class			DaTA( COMPID );
Application_class		App( &DaTA );
Interpreter_class 		Compiler( &DaTA );
vector<int>				pos_stack {};
String 					Str{""};
vector<line_struct_t> 	Program;
Statistic_class			Statistic;
Config_class*			Cfg = DaTA.Cfg_p;






#endif /* COMPOSER_H_ */
