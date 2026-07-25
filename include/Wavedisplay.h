/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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

#include <data/Interface.h>

/**************************************************
 * WD_struct
 * Wavedisplay data interface to SDS
 *************************************************/
struct WD_struct
{
	WD_data_t			data;
	WD_struct( interface_t* sds )
	{
		Read( sds );
	}
	~WD_struct() = default;
	void Read( interface_t* sds )
	{
		data = sds->WD_state;
	}
	void Save( interface_t* sds )
	{
		sds->WD_state = data;
	}
};

/**************************************************
 * WfDisplay_que_struct
 *************************************************/

struct WfDisplay_que_struct
{
	typedef vector<RoleId_e>
					roleid_vec_t;

    roleid_vec_t	Que					{};
    array<roleid_vec_t, MAXCONFIG >
    				Que_arr				{ roleid_vec_t() };
    RoleId_e		prev_role			;
    interface_t*	sds					;
    sta_role_map*	roleMap				;

					WfDisplay_que_struct( interface_t* _sds, sta_role_map* _map );
					~WfDisplay_que_struct() = default;
	void 			Add					( RoleId_e role );
	void 			Init				( interface_t* _sds );
	void 			Remove				( RoleId_e role );
	void 			Set_prev			();
	void			Set_que				( interface_t* _sds );
	void 			Show				( bool debug );

};


/**************************************************
 * Wavedisplay_class
 *************************************************/
class Wavedisplay_class :
	virtual	public	Logfacility_class,
					wavedisplay_struct
{
	SharedData_class*
					Sds_p 			;
	interface_t*	sds				;
	string 			rolename		;
	string 			oscname			;
	string			fourierBinFile	;
public:

					Wavedisplay_class( SharedData_class* _sds );
	virtual 		~Wavedisplay_class();

	void 			Add_role_ptr	( RoleId_e wd_role,
									Data_t* 	ptr,
									buffer_t* 	wd_frames );
	void 			Add_data_ptr	( OSCID_e wd_type,
									RoleId_e wd_role,
									Data_t* 	ptr,
									buffer_t* 	wd_frames );
	void 			Set_WdData		();
	void 			Set_wdcursor	( buffer_t pos, buffer_t unit );
	void			TestWd			();
	void 			WriteData 		();

private:
	void	 		gen_wave_data	( WdModeID_t mode );
	void 			set_Wdmode		();
	void 			set_Wdcursor	( uint16_t pos );
	void 			set_Wdnames		();
	void 			set_Wdptr		( RoleId_e role);
	void			set_fourierfile	();
	void 			write_display_arr(Data_t* dst, int len );

	typedef struct wd_ptr_struct
	{
		RoleId_e	role	{};
		OSCID_e		type	{};
		Data_t* 	Data_p	= nullptr;
		buffer_t* 	frames	= nullptr;
	} wd_ptr_t;

	typedef struct param_struct
	{
		uint 	 drift 		= 20;
		buffer_t len		= wavedisplay_len;	// length of the output buffer
		buffer_t step 		= 10; 				// jump over step values of data
		buffer_t max_offs 	= max_frames - len*step ;
	} param_t;

	typedef array< 	array< wd_ptr_t , 	WD_OSC_SIZE>, ROLE_SIZE >
					data_ptr_mtx_t;

	int 			frame_counter	;
	buffer_t 		offs 			;
	data_ptr_mtx_t	data_ptr_mtx 	;
	Data_t*			Data_p 			;	// is  Wd_ptr[][].Data_p
	buffer_t		wd_frames		;	// is *Wd_ptr[][].frames
	wd_ptr_t		Wd_ptr			;	// is Wd_ptr[][]
	WD_struct		WD				;
	bool			debug_right		;
	wd_arr_t 		display_data 	{ 0 };

	param_t 		param_flow		;
	param_t 		param_full 		;
	param_t 		param_split		;
	param_t 		param_cursor	;
	param_t			param_fft		;
	param_t			param_adsr		;
};


#endif /* WAVEDISPLAY_H_ */
