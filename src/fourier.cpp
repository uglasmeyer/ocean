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

/**************************************************
 * Fourier_class
 *************************************************/
bool TEST_FFT = false;

void fft(cd_vec_t& x)
{
   int		N	= x.size();
   if (N <= 1) return;

   // Split into even and odd elements
   cd_vec_t even(N / 2), odd(N / 2);
   for (int i = 0; i < N / 2; ++i)
   {
       even[i]	= x[i * 2];
       odd[i]	= x[i * 2 + 1];
   }

   // Recursive FFT calls
   fft(even);
   fft(odd);

   // Combine results
   for (int k = 0; k < N / 2; ++k)
   {
       cd_t t	= std::polar(1.0, -2 * M_PI * k / N) * odd[k];
       x[k]		= even[k] + t;
       x[k+N/2] = even[k] - t;
   }
}


Fourier_class::Fourier_class() :
	Logfacility_class("Fourier_class")
{
	this->resolution	= 128;

};
Fourier_class::~Fourier_class()
{
	DESTRUCTOR( className );
};


void Fourier_class::norm_wd_size( uint max )
{
	if( cd_data.size() == max )
	{
		return;
	}
	if( cd_data.size() < max )
	{
		for( uint n = cd_data.size() ; n < max; n++ )
			cd_data.push_back( cd_t( 0, 0 ) );
	}
	else
	{
		for( uint n = cd_data.size(); n > max; n-- )
			cd_data.pop_back();
	}
}

void Fourier_class::genCdData( wd_arr_t& data )
{
	cd_data.clear();
	for( uint n = 0; n < data.size(); n++ )
	{
		cd_data.push_back( cd_t( float( data[n]) , 0.0 ) );
	}
}
wd_arr_t Fourier_class::Amplitude( wd_arr_t data )
{
	genCdData	( data );
	norm_wd_size( wavedisplay_len );
	wd_arr_t	wd_arr	{0};
	uint		n 		= 0;
	float		max		= 0;

				fft		( cd_data );

	std::ranges::for_each( cd_data, [ &wd_arr, &n, &max ]( cd_t cd )
	{
		Data_t A = abs( cd );
		wd_arr[n] = A;
		if( A > max )
			max = A;
		n++;
	});
	n = 0;
	std::ranges::for_each( wd_arr, [ max ]( Data_t& wd )
	{
		wd = wd * max_data_amp / max ;
	});
	genCoeff();
	return wd_arr;
};

/*
https://www.electronics-lab.com/article/the-fourier-analysis-the-fast-fourier-transform-fft-method/
*/
void Fourier_class::initIdxvec( )
{
	idx_vec.clear	();
	uint size		= cd_data.size();
	uint len		= size / 2;
	uint N 			= 0;
	for( ; N < len; N++ )
	{
		idx_val_t idx_val	{ N, 8*N*dt*2*M_PI, (float)abs( cd_data[N] ), arg( cd_data[N] ) };
		idx_vec.push_back	( idx_val );
	}
}
void Fourier_class::findPeaks()
{
	coeff_vec.clear();
	for( uint N = 1; N < idx_vec.size()-1 ; N++ )
	{
		phi_t prev	= (idx_vec[N-1	].amp);
		phi_t val	= (idx_vec[N	].amp);
		phi_t next	= (idx_vec[N+1	].amp);
		if ( ( val > prev) and ( val > next ) )
		{
			coeff_vec.push_back( idx_vec[N] );
		}
	};
}

void Fourier_class::genCoeff()
{
	Comment				(DEBUG, "Fourier_class::Reduce" );

	initIdxvec			();
	findPeaks			();
	std::ranges::sort	( coeff_vec, Greater );
	normCoeff			( 16 );
}

void Fourier_class::normCoeff( uint max_count )
{
	float	sum 		= 0.0;
	uint	len 		= coeff_vec.size();
			max_count	= min( len, max_count );

	for( uint n = 0; n < len; n++ )
	{
		if( n < max_count )
			sum 	+= ( coeff_vec[n].amp );
		else
			coeff_vec.pop_back();
	}

	if ( sum < 1E-6 )
		return;
	for( uint n = 0; n < coeff_vec.size(); n++ )
	{
		coeff_vec[n].amp = coeff_vec[n].amp / sum;
	}
}

void Fourier_class::show_coeff( fourier_vec_t coeff  )
{
	float r_sum = 0;
	for( idx_val_t val : coeff )
	{
		coutf << val.idx << '\t' << val.amp << '\t' << val.dphi << '\t' << val.phi << endl;
		r_sum += (val.amp);
	}
	coutf << "sum amp; " << r_sum  << endl;
};

Data_t Fourier_class::Fnc( param_t& p )
{
	Data_t	sum 	= 0.0;
	phi_t	rphi	= p.dphi / coeff_vec[0].dphi;
	std::ranges::for_each( coeff_vec, [ &sum, rphi ]( idx_val_t& val )
	{
				sum		+=val.amp * sin( val.phi );
		float	phi		= val.phi + rphi *  val.dphi;
				val.phi = MODPHI( phi, 2*M_PI ) ;
	});
	return sum;
}


void Fourier_class::TestFourier()
{
	TEST_START( className );
	TEST_FFT = true;
	show_coeff( coeff_vec );

/*	cd_vec_t data = {1, 2, 3, 4, 5};
	norm_wd_size( data, 4 );
	wd_arr_t amp = Amplitude( data );

	for (uint n = 0; n<4;n++)
		std::cout << amp[n] << "\n";
	for ( cd_t cd : cd_data )
		cout << cd << endl;
	Assert_equal( (int)amp[1], (int)4634 );
*/	TEST_FFT = false;
	Assert_equal( (uint16_t)1, resolution );
	TEST_END( className );
}

