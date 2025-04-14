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

uint8_t Dynamic_class::SetupVol(int future_vol,	int _mode)
{
	if ( _mode == COMBINE )
		current.mode = SLIDE;
	else
		current.mode 	= _mode;
	current.future	= check_range( range, future_vol );
	current.future_f= current.future * 0.01;
	setup();
	return current.future;
}

uint8_t Dynamic_class::SetupFrq(int future_frq, int _mode)
{
	if ( _mode == COMBINE )
		current.mode = SLIDE;
	else
		current.mode 	= _mode;
	current.future	= check_range( range, future_frq );
	current.future_f= Frequency.GetFrq( current.future );
	setup();
	return current.future;
}
void Dynamic_class::setup()
{
	if ( current.past == 0 ) // initialize past
	{
		current.past = current.future;
		current.mode = FIXED;
	}
	if ( current.mode == FIXED )
		end();
}
float Dynamic_class::Reset_state()
{
	current 	= restorestate;
	return current.present;
}
void Dynamic_class::set_state()
{
	restorestate= current;
}

void Dynamic_class::SetDelta ( const uint8_t& sl_duration )
{
	if( sl_duration == 0 ) // disable dynamic
	{
		current.mode = FIXED;
		end();
		current.delta = 0.0;
		set_state();
		return;
	}
	else
	{
		current.mode = SLIDE;
	}
	buffer_t 	slide_frames	= slideFrames( sl_duration );
	float 		delta_value		= current.future_f - current.past_f;
	current.delta				= abs( delta_value / slide_frames );
	set_state();
	return;
};

constexpr buffer_t Dynamic_class::slideFrames( const uint8_t& sl_duration )
{
				slideduration 	= check_range( slide_duration_range, sl_duration );
	float		slide_percent 	= float( slideduration) * 0.01 ;
	buffer_t 	frames 			= 4 * max_frames * slide_percent ;
	if (LogMask[TEST])
		frames = test_frames;

	return 	frames;
}



float Dynamic_class::Get( )
{
	if( current.mode == FIXED )
		return current.future_f;
	if( fcomp( current.present , current.future_f, 2*current.delta ) )
	{
		current.mode 	= FIXED;
		end();
	}
	else
	{
		if ( current.present > current.future_f )
			current.present	= current.present - current.delta;
		else
			current.present	= current.present + current.delta;
	}
	return current.present;
}

void Dynamic_class::Update()
{
	if( fcomp( current.present, current.future_f, 2*current.delta ) )
	{
		end();
	}
}

void Dynamic_class::end()
{
	current.past 	= current.future;
	current.past_f	= current.future_f;
	current.present	= current.future_f;
}

void Dynamic_class::Show( bool on )
{

	Comment( TEST,	" mode   : " + to_string( current.mode) +
					" slide% : " + to_string( slideduration ) +
 					" past   : " + to_string( current.past ) +
					" present: " + to_string( current.present ) +
					" future : " + to_string( current.future_f ) +
					" delta  : " + to_string( current.delta ));
}

void Dynamic_class::TestFrq()
{
	// Test Frequency
	//
	TEST_START( className );
	range = freqarr_range ;
	SetupFrq(C0, FIXED );
	SetupFrq(71, SLIDE );
	SetDelta( 1 );
	for( uint n = 0; n < test_frames - 5 ; n++ )
	{
		Get();
	}
	Update();
	Show( true );
	assert( fcomp( 	current.present+5*current.delta, current.future_f, current.delta ));

	SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		Get();
	}
	Update();
	Show( true );
	ASSERTION( fcomp( 	current.present, current.future_f ), "Dyntest Frq",
			current.present, current.future_f );

	SetupFrq(100, FIXED );
	SetupFrq(71 , SLIDE );
	SetDelta( 0 );
	for( uint n = 0; n < test_frames ; n++ )
	{
		Get();
	}
	Update();
	Show( true );
	ASSERTION( fcomp( 	current.present, current.future_f ), "Dyntest Frq",
			current.present, current.future_f );
	TEST_END( className );


}
void Dynamic_class::TestVol()
{
	TEST_START( className );
	assert( check_range( range,    2 ) == 2 );
	assert( check_range( range,  112 ) == 100 );

	range = volume_range;
	SetupVol(50, FIXED );
	SetupVol(26, SLIDE );
	SetDelta( 1 );
	for( uint n = 0; n < test_frames - 5 ; n++ )
	{
		Get();
	}
	Update();
	Show( true );
	ASSERTION( fcomp( 	current.present-5*current.delta, current.future_f ), "Dyntest Vol",
			current.present+5*current.delta, current.future_f );
	SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		Get();
	}
	Update();
	Show( true );
	ASSERTION( fcomp( 	current.present, current.future_f ), "Dyntest Vol",
			current.present, current.future_f );

//	assert( false );

	TEST_END( className );


}



