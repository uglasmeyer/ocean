/*
 * Wavedisplayclass.h
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
 */

#ifndef WAVEDISPLAY_H_
#define WAVEDISPLAY_H_

#include <synthesizer.h>
#include <GUIinterface.h>

class Wavedisplay_class : virtual public Logfacility_class
{
	ifd_t* ifd;

public:
	Wavedisplay_class( ifd_t* );
	virtual ~Wavedisplay_class(){};

	void clear_data		();
	void gen_cxwave_data( void  );
	void add_data_ptr	( Data_t* );
	void set_data_ptr	( size_t );
	void update			( int, Data_t*);
	void set_type		( int );

private:

	int 			frame_counter	= 0;
	buffer_t 		offs 			= 0;
	size_t 			ptr_index 		= 0;
	int 			Type			= 0;
	array<Data_t*, 10>
					data_ptr_array{  };
	bool			split_switch	= false;
	array<Data_t, wavedisplay_len> display_buffer = {0};


	typedef struct param_struct
	{
		int 	 drift 		= 20;
		buffer_t len		= wavedisplay_len;// length of the output buffer
		buffer_t step 		= 10; // ignore step data points
		buffer_t max_offs 	= max_frames - len*step ;//- step;
	} param_t;

	param_t param_flow;
	param_t param_full 	= { 0, wavedisplay_len		, frames_per_sec/wavedisplay_len, frames_per_sec };
	param_t param_split	= { 0, wavedisplay_len / 2	, 1 							, frames_per_sec };

};

#endif /* WAVEDISPLAY_H_ */
