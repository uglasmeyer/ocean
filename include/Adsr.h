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
 * Adsr.h
 *
 *  Created on: Aug 5, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef ADSR_H_
#define ADSR_H_

#include <Oscbase.h>

const spectrum_t default_adsr_spec =
{
							.vol		= { 1.0, 0.0, 0.0, 0.0, 0.0 } ,
							.frqadj		= { 1.0, 2.0, 3.0, 4.0, 5.0 },
							.frqidx 	= { 1  , 0  , 0  , 0  , 0   }, // see bps
							.volidx 	= { 100, 0  , 0  , 0  , 0   },
							.sum 		= 1.0,
							.wfid 		= { 1  , 1  , 1  , 1  , 1   },
							.osc 		= OSCID,
							.adsr		= true
};

struct adsr_struct
{ //SDS related. Is specific for each OSC
	uint8_t 	bps 	= 1; //adsr related// { 0, 1, 2, 3, 4 }  => 0, 1, 1/2, 1/4, 1/5, 1/8 sec.,
	uint8_t 	hall	= 0; //adsr related// hall effect [0..100} data shift
	uint8_t 	attack 	= 10; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint8_t 	decay  	= 90;
	spectrum_t 	spec 	= default_adsr_spec;
} ;
typedef adsr_struct			adsr_t;

const adsr_struct		default_adsr	=
{
		.bps 	= 1,
		.hall 	= 0,
		.attack = 10,
		.decay 	= 90,
		.spec 	= default_adsr_spec
};



class ADSR_class :
	virtual	public	Oscillator_base
{
	buffer_t 		hall_cursor 	;
	buffer_t 		beat_cursor 	;
	bool			tainted			; // becomes true if adsr_data changes
	buffer_t		beat_frames		;
	Heap_Memory		adsr_Mem		;
	adsr_struct		adsr_data		;
	uint8_t			kbdbps			;

public:

	buffer_t 		adsr_frames 	;

	void 			Apply_adsr		( buffer_t frames, Data_t* data, buffer_t frame_offset );
	Data_t* 		AdsrMemData_p	();

	void	 		Set_hallcursor	( buffer_t cursor = 0);
	void			Set_beatcursor	( buffer_t cursor );
	void 			Set_feature		( feature_t f );
	adsr_t			Set_adsr		( adsr_t );
	void			Set_adsr_spec	( spectrum_t );
	adsr_t			Get_adsr		();
	Data_t*			Adsr_OSC		();
	void			Set_bps			( uint8_t bps = 1 );
	void			Set_kbdbps		( uint8_t bps );
	string			Show_adsr		( adsr_t );
					ADSR_class		( OSCID_e _typeid );
	virtual 		~ADSR_class		();

private:

	void			adsrOSC			( const buffer_t& );
};




#endif /* ADSR_H_ */
