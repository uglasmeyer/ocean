/*
 * dynamic.cpp
 *
 *  Created on: Mar 17, 2025
 *      Author: sirius
 */


/**********************
 * Volume_class
 *********************/

#include <Dynamic.h>

Dynamic_class::Dynamic_class( range_t<int> _range )  :
Logfacility_class("Dynamic_class")
{
	className 	= Logfacility_class::className ;
	range		= _range;
};
Dynamic_class::~Dynamic_class()
{
};

uint8_t Dynamic_class::Set(int future_value,
						int _mode)
{
	future			= check_range( range, future_value );
	this->mode		= _mode;
	switch ( this->mode )
	{
		case COMBINE :
		case FIXED :
		{
			past		= future;
			present 	= future;
			present_pc 	= present * 0.01;
			break;
		}
		case SLIDE :
		{
			past	= rint( present );
			break;
		}
		default :
			assert( false );
			break;
	}
	return future;
}

constexpr buffer_t Dynamic_class::slideFrames( const uint8_t& sl_duration )
{
			slideduration 	= check_range( slide_duration_range, sl_duration );
	float	slide_percent 	= float( slideduration) * 0.01 ;
	return 	( 4 * max_frames * slide_percent );

}
void Dynamic_class::DeltaFrq( const uint8_t& sl_duration )
{
	buffer_t 	slide_frames	= slideFrames( sl_duration );
	float 		delta_value		= Frequency.Calc( this->future) -
								  Frequency.Calc( this->past  ) ;
				delta			= delta_value / slide_frames; // linear sliding
}

float Dynamic_class::GetFrq( )
{
	if( this->mode == FIXED )
		return present;
	if( not fcomp( present , (float) future, 1 ) )
	{
		present		= present + delta;
	}
	return present;
}

void Dynamic_class::DeltaVol ( const uint8_t& sl_duration )
{
	buffer_t 	slide_frames	= slideFrames( sl_duration );
	float 		delta_value		= this->future - this->past;
				delta			= delta_value / slide_frames;
};


float Dynamic_class::GetVol( )
{
	if( this->mode == FIXED )
		return present_pc;
	if( not fcomp( present , (float) future, 1 ) )
	{
		present		= present + delta;
		present_pc	= present * 0.01;
	}
	return present_pc;
}

void Dynamic_class::Update()
{
	if( fcomp( present, (float) future, 1 ) )
	{
		past 	= future;
		present = future;
//		Show( false );
	}
}
void Dynamic_class::Show( bool on )
{
	if ( on )
		Set_Loglevel( DBG2, true);
	Comment( DBG2,	" mode   " + to_string( mode) +
					" slide% " + to_string( slideduration ) +
 					" past   " + to_string( past ) +
					" present" + to_string( present ) +
					" future " + to_string( future ) +
					" delta  " + to_string( delta ));
	Set_Loglevel( DBG2, false );
}

#include <Frequency.h>
void Dynamic_class::Test()
{
	TEST_START( className );
	Set_Loglevel( DBG2, true );
	assert( check_range( range,    2 ) == 2 );
	assert( check_range( range,  112 ) == 100 );

	Dynamic_class	TestDyn { freqarr_range };
	TestDyn.Set(50, FIXED );
	TestDyn.Set(C0, SLIDE );
	TestDyn.DeltaVol( 1 );
	for( uint n = 0; n < 10000; n++ )
	{
		TestDyn.GetVol();
	}
	TestDyn.Update();
	TestDyn.Show( true );
	ASSERTION( fcomp( 	TestDyn.present, TestDyn.future ), "Dyntest ",
						TestDyn.present, TestDyn.future );
	Set_Loglevel( DBG2, false );
	TEST_END( className );


}



