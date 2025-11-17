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
 * Statistic.h
 *
 *  Created on: Oct 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DATA_STATISTIC_H_
#define DATA_STATISTIC_H_
#include <Ocean.h>
#include <Logfacility.h>

/*
 *
 */
typedef struct statistic_struct
{
	long 	shm 	= 0;
	long 	stereo	= 0;
	long 	data 	= 0;
	long 	total	= 0;
} statistic_t;
extern statistic_t statistic;


class Statistic_class :
	public virtual Logfacility_class
{
	string className = "";

public:

	string module = "";
	void Show_Statistic( );

	Statistic_class( string module );
	virtual ~Statistic_class();
};

#endif /* DATA_STATISTIC_H_ */
