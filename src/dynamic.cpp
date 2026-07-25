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
/**************************************************
 * Dynamic_class
 *************************************************/
Dynamic_class::Dynamic_class( DYNTYPE_e _type, range_T<int> _r )
	: Logfacility_class("Dynamic_class")
	, Frequency_class()

{
	this->dynface		= _type;
	range_T<uint8_t> 	tmp { (uint8_t)(_r.min), (uint8_t)(_r.max) };
	this->range			= tmp;
	this->slide_value 	= 0;
	this->restorestate	= state_struct();
	this->current		= state_struct();

};
Dynamic_class::~Dynamic_class()
{
	DESTRUCTOR( className )
};



uint8_t Dynamic_class::Setup( uint8_t future, DYNAMIC_e mode )
{
	current.future		= check_range( range, future, "Dynamic_class::Setup" );
	switch ( dynface )
	{
		case DYNTYPE_e::FRQ : { current.future_f	= GetFrq( current.future ); break; }
		case DYNTYPE_e::VOL : { current.future_f	= current.future * percent; break; }
		default : { break; }
	} // switch  dyntype
	if ( current.mode	== INIT ) // initialize past
	{
		current.past 	= current.future;
		current.mode 	= FIXED;
	}
	else
	{
		current.mode 	= check_range( dynamic_valrange, mode, "" );
	}
	if ( current.mode 	== FIXED )
		end();
	return current.future;

}

void Dynamic_class::setup_past()
{
	if ( current.mode 	== FIXED )
		end();
//  else: change past only if loop ends.
	// This will be done elsewhere
//	restorestate		= current; // used by dynamic frequency reset in osc.cpp
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

void Dynamic_class::SetDelta ( uint8_t sl_value )
{
	if( sl_value == 0 ) // disable dynamic
	{
		current.mode 	= FIXED;
		current.delta 	= 0.0;
		end				();
		set_state		();
		return;
	}
	const buffer_t 	slide_frames	= slideFrames( sl_value );
	const float		delta_value		= current.future_f - current.past_f;
					current.delta	= abs( delta_value / slide_frames );
					current.mode 	= SLIDE;
					set_state		();
};

buffer_t Dynamic_class::slideFrames( uint8_t value )
{
				slide_value 	= check_range( range, value, "slideFrames" );
	float		slide_percent 	= float( slide_value) * percent ;
	buffer_t 	frames 			= max_slide_frames * slide_percent ;

	if (LogMask[TEST])
		frames = test_frames;

	return 	frames;
}


dynamic_state_t Dynamic_class::GetCurrent()
{
	return current;
}
void Dynamic_class::SetCurrent( dynamic_state_t state )
{
	current = state;
}
float Dynamic_class::Get( )
{
	if( current.mode == FIXED )
		return current.future_f;
	if( fcomp( current.present , current.future_f, current.delta ) ) // ||p| - |f|| <= |d|
	{
		current.mode 	= FIXED;
		end();
	}
	else
	{
		if ( current.present > current.future_f )
		{
			current.present	= current.present - current.delta;
//			Assert_lt( current.future_f, current.present+0.0001f, "current.delta: " + to_string( current.delta ));
		}
		else
		{
			current.present	= current.present + current.delta;
//			Assert_lt( current.present, current.future_f+0.0001f, "current.delta: " + to_string( current.delta ));
		}
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
	current.past 	= current.future  ;
	current.past_f	= current.future_f;
	current.present	= current.future_f;
	restorestate	= current;
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
					(uint) slide_value,
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

	Setup(100, FIXED );
	Setup(71 , SLIDE );
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
	Show( true );
	Assert_lt( double(abs(-current.present + current.future_f + dn*current.delta)), 1E-4  , "Dyntest Vol" );
	SetDelta( 1 );
	for( uint n = 0; n < test_frames  ; n++ )
	{
		Get();
	}
	Update();
	Show( true );
	Assert_equal( current.present, current.future_f , "Dyntest Vol" );

//	assert( false );

	TEST_END( className );


}



