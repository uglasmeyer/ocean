/*
 * Wavedisplayclass.h
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
 */

#ifndef WAVEDISPLAY_H_
#define WAVEDISPLAY_H_

#include <data/Interface.h>
#include <Wavedisplay_base.h>




class Wavedisplay_class : virtual Logfacility_class
{

public:
	Wavedisplay_class( );
	virtual ~Wavedisplay_class(){};

	void Clear_data		();
	wd_arr_t Gen_cxwave_data( void  );
	void Add_data_ptr	( const string& name, Data_t* );
	void Set_data_ptr	( size_t );
	void Update			( int, Data_t*);
	void Set_type		( int );

private:

	int 			frame_counter	= 0;
	buffer_t 		offs 			= 0;
	size_t 			ptr_index 		= 0;
	int 			Type			= FULLID;
	array<Data_t*, WD_SIZE>
					data_ptr_arr{  };
	bool			split_switch	= true;
	wd_arr_t 		display_buffer = {0};


	typedef struct param_struct
	{
		int 	 drift 		= 20;
		buffer_t len		= wavedisplay_len;	// length of the output buffer
		buffer_t step 		= 10; 				// ignore step # of data points
		buffer_t max_offs 	= max_frames - len*step ;
	} param_t;

	param_t param_flow;
	param_t param_full 	= { 0, wavedisplay_len		, frames_per_sec/wavedisplay_len, frames_per_sec };
	param_t param_split	= { 0, wavedisplay_len / 2	, 1 							, frames_per_sec };

};

#endif /* WAVEDISPLAY_H_ */
