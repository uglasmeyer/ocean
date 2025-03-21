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

uint8_t Dynamic_class::setup(	int _future,
								int _mode)
{
	this->mode		= _mode;
	if ( past == 0 ) // initialize past
	{
		mode = FIXED;
	}
	if ( _mode == FIXED )
	{
		present 	= future_f;
		past		= future;
	}
	return past;

}

uint8_t Dynamic_class::SetupVol(int future_vol,
								int _mode)
{
	future		= check_range( range, future_vol );
	future_f 	= future * 0.01;
	past 		= setup( future, _mode );
	if (_mode == FIXED ) // else : past was unchanged
		past_f		= past * 0.01;

	return future;
}

uint8_t Dynamic_class::SetupFrq(int future_frq,
								int _mode)
{
	future		= check_range( range, future_frq );
	future_f	= Frequency.Calc( future );
	past		= setup( future, _mode);
	if (_mode == FIXED ) // else : past was unchanged b< setup
		past_f		= Frequency.Calc( past );

	return future;
}
void Dynamic_class::SetDelta ( const uint8_t& sl_duration )
{
	if( sl_duration == 0 ) // disable dynamic
	{
		this->mode = FIXED;
		past = future;
		past_f = future_f;
		present = future_f;
		delta = 0.0;
		return;
	}
	else
	{
		this->mode = SLIDE;
	}
	buffer_t 	slide_frames	= slideFrames( sl_duration );
//	float 		delta_value		= this->future_f - this->past*0.01;
	float 		delta_value		= this->future_f - this->past_f;
				delta			= delta_value / slide_frames;
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
	if( this->mode == FIXED )
		return future_f;
	if( not fcomp( present , future_f, 2*delta ) )
	{
		present		= present + delta;
	}
	return present;
}

void Dynamic_class::Update()
{
	if( fcomp( present, future_f, 2*delta ) )
	{
		past 	= future;
		past_f 	= future_f;
		present = future_f;
		Show( true );
	}
}


void Dynamic_class::Show( bool on )
{
	Comment( TEST,	" mode   : " + to_string( mode) +
					" slide% : " + to_string( slideduration ) +
 					" past   : " + to_string( past ) +
					" present: " + to_string( present ) +
					" future : " + to_string( future_f ) +
					" delta  : " + to_string( delta ));
}

void Dynamic_class::Test()
{
	TEST_START( className );
	assert( check_range( range,    2 ) == 2 );
	assert( check_range( range,  112 ) == 100 );

	Dynamic_class	TestVol { volume_range };
	TestVol.SetupVol(50, FIXED );
	TestVol.SetupVol(26, SLIDE );
	TestVol.SetDelta( 1 );
	for( uint n = 0; n < test_frames - 5 ; n++ )
	{
		TestVol.Get();
	}
	TestVol.Update();
	TestVol.Show( true );
	ASSERTION( fcomp( 	TestVol.present+5*TestVol.delta, TestVol.future_f ), "Dyntest Vol",
						TestVol.present+5*TestVol.delta, TestVol.future_f );
	TestVol.SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		TestVol.Get();
	}
	TestVol.Update();
	TestVol.Show( true );
	ASSERTION( fcomp( 	TestVol.present, TestVol.future_f ), "Dyntest Vol",
						TestVol.present, TestVol.future_f );

	// Test Frequency
	//
	Dynamic_class	TestFrq { volume_range };
	TestFrq.SetupFrq(C0, FIXED );
	TestFrq.SetupFrq(71, SLIDE );
	TestFrq.SetDelta( 1 );
	for( uint n = 0; n < test_frames - 5 ; n++ )
	{
		TestFrq.Get();
	}
	TestFrq.Update();
	TestFrq.Show( true );
	assert( fcomp( 	TestFrq.present+5*TestFrq.delta, TestFrq.future_f, TestFrq.delta ));

	TestFrq.SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		TestFrq.Get();
	}
	TestFrq.Update();
	TestFrq.Show( true );
	ASSERTION( fcomp( 	TestFrq.present, TestFrq.future_f ), "Dyntest Frq",
						TestFrq.present, TestFrq.future_f );

	TestFrq.SetupFrq(100, FIXED );
	TestFrq.SetupFrq(71 , SLIDE );
	TestFrq.SetDelta( 0 );
	for( uint n = 0; n < test_frames ; n++ )
	{
		TestFrq.Get();
	}
	TestFrq.Update();
	TestFrq.Show( true );
	ASSERTION( fcomp( 	TestFrq.present, TestFrq.future_f ), "Dyntest Frq",
						TestFrq.present, TestFrq.future_f );

//	assert( false );

	TEST_END( className );


}



