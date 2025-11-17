//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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
