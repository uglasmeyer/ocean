/*
 * Record.cpp
 *
 *  Created on: Mar 28, 2024
 *      Author: sirius
 */

#include "Record.h"

void Record_class::Set( uint* count, uint max )
{
	Comment( INFO, "recording is activated");
	counter = count;
	max_counter = max;
	active = true;
}

void Record_class::Reset( )
{
	ifd_addr->RecCounter = 0;
}

void Record_class::Unset()
{
	Comment( INFO, "recording is de-activated");
	active = false;
}

void Record_class::Set_progress_bar( uint value )
{
	ifd_addr->RecCounter = value;
}

void Record_class::Progress_bar_update( )
{
	if ( active )
	{
		uint count = *counter;
		int value = rint( ( 100 * count )/max_counter ) ;
		Set_progress_bar( value );
		if ( count >= max_counter)
			Unset();
		Comment(DBG2, "recording : " + to_string(value) + " %");
	}
}


