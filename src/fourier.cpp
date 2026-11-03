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
#include <Table.h>


/**************************************************
 * osc_fourier_struct
 *************************************************/
void fft_file_struct::Read( string filename )
{
	void* 	data_p	= GetData_p();
	loadData		( filename, data_p, 0 ); // if empty arr consists of default fft_data
}
void fft_file_struct::Save( string filename )
{
	size_t	bytes	= sizeof( data );
	void*	data_p	= GetData_p();
	dumpData		( filename, data_p, bytes );
}
fft_file_struct::fft_file_data_t*
	fft_file_struct::GetData_p()
{
	return &data;
};
void fft_file_struct::SetOsc_Data( OSCID_e oscid, const Fourier_class::fft_arr_t& arr )
{
	data.osc[oscid] 	= arr;
};
void fft_file_struct::SetAdsrData( OSCID_e oscid, const Fourier_class::fft_arr_t& arr )
{
	data.adsr[oscid]	= arr;
}

/**************************************************
 * Fourier_class
 *************************************************/
bool TEST_FFT = false;

struct
{
	bool operator()( Fourier_class::fft_data_t& a, Fourier_class::fft_data_t& b) const
	{
		return ( abs(a.amp) > abs(b.amp) ) ;
	}
} Greater;

struct
{
	bool operator()( Fourier_class::fft_data_t& a, Fourier_class::fft_data_t& b) const
	{
		return ( a.idx < b.idx ) ;
	}
} Cindex;

using namespace fourier_ns;

#pragma GCC optimize("O3", "fast-math")
void Fourier_class::fft( cxd_vec_t& x )
{
	// perform the recusive fft analyzis of the complex data vector
	// it must be of this 2^N, N>2

	int		N	= x.size();
	if (N <= 1) return;

	// Split into even and odd elements
	cxd_vec_t even(N / 2);
	cxd_vec_t odd (N / 2);
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
		x[k]	= even[k] + t;
		x[k+N/2]= even[k] - t;
	}
}


Fourier_class::Fourier_class( string name ) :
	Logfacility_class("Fourier_class")
	, resolution	( resolution_max )
{
	this->osc_typename		= name;
	this->amp_limit			= percent;
	resolution.Idx2Res		( 0 );
	coeff_vec.clear			();
};

Fourier_class::~Fourier_class()
{
	DESTRUCTOR( className );
};

void Fourier_class::SetResolution( uint8_t idx )
{
	resolution.Idx2Res( idx );
}

uint16_t	Fourier_class::GetResolution()
{
	return coeff_vec.size();//resolution.Idx;
}
bool Fourier_class::Is_ready()
{
	return ( coeff_vec.size() > 0 );
}

void Fourier_class::resize_cxdata( uint max )
{	// if the data size is not equal to the given size (max)
	// fill up or reduce the vector accordingly
	// make sure the data size is multiple of 2
	if( TEST_FFT )
		max = 8;
	cxd_data.resize( max );
	return;
}


void Fourier_class::Write_fftdata( fftarr_t& sdsfft_data )
{	// write coefficient amplitude to sds

	fftarr_t arr { 0 };	// clear arr
	for_each( coeff_vec,
			[ this, &arr ]( fft_data_t  c )	{ arr[c.idx] = c.amp; } );
	sdsfft_data = arr;
}

void Fourier_class::Amplitude( const wd_arr_t& wd_data )
{
	// The workflow is completely managed by: Instrument - Gen_fftAmplitude
	// and triggered by: GUI - spectrum dialog
	// fft trigger-> Amplitude -> Generate coefficients -> write coeffience to sds
	// assign wd_data to complex cd_data
	// perform the fft analysis on cd_data

	size_t	len	= wd_data.size();
	assert( len == wavedisplay_len );

	auto gen_cxd_data	= [ this, &wd_data ]( )
	{	// create a complex vector z = (data, 0 ) out of wave display data

		cxd_data.clear();
		for_each( wd_data,
				[ this ]( float data )
				{ cxd_data.push_back( cxd_t( data, 0.0 ) ); } );
	};

	gen_cxd_data	();
	fft				( cxd_data );
	cxd_data.resize	( wavedisplay_len / 2 ); // use only the cos section

};

/*
https://www.electronics-lab.com/article/the-fourier-analysis-the-fast-fourier-transform-fft-method/
*/
#pragma GCC optimize("O3", "fast-math")
Data_t Fourier_class::Fnc( param_t& p )
{	// Fnc(t) = Fnc(n*dt) = Σ A_i * cos( 2pi w_i )
	Data_t		sum 	= 0.0f;
//	const phi_t	dphi	= p.dphi; 	// dphi = 2pi w dt, (w=1)
									// devide FFT.idx by 2 outside the loop
									// so that FFT.idx can be an integer
	for_each( coeff_vec.begin(), coeff_vec.end(),
			[ &sum, p ]( fft_data_t& FFT )
			{					// phi0   + freq * dt
				phi_t	phi		= FFT.phi + p.dphi * FFT.idx ;
						sum		+=FFT.amp * cos( phi );
						FFT.phi	= MODPHI( phi, p.maxphi ) ; // update coeff_vec to prepare the next sum
			});
	return sum;
}

void Fourier_class::Shift_Coeff( int shift )
{
	if( shift == 0 ) return;
	if( coeff_bkp.size() < 1 ) return;

	coeff_vec	= coeff_bkp;

	const int	maxidx		= fft_vec.size() - 1;
	for_each( coeff_vec.begin()+1, coeff_vec.end(),
			[ shift, maxidx ]( fft_data_t& c )
			{
				int	idx	= c.idx + shift;
				if ( ( idx < 1 ) or ( idx > maxidx ) )
					c.amp = 0.0f;
				else
					c.idx = c.idx + shift ;
			}
			);
	norm_coeff_vec();
}

void Fourier_class::GenCoeff( fourier_data_t& fft_data )//FILTER_e filter, int8_t shift )
{

	SetResolution 			( fft_data.residx );
	init_fft_vec			();
	init_coeff_vec			( fft_data );
	norm_coeff_vec			();
	coeff_bkp				= coeff_vec;
	fft_data.size			= GetResolution();

	show_coeff				();
	Info					( "Fourier coefficients generated" );
};

void Fourier_class::norm_coeff_vec(  )
{
	double sum = 0.0;
	for_each( coeff_vec,
			[ &sum ]( fft_data_t data ){ sum += data.amp; });
	for_each( coeff_vec,
			[ sum ]( fft_data_t& data ){ data.amp /= sum ;});
};

void Fourier_class::init_fft_vec()
{	// convert z from cxd_data to polar coordinates arg(z) and abs(z)


	auto modulo	= []( const phi_t& x, const phi_t n )
	{
		float 	d 	= floor ( x / n ); // floor corresponds to the gaussian bracket
		return 	x - d*n;
	};

	int	N			= 0;
	fft_vec.clear	();
	for_each	( cxd_data,
				[ this, modulo, &N ]( cxd_t z )
				{
					float		A		= abs( z );
					fft_data_t	fft_val	= { N,			// multiply dphi by N/2 see osc dT
											A  ,
											modulo( arg( z ), 2*M_PI ) };
					fft_vec.push_back	( fft_val );
					N++;
				}	);
}

void Fourier_class::init_coeff_vec( fourier_data_t& fft_data )
{

	auto get_min_index = [ this ](  )
	{
		if( coeff_vec.size() < 3 ) return coeff_vec.back().idx;
		sort		( coeff_vec.begin()+1, coeff_vec.end(), Cindex );
		int minidx	= coeff_vec[1].idx;
		return minidx-1;
	};

	auto isPeakVal = [ this ]( const uint& n )
	{
		phi_t prev	= ( this->fft_vec[n-1].amp);
		phi_t val	= ( this->fft_vec[ n ].amp);
		phi_t next	= ( this->fft_vec[n+1].amp);
		if ( val > std::max( prev, next ) )
			coeff_vec.push_back( this->fft_vec[n] );
	};

	auto automatic = [ this ]( fft_data_t  data )
	{
		if( data.amp > amp_limit )
			coeff_vec.push_back( data );
	};

	auto max_fft_vec_amp = [ this ](  )
	{
		if( fft_vec.size() == 0 )
			return 1.0f;
		float	max_amp = 0.0f;
		for_each( fft_vec,
				[ this, &max_amp ](fft_data_t data )
				{ if( data.amp > max_amp )
					max_amp = data.amp ;});
		return max_amp;
	};

	coeff_vec.clear();
	if ( fft_vec.size() < 4 )
		return;
	switch ( fft_data.filter )
	{
		case PEAKS	: {	coeff_vec.push_back( fft_vec[0] );
						for_each( fft_vec.begin()+1, fft_vec.end()-1,
								[ this, isPeakVal ]	( fft_data_t add ){ isPeakVal( add.idx ); }
								);
						coeff_vec.resize( resolution.Res );
						break; }
		case AUTO	: { amp_limit = max_fft_vec_amp() * percent;
						coeff_vec.push_back( fft_vec[0] );
						for_each( fft_vec.begin()+1, fft_vec.end(),
								[ this, automatic ]	( fft_data_t add ){ automatic( add ); }
								);
						int	shift	= - get_min_index();
						Shift_Coeff	( shift );
						break; }
		case SIZE 	: { sort	( fft_vec.begin()+1, fft_vec.end(),  Greater );
						for_each( fft_vec,
								[ this ]			( fft_data_t add ){ coeff_vec.push_back( add ) ;}
								);
						coeff_vec.resize( resolution.Res );
						break; }
		case NONE	: { for_each( fft_vec,
								[ this ]			( fft_data_t add ){ coeff_vec.push_back( add ) ;}
								);
						break; }
		default		: { break; }
	} // switch  filter_type
}

void Fourier_class::show_coeff()
{
	Table_class T 	{ "Fourier data " + osc_typename };
	T.AddColumn		( "N"	, 5 );
	T.AddColumn		( "Amp"	, 12 );
	T.AddColumn		( "phi"	, 12 );
	T.PrintHeader	();

	float r_sum		= 0;
	for( fft_data_t val : coeff_vec )
	{
		T.AddRow( val.idx, val.amp, val.phi );
		r_sum += (val.amp);
	}
	T.AddColumn		( ""	, 12 );
	T.AddRow( "Sum", r_sum, "Resolution", int( coeff_vec.size() ) );
};

Fourier_class::fft_arr_t Fourier_class::Coeff2Arr()
{
	fft_arr_t	arr		{ fft_data() };
	uint 		n		= 0;
	for_each( coeff_vec, [ &arr, &n ]( fft_data_t val )
	{
		arr[n] = val;
		n++;
	});
	return arr;
}

size_t Fourier_class::Arr2Coeff( fft_arr_t& arr )
{

	coeff_vec.clear		();
	for_each( arr, [ this ]( fft_data_t val )
	{
		if( val.amp > 0.0f )
			this->coeff_vec.push_back( val ) ;
	});

	show_coeff		();
	return 			coeff_vec.size();
}



void Fourier_class::TestFourier()
{
	TEST_START( className );

	cxd_data.resize( 16 );
	Assert_equal( cxd_data.size(), size_t( 16 ) );

	TEST_FFT=true;
	resize_cxdata( 13 );
	Assert_equal( cxd_data.size(), size_t( 8 ) );
	fourier_res_t Rest {64};
	Assert_equal( int( Rest.Res ), 8 );
	Assert_equal( 32, int( Rest.Idx2Res( Rest.Res2Idx( 32))) );


	coeff_vec.clear();
	SetResolution( 0 );
	wd_arr_t wd_data  = { 1, 2, 3, 4, 1, 2, 3, 4 };
	coutf << show_items( wd_data ) << endl;
	Amplitude( wd_data );
	fourier_data_t test = test_data;
	GenCoeff( test );

	TEST_FFT = false;
	TEST_END( className );
}

