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

typedef float 						frq_t;
const 	uint 						C0			= 26;	// defined by: frq_vedtor[C0] = oct_base_frq

constexpr uint FrqIndex( const uint& octave, const uint& note )
{
	return octave*oct_steps + note + C0 ;
};
const 	uint8_t						A3			= FrqIndex(3,2);
const 	size_t 						FRQARR_SIZE = FrqIndex(max_octave, 0 );

static_assert(  FRQARR_SIZE < numeric_limits<uint8_t>::max() );

typedef array<string, FRQARR_SIZE> 	frqstrarr_t;
typedef array<frq_t,  FRQARR_SIZE> 	frqarray_t;

extern frqarray_t 					frqArray;;
extern frqstrarr_t 					frqNamesArray;


const range_t<int > 				frqarr_range {1, FRQARR_SIZE-1 };

class Frequency_class :
		virtual Logfacility_class
{
	string className = "";

public:
	range_t<float> frequency_range;// { frqArray[1], frqArray[FRQARR_SIZE-1] };

	frq_t Calc( const frq_t& _base_freq, const int& idx );
	frq_t GetFrq( const int& idx );
	frq_t Frqadj( const uint8_t& channel, const int8_t& value );

	Frequency_class();
	virtual ~Frequency_class();
	uint  Index( const string& frqName );
	void ShowFrqTable();
	void TestFrequency();


//	const uint A3 = Index("A3");

private:
	void initFrqArray(  );
	void initFrqNamesArray();


};




#endif /* FREQUENCY_H_ */
