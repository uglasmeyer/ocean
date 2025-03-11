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

typedef float 	frq_t;
const 	uint 	C0			= 26;	// defined by: frq_vedtor[C0] = oct_base_frq
//const	uint	A1			= 35;	// 55Hz
//const 	uint	A2			= A1+12;// 110Hz
//const 	uint	A3			= A2+12;// 220Hz
const 	size_t 	FRQARR_SIZE = ( max_octave  * oct_steps ) + C0 - 1;

extern array<frq_t, FRQARR_SIZE> frqArray ;
constexpr void initFrqArray();
extern array<string, FRQARR_SIZE> frqNamesArray ;
constexpr void initFrqNamesArray();

class Frequency_class :
		virtual Logfacility_class
{
	string className = "";
public:
	uint frq_vector_len	= 0;

	frq_t Calc( const frq_t& _base_freq, const int& idx );
	frq_t Calc( const int& idx );
	uint Index( string frqName );
	void ShowFrqTable();
	frq_t Frqadj( const uint8_t& channel, const int8_t& value );
	void TestFrequency();

	Frequency_class();
	virtual ~Frequency_class();

private:
};




#endif /* FREQUENCY_H_ */
