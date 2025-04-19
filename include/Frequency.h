/*
 * Frequency.h
 *
 *  Created on: Feb 7, 2025
 *      Author: sirius
 */

#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>
#include <Exit.h>
#include <Table.h>

typedef float 	frq_t;
const 	uint 	C0			= 26;	// defined by: frq_vedtor[C0] = oct_base_frq
const 	size_t 	FRQARR_SIZE = ( max_octave  * oct_steps ) + C0 - 1;
static_assert(  FRQARR_SIZE < 128 );// ensure limitation to uint8

extern array<frq_t, FRQARR_SIZE> frqArray ;
constexpr void initFrqArray();

extern array<string, FRQARR_SIZE> frqNamesArray ;
constexpr void initFrqNamesArray();

const range_t<int > freqarr_range {1, FRQARR_SIZE-1 }; // provide limitation to uint8

class Frequency_class :
		virtual Logfacility_class
{
	string className = "";

public:
	range_t<float> freqfloat_range { 0.0, 0.0 };

	frq_t Calc( const frq_t& _base_freq, const int& idx );
	frq_t GetFrq( const int& idx );
	frq_t Frqadj( const uint8_t& channel, const int8_t& value );
	uint Index( const string& frqName );
	void ShowFrqTable();
	void TestFrequency();

	Frequency_class();
	virtual ~Frequency_class();

private:

};




#endif /* FREQUENCY_H_ */
