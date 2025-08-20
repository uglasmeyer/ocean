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
#include <Exit.h>
#include <String.h>
#include <Table.h>

constexpr uint 						C0			= 26;	// defined by: frq_vedtor[C0] = oct_base_frq

constexpr int 						FRQARR_SIZE = max_octave*oct_steps + C0;
static_assert						( FRQARR_SIZE < numeric_limits<uint8_t>::max() );
typedef array<string, FRQARR_SIZE> 	frqstrarr_t;
typedef array<frq_t,  FRQARR_SIZE> 	frqarray_t;
constexpr range_T<int > 			frqarr_range{ 1, (int)FRQARR_SIZE - 1 };

constexpr uint frqIndex( const int& note, const int& octave )
{
	int idx = octave*oct_steps + note + C0;
	return  check_range( frqarr_range, idx, "frqIndex" );
};
constexpr uint8_t					A3			= frqIndex(9,3); // 220 Hz
constexpr uint8_t					C4			= frqIndex(0,4); // 261 Hz

extern frqarray_t 					frqArray;
extern frqstrarr_t 					frqNamesArray;

class Frequency_class :
	virtual Logfacility_class
{
	string 							className 		= "";
	void 							initFrqArray	();
	void 							initFrqNamesArray();
public:
	const float						log2 			= log(2.0);
	range_T<frq_t> 					frequency_range	{ 0, 0 };

	frq_t 							Calc			( const frq_t& _base_freq, const int& idx );
	frq_t 							GetFrq			( const int& idx );
	frq_t 							Frqadj			( const uint8_t& channel, const int8_t& value );
	uint  							Index			( const string& frqName );
	uint  							Index			( const int& oct, const int& step );
	uint  							Index			( const frq_t& freq );

	void 							ShowFrqTable	();
	void 							TestFrequency	();

									Frequency_class	();
	virtual 						~Frequency_class()
										{ DESTRUCTOR( className ); }

};




#endif /* FREQUENCY_H_ */
