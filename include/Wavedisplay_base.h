/*
 * Wavedisplay_base.h
 *
 *  Created on: Sep 9, 2024
 *      Author: sirius
 */

#ifndef WAVEDISPLAY_BASE_H_
#define WAVEDISPLAY_BASE_H_

#include <Ocean.h>

typedef complex<double>	cd_t;
typedef vector<cd_t>	cd_vec_t;


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
	enum
	{
		FULLID,
		FLOWID,
		DEBUGID,
		FFTID
	};

};
typedef wavedisplay_struct wavedisplay_t;

struct WD_status_struct
{	// SDS related

	uint8_t oscId 	= osc_struct::OSCID;
	uint8_t roleId 	= osc_struct::INSTRID;
	bool 	fftmode = false;
	uint8_t wd_mode	= wavedisplay_struct::FULLID;

} ;
typedef WD_status_struct 		wd_status_t;

const size_t WD_OSC_SIZE 	= osc_struct().types.size();
const size_t WD_ROLES_SIZE 	= osc_struct().roles.size();
const size_t WD_MODE_SIZE 	= wavedisplay_struct().types.size();

const size_t wavedisplay_len		= 512;

typedef array< Data_t,	wavedisplay_len> 	wd_arr_t;

extern wd_arr_t fft(cd_vec_t data, bool invert);

#endif /* WAVEDISPLAY_BASE_H_ */
