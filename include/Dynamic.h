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
 * Volume.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef DYNAMIC_H_
#define DYNAMIC_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>
#include <Frequency.h>


class Dynamic_class
	: virtual Logfacility_class
	, virtual Frequency_class
{
	string 			className 			= "";
	uint8_t			slideduration 		= 0;

	const buffer_t	test_frames			= 1000;
	range_T<int>	range				;//{ 0, 0 };
	const range_T<uint8_t>
					slide_duration_range{ 0, 100 };

	typedef struct state_struct
	{
		float		present				= 0.0;
		float 		future_f			= 0.0;
		float		past_f				= 0.0;
		float		delta 				= 0.0;
		uint8_t		future				= 0;
		uint8_t		past				= 0;
		uint8_t		mode				= FIXED;
	} state_t;

	state_t 		restorestate		= state_struct();
	state_t			current				= state_struct();

public:

			Dynamic_class( range_T<int> r );
	virtual ~Dynamic_class();

	uint8_t	SetupVol(int future_vol, int mode);
	uint8_t SetupFrq(int future_frq, int mode);

	void 	SetDelta( const uint8_t& sl_duration  );
	float 	Get( );
	state_t GetCurrent();
	void 	Update();
	float 	Reset_state();

	void 	Show( bool on );
	void 	TestVol();
	void 	TestFrq();


private:
	constexpr buffer_t slideFrames( const uint8_t& sl_duration );
	void	setup();
	void 	end();
	void 	set_state();


};



#endif /* DYNAMIC_H_ */
