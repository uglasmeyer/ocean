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

