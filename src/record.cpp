/*
 * Record.cpp
 *
 *  Created on: Mar 28, 2024
 *      Author: sirius
 */

#include "Record.h"

ProgressBar_class::ProgressBar_class(ifd_t* addr) :
Logfacility_class("Record")
{
	this->ifd_addr = addr;
	this->counter = 0;
};

ProgressBar_class::~ProgressBar_class()
{
	ifd_addr->RecCounter = 0;
};


void ProgressBar_class::Set( uint* count, uint max )
{
	Comment( INFO, "recording is activated");
	counter = count;
	max_counter = max;
	active = true;
}

void ProgressBar_class::Reset( )
{
	ifd_addr->RecCounter = 0;
}

void ProgressBar_class::Unset()
{
	Comment( INFO, "recording is de-activated");
	active = false;
}

void ProgressBar_class::Setup( uint value )
{
	ifd_addr->RecCounter = value;
}

void ProgressBar_class::Update( )
{
	if ( active )
	{
		uint count = *counter;
		int value = rint( ( 100 * count )/max_counter ) ;
		Setup( value );
		if ( count >= max_counter)
			Unset();
		Comment(DBG2, "recording : " + to_string(value) + " %");
	}
}


