/*
 * Statisticclass.cpp
 *
 *  Created on: Oct 3, 2024
 *      Author: sirius
 */

#include <data/Statistic.h>


statistic_t statistic = statistic_struct(); // extern declared in statistic.h

Statistic_class::Statistic_class() :
Logfacility_class( "Statistic" )
{}

Statistic_class::~Statistic_class()
{}

void Statistic_class::Show_Statistic( string module )
{
	long total = statistic.data + statistic.stereo + statistic.shm;
	Comment( INFO, "Memory Statistic for "+ module);
	Comment( INFO, "Data        : "+ to_string( statistic.data ));
	Comment( INFO, "Stereo Data : "+ to_string( statistic.stereo ));
	Comment( INFO, "Shared Data : "+ to_string( statistic.shm ));
	Comment( INFO, "total       : "+ to_string( total));
}

