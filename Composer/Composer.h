/*
 * Composer.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef COMPOSER_H_
#define COMPOSER_H_

#include <Appsymbols.h>
#include <Composer/Interpreter.h>

Interpreter_class 		Interpreter( &App );
Composer_class			Composer{ &DaTA, &Interpreter };

Statistic_class			Statistic{ Log.className };






#endif /* COMPOSER_H_ */
