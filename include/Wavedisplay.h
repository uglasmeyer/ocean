/*
 * Wavedisplayclass.h
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
 */

#ifndef WAVEDISPLAY_H_
#define WAVEDISPLAY_H_

#include <Wavedisplay_base.h>
#include <data/Interface.h>


class Wavedisplay_class :
	virtual			Logfacility_class,
					wavedisplay_struct,
	virtual public 	osc_struct
{
	Interface_class* Sds_p 			= nullptr;

public:

					Wavedisplay_class( Interface_class* sds );
	virtual 		~Wavedisplay_class() = default;

	void 			Add_role_ptr	( OscroleId_t wd_role,
									Data_t* 	ptr,
									buffer_t* 	wd_frames );
	void 			Add_data_ptr	( OSCID_e wd_type,
									OscroleId_t wd_role,
									Data_t* 	ptr,
									buffer_t* 	wd_frames );
	void 			SetDataPtr		( const WD_data_t& status  );
	void 			Write_wavedata 	();
	void 			Set_wdcursor	(int pos, int max );
	void 			Set_wdcursor	(int pos );


private:
	void 			set_wdmode		( const WdModeID_t& mode );
	void 			setFFTmode		( const bool& mode );
	void 			set_wdcursor	( uint16_t pos );

	string 			className		= "";
	int 			frame_counter	= 0;
	buffer_t 		offs 			= 0;
	size_t 			wdId 			= 0;
	size_t			osId			= 0;
	WdModeID_t		WdMode			= wavedisplay_struct::FULLID;
	struct wd_ptr_struct
	{
		Data_t* 	ptr		= nullptr;
		buffer_t* 	frames	= nullptr;
	};

	typedef 		wd_ptr_struct 	wd_ptr_t;
	array< array< wd_ptr_t , WD_OSC_SIZE>,  WD_ROLES_SIZE >
					data_ptr_mtx 	;
	Data_t*			data_ptr 		= nullptr;
	buffer_t		wd_frames		= 0;
	WD_data_t		wd_status		= WD_data_struct();
	bool			debug_right		= true;
	bool			fft_mode		= false;
	wd_arr_t 		display_data 	= { 0 };

	void	 		gen_cxwave_data( void  );



	typedef struct param_struct
	{
		uint 	 drift 		= 20;
		buffer_t len		= wavedisplay_len;	// length of the output buffer
		buffer_t step 		= 10; 				// jump over step values of data
		buffer_t max_offs 	= max_frames - len*step ;
	} param_t;

	param_t param_flow	= param_struct();
	param_t param_full 	= param_struct(); // dynamic parameter are overwritten{ 0, wavedisplay_len		, max_frames/wavedisplay_len, max_frames };
	param_t param_split	= { 0, wavedisplay_len / 2	, 1 						, max_frames };//frames_per_sec };

};

#endif /* WAVEDISPLAY_H_ */
