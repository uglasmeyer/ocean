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
 * Record.h
 *
 *  Created on: Mar 28, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <data/Interface.h>
#include <data/Memory.h>
#include <Ocean.h>

class ProgressBar_class : virtual Logfacility_class
{
public:
	// static parameter
	uint8_t* RecCounter;
	ProgressBar_class(uint8_t* ); // addr of the counter to watch
	virtual ~ProgressBar_class();

	// dynamic parameter
	uint* counter;
	uint max_counter = 1;
	bool active = false;

	void Setup( uint8_t* sds );
	void SetValue( uint );
	void Set( uint* count, uint max );
	void Unset();
	void Reset();
	void Update( );
};

#endif /* PROGRESSBAR_H_ */
