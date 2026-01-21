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
 * Wavedisplayclass.h
 *
 *  Created on: Feb 23, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef WAVEDISPLAY_H_
#define WAVEDISPLAY_H_

#include <Wavedisplay_base.h>
#include <data/Interface.h>

/**************************************************
 * Wavedisplay_class
 *************************************************/
class Wavedisplay_class :
	virtual	public	Logfacility_class,
					wavedisplay_struct,
	virtual public 	osc_struct
{
	Interface_class* Sds_p 			;
	interface_t*	sds				;

public:

					Wavedisplay_class( Interface_class* _sds );
	virtual 		~Wavedisplay_class() = default;

	void 			Add_role_ptr	( RoleId_e wd_role,
									Data_t* 	ptr,
									buffer_t* 	wd_frames );
	void 			Add_data_ptr	( OSCID_e wd_type,
									RoleId_e wd_role,
									Data_t* 	ptr,
									buffer_t* 	wd_frames );
	void 			Set_DataPtr		( WD_data_t& status  );
	void 			Write_wavedata 	();
	void 			Set_wdcursor	( buffer_t pos, buffer_t unit );
	void 			Set_WdRole		(const RoleId_e &role);
	void 			Set_wdmode		( const WdModeID_t& mode );


private:
	void 			setFFTmode		( const bool& mode );
	void 			set_wdcursor	( uint16_t pos );
	void	 		gen_cxwave_data	( void  );

	typedef struct wd_ptr_struct
	{
		Data_t* 	ptr		= nullptr;
		buffer_t* 	frames	= nullptr;
	} wd_ptr_t;

	typedef struct param_struct
	{
		uint 	 drift 		= 20;
		buffer_t len		= wavedisplay_len;	// length of the output buffer
		buffer_t step 		= 10; 				// jump over step values of data
		buffer_t max_offs 	= max_frames - len*step ;
	} param_t;

	typedef array< 	array< wd_ptr_t , 	WD_OSC_SIZE>,  	WD_ROLES_SIZE >
					data_ptr_mtx_t;

	string 			className		;
	int 			frame_counter	;
	buffer_t 		offs 			;
	WdModeID_t		WdMode			;
	data_ptr_mtx_t	data_ptr_mtx 	;
	Data_t*			data_ptr 		;
	wd_ptr_t		Wd_ptr			;
	WD_data_t		wd_data			;
	bool			debug_right		;
	bool			fft_mode		;
	wd_arr_t 		display_data 	{ 0 };

	param_t param_flow				;
	param_t param_full 				;
	param_t param_split				;
	param_t param_cursor			;
};

#endif /* WAVEDISPLAY_H_ */
