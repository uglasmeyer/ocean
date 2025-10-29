/*
 * Adsr.h
 *
 *  Created on: Aug 5, 2025
 *      Author: sirius
 */

#ifndef ADSR_H_
#define ADSR_H_

#include <Oscbase.h>

const spectrum_t default_adsr_spec =
{
							.vol		= { 1.0, 0.0, 0.0, 0.0, 0.0 } ,
							.frqadj		= { 1.0, 2.0, 3.0, 4.0, 5.0 },
							.frqidx 	= { 1, 1, 1, 1, 1 }, // see bps
							.volidx 	= { 100, 0, 0, 0, 0 },
							.sum 		= 1.0,
							.wfid 		= {1, 1, 1, 1, 1 },
							.osc 		= OSCID,
							.adsr		= true
};

struct adsr_struct
{ //SDS related. Is specific for each OSC
	uint8_t 	bps 	= 1; //adsr related// { 0, 1, 2, 3, 4 }  => 0, 1, 1/2, 1/4, 1/5, 1/8 sec.,
	uint8_t 	hall	= 0; //adsr related// hall effect [0..100} data shift
	uint8_t 	attack 	= 10; // [0 ... 100 ]   -> [ 0.1 ... 1 ]
	uint8_t 	decay  	= 90;
	spectrum_t 	spec 	= spec_struct();
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
	virtual  		Logfacility_class,
	virtual public 	Spectrum_class,
	virtual			Oscillator_base

{
	string 			className 		= "";
	buffer_t 		hall_cursor 	= 0;
	buffer_t 		beat_cursor 	= 0;
	bool			tainted			= true; // becomes true if adsr_data changes
	buffer_t		beat_frames		= max_frames;
	Memory			adsr_Mem		{ monobuffer_bytes }; //max_frames*sizeof(Data)

public:

	buffer_t 		adsr_frames 	= adsr_Mem.mem_ds.data_blocks ;

	void 			Apply_adsr		( buffer_t frames, Data_t* data, buffer_t frame_offset );
	Data_t* 		AdsrMemData_p	();

	void	 		Set_hallcursor	( buffer_t cursor = 0);
	void			Set_beatcursor	( buffer_t cursor );
	void 			Set_feature		( feature_t f );
	adsr_t			Set_adsr		( adsr_t );
	void			Set_adsr_spec	( spectrum_t );
	adsr_t			Get_adsr		();
	void			Set_bps			();
	string			Show_adsr		( adsr_t );
					ADSR_class		( OscId_t _typeid );
					ADSR_class		(); // viewSDS
	virtual 		~ADSR_class		();

private:
	adsr_struct		adsr_data		;

	void			adsrOSC			( const buffer_t& );
};




#endif /* ADSR_H_ */
