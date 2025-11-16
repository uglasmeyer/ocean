/*
 * Wavedisplay_base.h
 *
 *  Created on: Sep 9, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef WAVEDISPLAY_BASE_H_
#define WAVEDISPLAY_BASE_H_

#include <Ocean.h>

struct wavedisplay_struct
{
	const vector<string> types =
	{
		"Full",
		"Flow",
		"Debug"
	};
	const vector<string> fftmodes =
	{
		"A(τ)",
		"A(ω)"
	};
	enum WDMODE_t
	{
		FULLID,
		FLOWID,
		DEBUGID,
		FFTID
	};

};
typedef wavedisplay_struct
						wavedisplay_t;
typedef wavedisplay_t::WDMODE_t
						WdModeID_t;

struct WD_data_struct
{	// SDS related

	OSCID_e 			oscId 			= OSCID;
	OscroleId_t			roleId 			= INSTRROLE;
	bool 				fftmode			= false;
	WdModeID_t 			wd_mode			= wavedisplay_struct::FULLID;
	uint16_t			cursor			= 0;		// display current sound location in case of full wd_mode

} ;
typedef WD_data_struct 	WD_data_t;

const uint8_t 			WD_OSC_SIZE 	= NOOSCID;
const size_t 			WD_ROLES_SIZE 	= NOROLE;
const uint8_t 			WD_MODE_SIZE 	= wavedisplay_struct().types.size();

const size_t 			wavedisplay_len	= 512;

typedef array< Data_t,	wavedisplay_len>wd_arr_t;

typedef complex<double>	cd_t;
typedef vector<cd_t>	cd_vec_t;
extern wd_arr_t 		fft				(cd_vec_t data, bool invert);

#endif /* WAVEDISPLAY_BASE_H_ */
