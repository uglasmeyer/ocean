/*
 * Composer.h
 *
 *  Created on: Sep 29, 2024
 *      Author: sirius
 */

#ifndef COMPOSER_H_
#define COMPOSER_H_

#include <Appsymbols.h>
#include <Composer/Interpreter.h>

Interpreter_class 		Compiler( &App );
Variation_class 		Variation;
vector<int>				pos_stack {};
String 					Str{""};
vector<line_struct_t> 	Program;
Statistic_class			Statistic{ Log.className };






#endif /* COMPOSER_H_ */
