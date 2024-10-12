/*
 * Record.cpp
 *
 *  Created on: Mar 28, 2024
 *      Author: sirius
 */

#include "Record.h"

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


