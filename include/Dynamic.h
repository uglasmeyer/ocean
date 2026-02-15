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

#include <Frequency.h>

typedef struct state_struct
{
	float		present				= 0.0;
	float 		future_f			= 0.0;
	float		past_f				= 0.0;
	float		delta 				= 0.0;
	int			future				= 0;
	int			past				= 0;
	int			mode				= FIXED;
} dynamic_state_t;

/**************************************************
 * Dynamic_class
 *************************************************/
class Dynamic_class
	: virtual public Frequency_class
{

	const buffer_t	test_frames			= 1000;
	const range_T<uint8_t>
					slide_duration_range{ 0, 100 };
	range_T<int>	range				;
	uint8_t			slideduration 		;
	dynamic_state_t	restorestate		;
	dynamic_state_t	current				;

public:


					Dynamic_class		( range_T<int> r );
	virtual 		~Dynamic_class		();

	uint8_t			SetupVol			(int future_vol, DYNAMIC mode);
	uint8_t 		SetupFrq			(int future_frq, DYNAMIC mode);
	void 			SetDelta			( const uint8_t& sl_duration  );
	float 			Get					( );
	void			SetCurrent			( dynamic_state_t state );
	dynamic_state_t GetCurrent			();
	void 			Update				();
	float 			Reset_state			();
	void 			Show				( bool on );
	void 			TestVol				();
	void 			TestFrq				();

private:
	constexpr buffer_t
					slideFrames( const uint8_t& sl_duration );
	void			setup_past();
	void 			end();
	void 			set_state();


};


#endif /* DYNAMIC_H_ */
