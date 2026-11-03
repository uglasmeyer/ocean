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
 * dynamic.cpp
 *
 *  Created on: Mar 17, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include <Table.h>
#include <Dynamic.h>
#include <Range.h>
#include <math.h>

/**************************************************
 * Dynamic_class
 *************************************************/
Dynamic_class::Dynamic_class( DYNFACE_e _type, range_T<int> _r, range_T<int> _sl )
	: Logfacility_class("Dynamic_class")
	, Frequency_class()
	, value_range{ (uint8_t)(_r.min), (uint8_t)(_r.max) }
	, slide_range{ _sl.min, _sl.max }

{
	this->dynface		= _type;
//	this->restorestate	= dynamic_state_struct();
	this->current		= dynamic_state_struct();

};
Dynamic_class::~Dynamic_class()
{
	DESTRUCTOR( className )
};

uint8_t Dynamic_class::Setup( uint8_t future, DYNAMIC_e mode, size_t channel )
{
	current.future		= check_range( value_range, future, "Dynamic_class::Setup" );
	switch ( dynface )
	{
		case DYNFACE_e::FRQ : { current.future_f	= GetFrq( current.future ); break; }
		case DYNFACE_e::VOL : { current.future_f	= current.future * percent; break; }
		default : { break; }
	} // switch  dyntype
	if ( mode	== INIT ) // initialize past
	{
		current.past 	= current.future;
		current.mode 	= FIXED;
	}
	else
	{	// fixed->slide, init->combine
		current.mode 	= check_range( dynamic_valrange, mode, "dynamic_valrange" );
	}
	if ( current.mode 	== FIXED )
		end( channel );
	return current.future;

}

float Dynamic_class::Reset_state( size_t channel )
{
	current 	= dyn_state_arr[channel];//restorestate;
	return current.present;
}

void Dynamic_class::SetCurrent( dynamic_state_t state )
{
	current = state;
}

void Dynamic_class::SetDelta ( uint8_t sl_value, size_t channel  )
{
	auto slideFrames	= [ this, sl_value ]()
	{
		int	slide_value = check_range( slide_range, (int)sl_value, "slideFrames" );
		float 	frames	= max_slide_frames * slide_value / slide_range.max ;

		if (LogMask[TEST])
			frames = test_frames;

		return 	frames;
	};

	if( sl_value == 0 ) // disable dynamic
	{
		current.mode 	= FIXED;
		current.delta 	= 0.0;
		end				( channel );
		dyn_state_arr[channel] = current;
//		restorestate	= current;

		return;
	}
	current.delta	= abs( ( current.future_f - current.past_f ) / slideFrames() );
	current.mode 	= SLIDE;
	dyn_state_arr[channel] = current;
//	restorestate	= current;

};

dynamic_state_t Dynamic_class::GetCurrent()
{
	return current;
}

float Dynamic_class::Get( size_t channel )
{
	if( current.mode == FIXED )
		return current.future_f;

	// if p - f ~ 0
	if( fcomp( current.present , current.future_f, current.delta ) )
	{
		current.mode 	= FIXED;
		end( channel );
	}
	// if p - f != 0
	else
	{	// p = p + signum( f - p ) * |delta|
		current.present += copysign( current.delta, current.future_f - current.present);
	}
	return current.present;
}

void Dynamic_class::Update( size_t channel )
{
	if( fcomp( current.present, current.future_f, 2*current.delta ) )
	{
		end( channel );
	}
}

void Dynamic_class::end( size_t channel )
{
	current.past 	= current.future  ;
	current.past_f	= current.future_f;
	current.present	= current.future_f;
	dyn_state_arr[channel] = current;

//	restorestate	= current;
}

void Dynamic_class::show( bool on )
{
	Table_class	Table{ "Dynamics" };
	Table.AddColumn( "mode    ", 8 );
	Table.AddColumn( "past    ", 8 );
	Table.AddColumn( "present ", 8 );
	Table.AddColumn( "future  ", 8 );
	Table.AddColumn( "delta   ", 8 );
	Table.PrintHeader();
	Table.AddRow(	(uint) current.mode,
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

	Assert_equal( check_range( frqext_range, 200, "" ), int(FRQEXT_SIZE)-1 );

	Setup(C0, FIXED );
	Setup(71, SLIDE );
	SetDelta( 1 );
	for( uint n = 0; n < test_frames - 5 ; n++ )
	{
		Get();
	}
	Update();
	show( true );
	assert( fcomp( 	current.present+5*current.delta, current.future_f, current.delta ));

	SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		Get();
	}
	Update();
	show( true );
	ASSERTION( fcomp( 	current.present, current.future_f ), "Dyntest Frq",
			current.present, current.future_f );

	Setup(100, FIXED );
	Setup(71 , SLIDE );
	SetDelta( 0 );
	for( uint n = 0; n < test_frames ; n++ )
	{
		Get();
	}
	Update();
	show( true );
	ASSERTION( fcomp( 	current.present, current.future_f ), "Dyntest Frq",
			current.present, current.future_f );
	TEST_END( className );


}
void Dynamic_class::TestVol()
{
	TEST_START( className );
	assert( check_range( volidx_range,    2, "" ) == 2 );
	assert( check_range( volidx_range,  112, "" ) == 100 );

	Setup(50, FIXED );
	Setup(25, SLIDE );
	SetDelta( 10 );
	buffer_t dn = 100;
	for( uint n = 0; n < test_frames - dn ; n++ )
	{
		Get();
	}
	Update();
	show( true );
	Assert_lt( double(abs(-current.present + current.future_f + dn*current.delta)), 1E-4  , "Dyntest Vol" );
	SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		Get();
	}
	Update();
	show( true );
	Assert_equal( current.present, current.future_f , "Dyntest Vol" );

//	assert( false );

	TEST_END( className );


}



