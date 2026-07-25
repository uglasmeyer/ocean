/**************************************************************************
MIT License

Copyright (c) 2026 Ulrich Glasmeyer

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
 * Fourier.h
 *
 *  Created on: Mar 9, 2026
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef FOURIER_H_
#define FOURIER_H_


#include <Wavedisplay_base.h>
#include <complex>
#include <Logfacility.h>




/**************************************************
 * Fourier_class
 *************************************************/
const uint				MaxResolution	= 64;
typedef complex<double>	cxd_t;
typedef vector<cxd_t>	cxd_vec_t;


class Fourier_class :
	virtual public Logfacility_class
{
	const phi_t dphi = update_frequency*dt*2*M_PI;

	typedef struct idx_val_data
	{
		phi_t	dphi= 0.0;
		float	amp = 0.0;
		phi_t	phi	= 0.0;
	} idx_val_t;
	typedef vector<idx_val_t>
					fourier_vec_t	;
	typedef array<idx_val_t, MaxResolution>
					fourier_arr_t	;

	struct
	{
		bool operator()( idx_val_t& a, idx_val_t& b) const
		{
			return ( abs(a.amp) > abs(b.amp) ) ;
		}
	} Greater;

	uint			resolution		;
	uint8_t			resolutionIdx	;
	float			A0				; // A0*cos(0)
	range_T<uint>	resolution_range{ 8, MaxResolution };
	cxd_vec_t		cd_data			;
	fourier_vec_t	idx_vec			{};
	fourier_vec_t 	coeff_vec		{};

public:
					Fourier_class	();
	virtual			~Fourier_class	();
	void	 		Amplitude		( wd_arr_t& data );
	Data_t 			Fnc				( param_t& p );
	void			TestFourier		();
	void			Save_coeff		( string filename );
	void			Load_coeff		( string filename );
	void 			GenCoeff		();
	void 			SetResolution	( uint8_t idx );


private:

	void 			findPeaks		();
	void 			gen_cxd_vec		( const wd_arr_t& data );
	float			init_idx_vec		();
	void 			normCoeff		();
	void 			norm_cxd_vec_len		( uint max );
	void			show_coeff		();
};

extern Fourier_class* fourier;	// initialzed in fourier.cpp


#endif /* FOURIER_H_ */
