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
 * fft.cpp
 *
 *  Created on: Jan 26, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

// https://cp-algorithms.com/algebra/fft.html

/*
 * fourier.cpp
 *
 *  Created on: Mar 8, 2026
 *      Author: Ulrich.Glasmeyer@web.de
 */

/*
 * harmonic analysis of oscData
frq		idx
  55 	  7
 110 	 14
 220 	 28 * 8
 440 	 55 * 8
 880 	110 * 8
1760	220 * 8
4096	512 * 8
 */

#include <Fourier.h>
#include <System.h>

extern 			Logfacility_class Log;

Fourier_class	Fourier	{};
Fourier_class*	fourier	= &Fourier;

/**************************************************
 * Fourier_class
 *************************************************/
bool TEST_FFT = false;

void fft(cxd_vec_t& x)
{
	// perform the recusive fft analyzis of the complex data vector
	// it must be of this 2^N, N>2

	int		N	= x.size();
	if (N <= 1) return;

	// Split into even and odd elements
	cxd_vec_t even(N / 2), odd(N / 2);
	for (int i = 0; i < N / 2; ++i)
	{
		even[i]	= x[i * 2];
		odd [i]	= x[i * 2 + 1];
	}

	// Recursive FFT calls
	fft(even);
	fft(odd);

	// Combine results
	for (int k = 0; k < N / 2; ++k)
	{
		cxd_t t	= std::polar(1.0, -2 * M_PI * k / N) * odd[k];
		x[k]		= even[k] + t;
		x[k+N/2] = even[k] - t;
	}
}


Fourier_class::Fourier_class() :
	Logfacility_class("Fourier_class")
{
	this->resolutionIdx = 0;
	this->resolution	= 8 << resolutionIdx;
	this->A0			= 0.0f;
};

Fourier_class::~Fourier_class()
{
	DESTRUCTOR( className );
};

void Fourier_class::SetResolution( uint8_t idx )
{
	resolution = check_range( resolution_range , uint(8 << idx) );
}

void Fourier_class::norm_cxd_vec_len( uint max )
{	// if the data size is not equal to the given size (max)
	// fill up or reduce the vector accordingly
	if( TEST_FFT )
		max = 8;
	if( cd_data.size() == max )
	{
		return;
	}
	if( cd_data.size() < max )
	{
		for( uint n = cd_data.size() ; n < max; n++ )
			cd_data.push_back( cxd_t( 0, 0 ) );
	}
	else
	{
		for( uint n = cd_data.size(); n > max; n-- )
			cd_data.pop_back();
	}
}

void Fourier_class::gen_cxd_vec( const wd_arr_t& data )
{	// create a complex vector out of wave display data
	cd_data.clear();
	for( uint n = 0; n < data.size(); n++ )
	{
		cd_data.push_back( cxd_t( float( data[n]) , 0.0 ) );
	}
}

void Fourier_class::Amplitude( wd_arr_t& wd_data )
{
	// assign wd_data to complex cd_data
	// perform the fft analysis on cd_data
	// return the normalized amplitude of the fft by reference

	float	max_fft_amp	= 0.0f;
	uint	n			= 0;

	auto store_amp = [ &max_fft_amp, &n, &wd_data ]( cxd_t cxd )
	{
		Data_t	A			= abs( cxd ); // sqrt(re²+im²)
				wd_data[n] 	= A;
				n++;

		if( A > max_fft_amp )
			max_fft_amp = A;
	};

	gen_cxd_vec		( wd_data );
	norm_cxd_vec_len( wavedisplay_len );
	fft				( cd_data );

	// store the absolute value of the complex amplitude
	// into the reference of wd_data
	// and the maximum in max_fft_amp
	std::ranges::for_each( cd_data, store_amp );

	// norm the wave display data to 1.0,
	// so that the sum over the wave display data is 1.0
	if( max_fft_amp > 0.0f )
	{
		std::ranges::for_each( wd_data, [ max_fft_amp ]( Data_t& wd )
		{	wd /= max_fft_amp;	});
	}
};

/*
https://www.electronics-lab.com/article/the-fourier-analysis-the-fast-fourier-transform-fft-method/
*/
float Fourier_class::init_idx_vec( )
{
	idx_vec.clear	();
	for( uint N = 0 ; N < ( cd_data.size() / 2 ); N++ )
	{
		idx_val_t idx_val	{	N * dphi,
								(float)abs( cd_data[N] ),
								arg( cd_data[N] ) };
		idx_vec.push_back	( idx_val );
	}
	return idx_vec[0].amp;
}

void Fourier_class::findPeaks()
{
	auto isPeakVal = [ this ]( uint n )
	{
		phi_t prev	= ( this->idx_vec[n-1].amp);
		phi_t val	= ( this->idx_vec[n	 ].amp);
		phi_t next	= ( this->idx_vec[n+1].amp);
		return ( val > max( prev, next ) );
	};

	coeff_vec.clear();
	coeff_vec.push_back( idx_vec[0] );
	if ( idx_vec.size() < 4 )
		return;
	uint 	len 		= idx_vec.size() - 1;
	uint	coeff_cnt	= 0;	// coeffcounter
	for( uint N = 1; N < len ; N++ )
	{
		if ( isPeakVal( N ) )
		{
			if( coeff_cnt < resolution )
			{
				coeff_vec.push_back( idx_vec[N] );
				coeff_cnt++;
			}
		}
	};
}

void Fourier_class::GenCoeff()
{

	A0 = init_idx_vec		();
	findPeaks				();
	if( coeff_vec.size() > 1 )
		std::ranges::sort	( coeff_vec.begin() + 1, coeff_vec.end(),  Greater );
	normCoeff			();
	Comment	(INFO, "Fourier coefficients of resolution", coeff_vec.size(), "generated" );
}

void Fourier_class::normCoeff()
{
	float		sum 		= 0.0;
	uint		len 		= coeff_vec.size();
	uint		max_count	= min( len, resolution );

	for( uint n = 0; n < len; n++ )
	{
		if( n < max_count )
			sum 	+= ( coeff_vec[n].amp );
	}

	if ( sum > 0.0f )
		std::ranges::for_each( coeff_vec, [ sum ]( idx_val_t& cx ){ cx.amp /= sum ;} );
}

void Fourier_class::Save_coeff( string filename )
{
	fourier_arr_t 	arr		{ idx_val_data() };
	size_t 			bytes	= sizeof( arr );
	uint 			n		= 0;
	std::ranges::for_each( coeff_vec, [ &arr, &n ]( idx_val_t val )
	{
		arr[n] = val;
		n++;
	});

	void* data		= &arr;
	bool success	= dumpData( filename, data, bytes );
	if ( success ) return;

	Log.Comment( ERROR, "writing", bytes, " bytes to file", filename );
}

void Fourier_class::Load_coeff( string filename )
{
	size_t bytes	= sizeof( fourier_arr_t );
	fourier_arr_t arr { idx_val_data() };
	void* data		= &arr;
	loadData( filename, data, bytes );

	coeff_vec.clear	();
	std::ranges::for_each( arr, [ this ]( idx_val_t val )
	{
		if( val.amp > 0.0f )
			this->coeff_vec.push_back( val ) ;
	});
}

void Fourier_class::show_coeff()
{
	float r_sum = 0;
	for( idx_val_t val : coeff_vec )
	{
		coutf << (int)val.dphi/dphi << '\t' << val.amp << '\t' << val.dphi << '\t' << val.phi << endl;
		r_sum += (val.amp);
	}
	coutf << "sum amp; " << r_sum  << endl;
};

Data_t Fourier_class::Fnc( param_t& p )
{
	Data_t		sum 	= 0.0;//idx_vec[0].amp;//0.0;
	if( coeff_vec.size() == 0 )
		return sum;
	sum = coeff_vec[0].amp;
	const phi_t	w0	= p.dphi / coeff_vec[1].dphi; // 2pi w dt / 2pi w0 dt
	std::ranges::for_each( coeff_vec.begin()+1, coeff_vec.end(), [ &sum, w0 ]( idx_val_t& C )
//			std::ranges::for_each( coeff_vec, [ &sum, w0 ]( idx_val_t& C )
	{
					sum		+=C.amp * cos( C.phi );
			phi_t	phi		= C.phi + w0 *  C.dphi;
							C.phi = MODPHI( phi, 2*M_PI ) ;
	});
	return sum;
}


void Fourier_class::TestFourier()
{
	TEST_START( className );

	TEST_FFT=true;
	coeff_vec.clear();
	wd_arr_t wd_data  = { 1, 2, 3, 4, 1, 2, 3, 4 };
	coutf << show_items( wd_data ) << endl;
	Amplitude( wd_data );
	GenCoeff();
	frq_t c1 = coeff_vec[1].amp;
	Save_coeff( "/tmp/coeff_vec.tmp" );
	show_coeff();
	Load_coeff( "/tmp/coeff_vec.tmp" );
	frq_t l1 = coeff_vec[1].amp;
	show_coeff();

	TEST_FFT = false;
	Assert_equal( c1, l1 );
	TEST_END( className );
}

