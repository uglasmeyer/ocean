/*
 * Volume.h
 *
 *  Created on: Mar 9, 2025
 *      Author: sirius
 */

#ifndef VOLUME_H_
#define VOLUME_H_

#include <Ocean.h>
#include <Logfacility.h>

class Volume_class :
	Logfacility_class
{
	string 				className 			= "";

	uint8_t				slideduration 		= 0;
	buffer_t			audioframes			= 4 * max_frames;
	float				dvol 				= 0.0;

	const range_t<uint8_t>
				volume_range 		{ 0, 100};
	const range_t<uint8_t>
				slide_duration_range{ 1, 100 };

public:
	uint8_t		future				= 0;
	uint8_t		past				= 0;
	float		present				= 0.0;
	float 		present_pc			= present * 0.01;
	uint8_t		mode				= FIXED;

	Volume_class( );
	virtual ~Volume_class();

	void Set( 	uint8_t future_vol,
				int 	mode);

	float Delta_vol ( uint8_t sl_duration  );
	float Get( );
	void Update();

	void Show( bool on );

	void Test();

private:

};



#endif /* VOLUME_H_ */
