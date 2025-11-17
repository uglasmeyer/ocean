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
 * Record.cpp
 *
 *  Created on: Mar 28, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Progressbar.h>

ProgressBar_class::ProgressBar_class(uint8_t* counter_p) :
Logfacility_class("Record")
{
	this->RecCounter = counter_p;
	this->counter = 0;
};

ProgressBar_class::~ProgressBar_class()
{
	if ( RecCounter == nullptr ) return;
	*RecCounter = 0;
};

void ProgressBar_class::Setup( uint8_t* sds )
{
	RecCounter = sds;
}
void ProgressBar_class::Set( uint* count, uint max )
{
	Comment( INFO, "recording is activated");
	counter = count;
	max_counter = max;
	active = true;
}

void ProgressBar_class::Reset( )
{
	*RecCounter = 0;
}

void ProgressBar_class::Unset()
{
	Comment( INFO, "recording is de-activated");
	active = false;
}

void ProgressBar_class::SetValue( uint value )
{
	*RecCounter = value;
}

void ProgressBar_class::Update( )
{
	if ( active )
	{
		uint count = *counter;
		int value = rint( ( 100 * count )/max_counter ) ;
		SetValue( value );
		if ( count >= max_counter)
			Unset();
		Comment(DBG2, "recording : " + to_string(value) + " %");
	}
}


