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

#include <complex>

#include <Ocean.h>
#include <Wavedisplay_base.h>
#include <Logfacility.h>
#include <Range.h>

namespace fourier_ns
{
	enum MODE_e	: uint8_t
	{
		FOURIER,
		MANUAL,
		MODE_SIZE
	};
	enum FILTER_e : uint8_t
	{
		NONE,
		PEAKS,
		AUTO,
		SIZE,
		SHIFT
	};
	const vector<string> fourierfilter_txt =
	{
		"None",
		"Peaks",
		"Auto",
		"Size",
		"Shift"
	};
	typedef struct fourier_data_struct
	{	// SDS related
		uint8_t		residx	= 0;		// resolution 8,16,32, ...
		uint16_t	size	= 0;		// oscid spectrum size of coeff_vec
		int8_t		shift	= 0;		// shift the coeff_vec by value {-16,...0,...16}
		FILTER_e	filter	= AUTO;		// see init_coeff_vec
		bool		trigger	= false;	// true if spectrum dialog button "fourier" is activated
		MODE_e		mode	= MANUAL;	// state of the spectrum_dialog pb_fourier
	} fourier_data_t;
	const fourier_data_t	test_data	= { 0, 0, 0, AUTO, true, FOURIER };
}


typedef struct fourier_res_struct
{
	set<uint8_t> 		Set;
	vector<uint8_t>		Vec;
	vector<string> 		Str_vec {};
	range_T<uint8_t>	Idx_range{ 0, 5 }; // 2^(5+3) = 256
	uint8_t				MaxIdx;
	uint8_t				Idx;
	uint8_t				Res;

	fourier_res_struct( uint8_t maxres )
	{
		Res2Idx					( maxres );
		MaxIdx					= Idx;
		range_T<uint8_t> range	{  uint8_t( 0 ), MaxIdx };
		Idx_range				= range;

		for( uint8_t i = 0; i < (MaxIdx + 1); i++ )
		{
			uint8_t res = 1 << ( i + 3 );
			Set.insert			( res );
			Str_vec.push_back	( to_string( res ) );
			Vec.push_back		( res );
		}
		Idx						= 0;
		Res						= Idx2Res( Idx );
	}

	uint8_t	Idx2Res( uint8_t idx )
	{
		Idx			= idx;
		Res			= Vec[Idx];
		return 		Res;
	}
	uint8_t Res2Idx( uint8_t res )
	{
		float	f	= 	log( res ) / log(2) - 3;
		if( f < 0 )
			f = 0;
		Idx			= check_range( Idx_range, uint8_t( rint( f ) ) );
		Res 		= 1 << ( Idx + 3 );
		return Idx;
	}

} fourier_res_t;


/**************************************************
 * Fourier_class
 *************************************************/

const uint8_t resolution_max 		= 128;

class Fourier_class :
	virtual public Logfacility_class
{
	typedef std::complex<double>	cxd_t;
	typedef vector<cxd_t>			cxd_vec_t;
	cxd_vec_t		cxd_data		;	// result of the fft
	fourier_res_t	resolution		;
	string			osc_typename	;

public:
	typedef struct fft_data
	{
		int		idx = 0;
		float	amp = 0.0;
		phi_t	phi	= 0.0;			// phi evolves with time by dphi = idx*dt
	} fft_data_t;

	typedef vector<fft_data_t>
					fft_vec_t		;
		fft_vec_t 		coeff_vec		{};	// generate Fnc result

	typedef array<fft_data_t, resolution_max>
					fft_arr_t		;	// fourier file storage array per osc/adsr

					Fourier_class	( string name );
	virtual			~Fourier_class	();
	void	 		Amplitude		( const wd_arr_t& data );
	Data_t 			Fnc				( param_t& p );
	void 			GenCoeff		( fourier_ns::fourier_data_t& fft_data );
	uint16_t		GetResolution	();
	bool 			Is_ready		();
	size_t			Arr2Coeff		( fft_arr_t& arr );
	fft_arr_t		Coeff2Arr		( );
	void 			SetResolution	( uint8_t idx );
	void			Shift_Coeff		( int shift );
	void			TestFourier		();
	void 			Write_fftdata	( fftarr_t& data );


private:
	fft_vec_t		fft_vec			{};
	fft_vec_t		coeff_bkp		{};
	float			amp_limit	;

	void 			fft				( cxd_vec_t& x );
	void 			init_coeff_vec	( fourier_ns::fourier_data_t& fft_data );
	void			init_fft_vec	();
	void 			norm_coeff_vec	();
	void 			resize_cxdata	( uint max );
	void			show_coeff		();
};

/**************************************************
 * fft_file_data is the interface of fourier data vectors to the file system.
 * It holds temporary data for both, the osc and adsr
 * that has to be distributed to the osc and adsr fourier_class coefficient vectors
 * separately.
 * This allow us to use a single file to store the fft-coefficients for all oscillators
 *************************************************/


struct fft_file_struct
{
private:
	typedef array<Fourier_class::fft_arr_t, OSCIDSIZE>
						fft_file_arr_t;
	typedef struct fft_file_data
	{
		fft_file_arr_t	osc				{};
		fft_file_arr_t	adsr			{};
	} fft_file_data_t;
	fft_file_data_t	data 			= fft_file_data();

public:
					fft_file_struct	() = default;
					~fft_file_struct() = default;
	void 			Read			( string filename );
	void 			Save			( string filename );
	fft_file_data_t* GetData_p		();
	void			SetOsc_Data		( OSCID_e oscid, const Fourier_class::fft_arr_t& arr );
	void			SetAdsrData		( OSCID_e oscid, const Fourier_class::fft_arr_t& arr );
};



#endif /* FOURIER_H_ */
