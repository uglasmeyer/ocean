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

const range_t<uint8_t>	slide_duration_range{ 1, 100 };

class Dynamic_class :
	Logfacility_class
{
	string 		className 			= "";
	uint8_t		slideduration 		= 0;
	float		delta 				= 0.0;
	float		present				= 0.0;
	float 		present_pc			= present * 0.01;
	uint8_t		mode				= FIXED;
	range_t<int>range				{ 0, 0 };

public:
	uint8_t		future				= 0;
	uint8_t		past				= 0;


	Frequency_class	Frequency {};

	Dynamic_class( range_t<int> r );
	virtual ~Dynamic_class();

	uint8_t	Set(int future_vol,	int	mode);

	void 	DeltaVol( const uint8_t& sl_duration  );
	float 	GetVol( );
	void 	DeltaFrq( const uint8_t& sl_duration );
	float	GetFrq( );

	void 	Update();

	void 	Show( bool on );

	void 	Test();

private:
	constexpr buffer_t slideFrames( const uint8_t& sl_duration );

};



#endif /* DYNAMIC_H_ */
