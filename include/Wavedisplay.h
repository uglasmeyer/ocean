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
	Interface_class* Sds_p = nullptr;

public:

	Wavedisplay_class( Interface_class* sds );
	virtual ~Wavedisplay_class(){};

	void Clear_data		();
	void Add_data_ptr	( const string& name, Data_t* );
	void SetId	( size_t wd_id );
	void Set_type		( int wd_type );
	void Write_wavedata ( );

private:

	string 			className		= "";
	int 			frame_counter	= 0;
	buffer_t 		offs 			= 0;
	size_t 			wdId 		= 0;
	int 			Type			= FULLID;
	array<Data_t*, WD_SIZE>
					data_ptr_arr{  };
	bool			split_switch	= true;
	wd_arr_t 		display_buffer = {0};
	wd_arr_t 		gen_cxwave_data( void  );



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
