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
 * Frequency.h
 *
 *  Created on: Feb 7, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include <Ocean.h>
#include <Logfacility.h>
#include <String.h>

const int 								C0			= 26;	// defined by: frq_vedtor[C0] = oct_base_frq

#define FRQINDEX( note, octave )\
		{ (octave) * oct_steps + (note) + C0 }

constexpr 	int 						FRQARR_SIZE = FRQINDEX( 0, max_octave ) ;
const		uint8_t						FRQEXT_SIZE = FRQARR_SIZE + 3*oct_steps;

static_assert							( FRQARR_SIZE < numeric_limits<uint8_t>::max() ); //255

typedef 	array<string, FRQEXT_SIZE> 	frqstrarr_t;
typedef 	array<frq_t,  FRQEXT_SIZE> 	frqarray_t;
const 		uint			 			HARMON_PRIMS= 8;
constexpr 	int							HARMON_SIZE = ( HARMON_PRIMS-1) * HARMON_PRIMS / 2 + 1;
typedef 	array<frq_t,  HARMON_SIZE>	harmonic_t;

constexpr 	range_T<int > 				frqarr_range{ 1, (int)(FRQARR_SIZE - 1) };
constexpr 	range_T<int > 				frqext_range{ 1, (int)(FRQEXT_SIZE - 1) };

constexpr 	uint frqIndex( const int& note, const int& octave )
{
	int idx = FRQINDEX( note, octave );
	return  check_range( frqext_range, idx, "frqIndex" );
};

constexpr 	uint8_t						A4			= frqIndex(9,4); // 440 Hz
constexpr 	uint8_t						A3			= frqIndex(9,3); // 220 Hz
constexpr 	uint8_t						A2			= frqIndex(9,2); // 110 Hz
constexpr 	uint8_t						A1			= frqIndex(9,1); //  55 Hz
constexpr 	uint8_t						C4			= frqIndex(0,4); // 261 Hz
extern 		frqarray_t 					frqArray;
extern 		frqstrarr_t 				frqNamesArray;
extern 		harmonic_t					harmonicArray;

class Frequency_class :
	virtual Logfacility_class
{
	string 								className 		= "";
	void 								initFrqArray	();
	void 								initFrqNamesArray();
	void 								initHarmonics	();

public:
	const float							log2 			= log(2.0);
	range_T<int8_t>						harmonic_range	{ -HARMON_SIZE, HARMON_SIZE };
	frq_t 								Calc			( const frq_t& _base_freq,
													      const int& idx );
	frq_t 								GetFrq			( const int& idx );
	frq_t 								Frqadj			( const uint8_t& channel,
														  const int8_t& value );
	uint  								Index			( const string& frqName );
	uint  								Index			( const int& oct,
														  const int& step );
	uint  								Index			( const frq_t& freq );

	void 								ShowFrqTable	();
	void 								TestFrequency	();

										Frequency_class	();
	virtual 							~Frequency_class();

};




#endif /* FREQUENCY_H_ */
