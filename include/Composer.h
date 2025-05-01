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

#include <Appsymbols.h>

Interpreter_class 		Compiler( &DaTA );
Variation_class 		Variation;
vector<int>				pos_stack {};
String 					Str{""};
vector<line_struct_t> 	Program;
Statistic_class			Statistic{ Log.className };






#endif /* COMPOSER_H_ */
