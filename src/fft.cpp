/*
 * fft.cpp
 *
 *  Created on: Jan 26, 2025
 *      Author: sirius
 */

#include <Wavedisplay_base.h>

// https://cp-algorithms.com/algebra/fft.html

//using cd_t = complex<double>;
const double PI = acos(-1);

wd_arr_t fft(cd_vec_t data, bool invert)
{
	// assure data_len is power of 2
	uint data_len = 512;// ;
    if ( data.size() < 512 )
    	data_len = 256;

    // prepare data
    for ( uint i = 1, j = 0; i < data_len; i++)
    {
        int bit = data_len >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(data[i], data[j]);
    }

    // fft data
    for ( uint len = 2; len <= data_len; len <<= 1)
    {
        double phi = 2 * PI / len * (invert ? -1 : 1);
        cd_t wlen(cos(phi), sin(phi));
        for ( uint i = 0; i < data_len; i += len)
        {
            cd_t w(1.0, 0.0);
            for ( uint j = 0; j < len / 2; j++)
            {
            	uint k 		= i+j;
                cd_t u 		= data[k];
                cd_t v 		= data[k+len/2] * w;
                data[k] 	= u + v;
                data[k+len/2]= u - v;
                w 			*= wlen;
            }
        }
    }

    if (invert)
    {
        for (cd_t& x : data)
            x /= data_len;
    }

    // prepare wavedisplay data from complex values
    wd_arr_t wd_arr;

    float max = 0;
	for( uint n  = 0; n < wavedisplay_len; n++)
	{
		if ( n < data_len / 2 )
		{
			wd_arr[n] = abs( real( data[n])  );
			if ( wd_arr[n] > max )
				max = wd_arr[n];
		}
		else
			wd_arr[n] = 0;

	}
	// in case of null data return
	if ( max == 0 )
		return wd_arr;

	// normalize wavedisplay data
	float norm = max_data_amp / max;
	for( uint n = 0; n < wavedisplay_len; n++ )
	{
		wd_arr[n] = wd_arr[n] * norm;
	}
	return wd_arr;

}
