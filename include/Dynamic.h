/*
 * Volume.h
 *
 *  Created on: Mar 9, 2025
 *      Author: sirius
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
	range_T<int>	range				{ 0, 0 };
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

	state_t 		restorestate;

public:

	state_t			current;


			Dynamic_class( range_T<int> r );
	virtual ~Dynamic_class();

	uint8_t	SetupVol(int future_vol, int mode);
	uint8_t SetupFrq(int future_frq, int mode);

	void 	SetDelta( const uint8_t& sl_duration  );
	float 	Get( );
	void 	Update();
	float 	Reset_state();

	int		Current();
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
