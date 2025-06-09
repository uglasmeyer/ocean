/*
 * MasterVolume.h
 *
 *  Created on: May 8, 2025
 *      Author: sirius
 */

#ifndef AUDIOVOLUME_H_
#define AUDIOVOLUME_H_

#include <Logfacility.h>
#include <Dynamic.h>
#include <data/Interface.h>
#include <limits>

class AudioVolume_class :
	Logfacility_class
{
	string 			className 	= "";
	interface_t* 	sds			;

public:
	Dynamic_class DynVolume { volidx_range };

	AudioVolume_class( interface_t* _sds) :
		Logfacility_class( "AudioVolume_class" )
	{
		className = Logfacility_class::className;
		this->sds = _sds;
		DynVolume.SetupVol( sds->Master_Amp,	FIXED ); //set start and master_volume
	};

	~AudioVolume_class() = default;


	float dynamic_limit( buffer_t frames, Stereo_t* src )
	{
		data_t 	min = numeric_limits<data_t>::min();
		float 	max = min;
		for( buffer_t n = 0; n < frames; n++ )
		{
			if ( abs( src[n].left  ) > max ) max = abs( src[n].left  );
			if ( abs( src[n].right ) > max ) max = abs( src[n].right );
		}
		float ratio = 0.0;
		if ( max > 0 )
			ratio = max / abs(min) ;
//		cout << ratio << endl;
		if ( ratio > 1.0 )
			return 1.0 / ratio;
		return 1.0;
	}

	void Transform( buffer_t frames, Stereo_t* src, stereo_t* dst )
	{
		float 		ratio 		= dynamic_limit( frames, src );
		float 		balanceL 	= ( 100.0 - sds->mixer_balance ) / 200.0;
		float 		balanceR	= 1.0 - balanceL;

		DynVolume.SetDelta( sds->slide_duration);
		for( buffer_t n = 0; n < frames ; n++ )
		{
			float
			vol_percent 	= DynVolume.Get();
			dst[n].left 	= rint( src[n].left		* vol_percent * balanceL * ratio ) ;
			dst[n].right 	= rint( src[n].right	* vol_percent * balanceR * ratio ) ;
		}
		DynVolume.Update();

	}


private:
};



#endif /* AUDIOVOLUME_H_ */
