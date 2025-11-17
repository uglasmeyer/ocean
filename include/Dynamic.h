//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


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
