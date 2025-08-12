/*
 * Adsr.h
 *
 *  Created on: Aug 5, 2025
 *      Author: sirius
 */

#ifndef ADSR_H_
#define ADSR_H_

#include <Oscbase.h>

typedef struct adsr_struct
{ //SDS related. Is specific for each OSC
	uint8_t 	bps 	= 1; //adsr related// { 0, 1, 2, 3, 4 }  => 0, 1, 1/2, 1/4, 1/5, 1/8 sec.,
	uint8_t 	hall	= 0; //adsr related// hall effect [0..100} data shift
	uint8_t 	attack 	= 0; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint8_t 	decay  	= 100;
	spectrum_t 	spec 	= Spectrum_class::spec_struct();
} adsr_t;


class ADSR_class :
	virtual public 	Logfacility_class,
	virtual public 	Spectrum_class,
	virtual	public	Oscillator_base

{
	string 			className 		= "";
	buffer_t 		hall_cursor 	= 0;
	buffer_t 		beat_cursor 	= 0;
	buffer_t		beat_frames		= max_frames;
	Memory			adsr_Mem		{ monobuffer_bytes };

public:
	adsr_t			adsr_data		= adsr_struct();
	buffer_t 		adsr_frames 	= adsr_Mem.mem_ds.data_blocks ;

					ADSR_class		( char _typeid );
					ADSR_class		(); // viewSDS
	virtual 		~ADSR_class		();
	void 			Apply_adsr		( buffer_t frames, Data_t* data, buffer_t frame_offset );
	Data_t* 		AdsrMemData_p	();

	void	 		Set_hallcursor	( buffer_t cursor = 0);
	void			Set_beatcursor	( buffer_t cursor );
	void 			Set_feature		( feature_t f );
	void			Set_adsr		( adsr_t );
	string			Show_adsr		( adsr_t );

private:
	void			adsrOSC			( const buffer_t& );


};




#endif /* ADSR_H_ */
