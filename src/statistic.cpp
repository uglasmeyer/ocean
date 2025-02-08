/*
 * Statisticclass.cpp
 *
 *  Created on: Oct 3, 2024
 *      Author: sirius
 */

#include <data/Statistic.h>


statistic_t statistic = statistic_struct(); // extern declared in statistic.h

Statistic_class::Statistic_class( string module ) :
Logfacility_class( "Statistic" )
{
	className = Logfacility_class::module;

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

