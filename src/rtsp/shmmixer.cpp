/*
 * shmmixer.cpp
 *
 *  Created on: Oct 6, 2024
 *      Author: sirius
 */

#include <rtsp/ShmMixer.h>


void ShmMixer_class::InitShm()
{
	for( uint sdsid = 0; sdsid < MAXCONFIG ; sdsid ++ )
	{
		DaTA_p->SHM_vecL[sdsid].Clear();
		DaTA_p->SHM_vecR[sdsid].Clear();
	}

}

void operator+=( stereo_t& lhs, stereo_t& rhs)
{
	lhs.left += rhs.left;
	lhs.right+= rhs.right;
};

void ShmMixer_class::AddShm()
{
	if ( Log[ DEBUG ] ) cout.flush() << "#" ;
	if( DaTA_p->SDS_Id != 0 ) return; // not yor job

	stereo_t* audio_data = DaTA_p->GetShm_addr(  );
	if ( not audio_data ) return;

	assert( max_frames == DaTA_p->SHM_vecL[0].ds.size / sizeof(stereo_t) );

	for( uint sdsid = 1; sdsid < MAXCONFIG ; sdsid ++ )
	{
		stereo_t* shm_addr = DaTA_p->GetShm_addr( sdsid );
		if ( not shm_addr ) return;

		for( buffer_t n = 0; n < max_frames; n++ )
		{
			audio_data[n] += shm_addr[n];
		}
	}

}

