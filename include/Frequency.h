/*
 * Frequency.h
 *
 *  Created on: Feb 7, 2025
 *      Author: sirius
 */

#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include <Ocean.h>
#include <Exit.h>

typedef float frq_t;
const size_t FRQARR_SIZE = ( 6  * 12 ) + 25 - 1;

extern array<frq_t, FRQARR_SIZE> frq_vector ;
extern constexpr void initFrqVector(  );

class Frequency_class
{
public:
	int frq_vector_len	= 0;
	int C0				= 0;

	frq_t Calc( const frq_t& _base_freq, const int& idx );
	frq_t Calc( const int& idx );
	frq_t Frqadj( const uint8_t& channel, const int8_t& value );
	Frequency_class();
	virtual ~Frequency_class();

private:
};




#endif /* FREQUENCY_H_ */
