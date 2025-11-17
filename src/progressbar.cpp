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


