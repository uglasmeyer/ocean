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

class AudioVolume_class :
	Logfacility_class
{
	string className = "";
	interface_t* sds;

public:
	Dynamic_class DynVolume { volidx_range };

	AudioVolume_class( interface_t* _sds) :
		Logfacility_class("MasterVolume_class")

	{
		className = Logfacility_class::className;
		this->sds = _sds;
		DynVolume.SetupVol( sds->Master_Amp,	FIXED ); //set start and master_volume
	};

	~AudioVolume_class() = default;

	void Transform( buffer_t frames, stereo_t* src, stereo_t* dst )
	{
		float 		balanceL 	= ( 100.0 - sds->mixer_balance ) / 200.0;
		float 		balanceR	= 1.0 - balanceL;

		DynVolume.SetDelta( sds->slide_duration);
		for( buffer_t n = 0; n < frames ; n++ )
		{
			float
			vol_percent 	= DynVolume.Get();
			dst[n].left 	= src[n].left	* vol_percent * balanceL ;
			dst[n].right 	= src[n].right	* vol_percent * balanceR ;
		}
		DynVolume.Update();

	}


private:
};



#endif /* AUDIOVOLUME_H_ */
