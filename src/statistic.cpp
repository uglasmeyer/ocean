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
 * Statisticclass.cpp
 *
 *  Created on: Oct 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <data/Statistic.h>


statistic_t statistic = statistic_struct(); // extern declared in statistic.h

Statistic_class::Statistic_class( string name ) :
Logfacility_class( "Statistic" )
{
	className = Logfacility_class::className;
	module = name;
}

Statistic_class::~Statistic_class()
{}

void Statistic_class::Show_Statistic( )
{
	long total = statistic.data + statistic.stereo + statistic.shm;
	Info("Memory Statistic for " + module);
	Info("Data        : "+ to_string( statistic.data ));
	Info("Stereo Data : "+ to_string( statistic.stereo ));
	Info("Shared Data : "+ to_string( statistic.shm ));
	Info("total       : "+ to_string( total));
}

