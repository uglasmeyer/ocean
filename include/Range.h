/**************************************************************************
MIT License

Copyright (c) 2026 Ulrich Glasmeyer

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
 * Range.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef RANGE_H_
#define RANGE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <inttypes.h>

template< typename T >
struct range_T
{
	T	min;
	T	max;
	size_t	len = max - min;
};

const range_T<uint8_t>	percent_range		{ 0, 100 };
const range_T<int>		volidx_range		{ 0, 100 };
const range_T<uint8_t>	uint8_range			{ 0, 255 };

template< typename T>
constexpr bool in_range( range_T<T> range, T val )
{
	if ( val < range.min )
		return false;
	if ( val > range.max )
		return false;
	return true;
}
template< typename T>
constexpr T check_range( range_T<T> r, T val, std::string errmsg = "" )
{
	if( val < r.min )
	{
		if ( errmsg.length() > 0 )
			std::cout << "WARNING: " << errmsg
									<< ": "
									<< (int)val
									<< " adjusted to min boundaries "
									<< r.min << std::endl;
		return r.min;
	}
	if (val > r.max )
	{
		if ( errmsg.length() > 0 )
			std::cout.flush() << "WARNING: " << errmsg
									<< ": "
									<< (int)val
									<< " adjusted to max boundaries "
									<< r.max << std::endl;
		return r.max;
	}
	return val;
};

template< typename T>
constexpr T check_cycle( range_T<T> r, T val, std::string err=""   )
{
	if( r.max == 0 ) return 0;
	if( val < r.min )
		return r.max;
	if ( val > r.max)
		return val % r.max;//r.max % val;////r.min;//val - r.max;
	return val;
};

template< typename T>
constexpr T check_cycle2( range_T<T> r, T val  ) //exclude the last element
{
	if( r.len == 0 )
		return T(0);
	if( val < r.min )
		return T( r.max - ((r.min - val ) % r.len) );
	if ( val >= r.max )
		return T( r.min + ((val - r.max ) % r.len) );
	return val;
};

template<typename T>
std::string range_str( range_T<T> range )
{
	std::stringstream strs {};
	strs << (int)range.min << "..." << (int)range.max << ", " << (int) range.len ;
	return strs.str();
};













#endif /* RANGE_H_ */
