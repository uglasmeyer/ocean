/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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

const vector<string> slidermode_str =
{
	"Fixed",
	"Slide",
	"Combine",
	"Init"
};
enum DYNAMIC_e : unsigned char
{
	FIXED,
	SLIDE,
	COMBINE,
	INIT
}; // frequency and volume change mode

const range_T<DYNAMIC_e> dynamic_valrange{ FIXED, SLIDE };
const range_T<DYNAMIC_e> dynamic_frqrange{ FIXED, COMBINE }; // range(0,2)

typedef struct dynamic_state_struct
{
	float		present				= 0.0;
	float 		future_f			= 0.0;
	float		past_f				= 0.0;
	float		delta 				= 0.0;
	int			future				= 0;
	int			past				= 0;
	DYNAMIC_e	mode				= INIT;
} dynamic_state_t;

enum DYNFACE_e : uint8_t
{
	FRQ,
	VOL
};

// SDS related
const size_t				SPEC_CHANNELS	= 9;

/**************************************************
 * Dynamic_class
 *************************************************/
class Dynamic_class
	: virtual public Frequency_class
{


	const buffer_t	test_frames			= 1000;
	const buffer_t	max_slide_frames	= max_slide_sec * frames_per_sec;

	range_T<uint8_t>value_range			;
	range_T<int>	slide_range			;
	array<dynamic_state_t, SPEC_CHANNELS>
					dyn_state_arr 		{ dynamic_state_struct() };

//	dynamic_state_t	restorestate		;
	dynamic_state_t	current				;
	DYNFACE_e		dynface				;

public:

					Dynamic_class		( DYNFACE_e _type,
										range_T<int> _range,
										range_T<int> _sl_range);
	virtual 		~Dynamic_class		();

	uint8_t 		Setup				( uint8_t future_vol, DYNAMIC_e mode, size_t channel = 0 );
	void 			SetDelta			( uint8_t sl_duration, size_t channel = 0 );
	float 			Get					( size_t channel = 0 );
	dynamic_state_t GetCurrent			();
	void 			Update				( size_t channel = 0 );
	float 			Reset_state			( size_t channel = 0 );
	void			SetCurrent			( dynamic_state_t state );
	void 			TestVol				();
	void 			TestFrq				();

private:
	void 			end					( size_t channel = 0 );
	void 			show				( bool on );


};


#endif /* DYNAMIC_H_ */
