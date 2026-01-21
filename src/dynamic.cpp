/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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
 * dynamic.cpp
 *
 *  Created on: Mar 17, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


/**********************
 * Volume_class
 *********************/
#include <Table.h>
#include <Dynamic.h>

Dynamic_class::Dynamic_class( range_T<int> _range )
	: Logfacility_class("Dynamic_class")
	, Frequency_class()
{
	className 	= Logfacility_class::className ;
	range		= _range;
};
Dynamic_class::~Dynamic_class()
{
};

uint8_t Dynamic_class::SetupVol(int future_vol,	DYNAMIC _mode)
{
	if ( _mode == COMBINE )
		current.mode = SLIDE;
	else
		current.mode	= _mode;
	current.future		= check_range( range, future_vol, "SetupVol" );
	current.future_f	= current.future * percent;
	setup_past();
	return current.future;
}

uint8_t Dynamic_class::SetupFrq(int future_frq, DYNAMIC _mode)
{
	if ( _mode == COMBINE )
		current.mode = SLIDE;
	else
		current.mode 	= _mode;
	current.future		= check_range( range, future_frq, "SetupFrq" );
	current.future_f	= GetFrq( current.future );
	setup_past			();
//	set_state();
	return current.future;
}
void Dynamic_class::setup_past()
{
	if ( current.past == 0 ) // initialize past
	{
		current.past 	= current.future;
		current.mode 	= FIXED;
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
		current.mode 	= FIXED;
		current.delta 	= 0.0;
		end				();
		set_state		();
		return;
	}
	const buffer_t 	slide_frames	= slideFrames( sl_duration );
	const float		delta_value		= current.future_f - current.past_f;
					current.delta	= abs( delta_value / slide_frames );
					current.mode 	= SLIDE;
					set_state		();
};

constexpr buffer_t Dynamic_class::slideFrames( const uint8_t& duration )
{
				slideduration 	= check_range( slide_duration_range, duration, "slideFrames" );
	float		slide_percent 	= float( slideduration) * percent ;
	buffer_t 	frames 			= 4 * max_frames * slide_percent ;

	if (LogMask[TEST])
		frames = test_frames;

	return 	frames;
}


dynamic_state_t Dynamic_class::GetCurrent()
{
	return current;
}
void Dynamic_class::SetCurrent( state_t state )
{
	current = state;
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
	Table_class	Table{ "Dynamics" };
	Table.AddColumn( "mode    ", 8 );
	Table.AddColumn( "slide   ", 8 );
	Table.AddColumn( "past    ", 8 );
	Table.AddColumn( "present ", 8 );
	Table.AddColumn( "future  ", 8 );
	Table.AddColumn( "delta   ", 8 );
	Table.PrintHeader();
	Table.AddRow(	(uint) current.mode,
					(uint) slideduration,
					(uint) current.past,
					current.present,
					current.future_f,
					current.delta
				);
}

void Dynamic_class::TestFrq()
{
	// Test Frequency
	//
	TEST_START( className );
	range = frqarr_range ;
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
	assert( check_range( range,    2, "" ) == 2 );
	assert( check_range( range,  112, "" ) == 100 );

	range = volidx_range;
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



