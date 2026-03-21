/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <Wavedisplay.h>

/**************************************************
 * Wavedisplay_class
 *************************************************/
Wavedisplay_class::Wavedisplay_class( SharedData_class* _sds, Fourier_class* _fourier )
: Logfacility_class("Wavedisplay")
{
	this->Sds_p 		= _sds;
	this->sds			= _sds->addr;
	this->fourier		= _fourier;
	this->param_flow	= param_struct();
	this->param_full 	= param_struct();
	this->param_split	= { 0, wavedisplay_len/2, 1, max_frames };
	this->param_fft		= { 0, wavedisplay_len  , 0, 0 };
	this->param_cursor	= { 0, wavedisplay_len	, 0, 0 };
	this->param_adsr	= { 0, adsrdisplay_len	, 0, 0 };
	this->frame_counter	= 0;
	this->offs 			= 0;

	this->Wd_ptr		= wd_ptr_struct();
	this->wd_frames		= 0;
	this->debug_right	= true;

	Assert_equal( roleNames.size()-1, (size_t) ROLE_SIZE );
}

// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len

void Wavedisplay_class::gen_wave_data( WdModeID_t mode )
{
	if ( wd_frames == 0 )
	{
		Comment( WARN, "Zero wavedisplay frames: ",	rolename, oscname, wd_frames );
		return;
	}

	auto gen_debug	= [ this ]( param_t param )
	{
		if ( debug_right ) // n = param.len ... 2*param.len - left data range -> back buffer range
		{	// fill this area thereafter
			debug_right = not debug_right;
			for ( buffer_t n = 0; n < param.len; n++ )
			{
				Data_t value = wd_ptr[n];
				display_data[ n + param.len  ] = value;   // right half
			}
		}
		else // n=0 ... param.len - right data range -> front buffer range
		{	// fill this area first
			debug_right = not debug_right;
			for ( buffer_t n = wd_frames - param.len; n < wd_frames; n++ )
			{
				Data_t value = wd_ptr[n];
				display_data[ n + param.len - wd_frames ] = value;
			}
		}
	};

	auto gen_full	= [ this ]( param_t param )
	{
		float 	df	= wd_frames / float( param.len ) ;
		if ( df < 1.0 ) 	return;
		float 	f	= 0.0;
		std::ranges::for_each( display_data, [ this, &f, df ]( Data_t& Data )
		{
			buffer_t 	m 	= f;
						Data= wd_ptr[m];
						f 	= f + df;
		} );
	};

	auto gen_fft 	= [ this, gen_full ]( param_t param )
	{
		gen_full	( param );
		display_data= fourier->Amplitude( display_data );
	};

	auto gen_flow 	= [ this ]( param_t param )
	{
		uint idx 	= 0;
		uint step	= param.step;
		for ( buffer_t n = offs; n < param.len*step + offs; n = n + step )
		{
			Data_t	value 				= wd_ptr[ n ] ;
					display_data[ idx ] = value;
					idx++;
		}
	};

	auto gen_adsr = [ this ]( param_t param )
	{
					set_Wdptr	( ADSRROLE );
		uint 		idx			= 0;
		uint		step		= rint( wd_frames / param.len );
		assert( step>0 );
		Assert_lt( int((step-1) * param.len), int(wd_frames) );

		for ( buffer_t n = 0; n < param.len; n++ )
		{
			display_data[ n ] 	= wd_ptr[idx];
			idx += step;
		}
		set_Wdptr( wd_data.roleId );
	};

	if ( wd_ptr == nullptr )
	{
		Comment( ERROR, "wave display got nullptr for: ",
						rolename,
						oscname,
						wd_frames );
		return;
	}

	param_t	param;
	switch ( mode )
	{
		case FULLID :
		{
			param = param_full;
			gen_full( param );
			break;
		}
		case FLOWID :
		{
			param = param_flow;
			param.max_offs = wd_frames - param.len*param.step;
			gen_flow( param );
			break;
		}
		case CURSORID :
		{
			param = param_full;
			gen_full( param );
			break;
		}
		case DEBUGID :
		{
			param = param_split;
			gen_debug( param );
			break;
		}
		case FFTID :
		{
			param = param_fft;
			gen_fft( param );
			break;
		}
		case ADSRID :
		{
			param = param_adsr;
			gen_adsr( param );
			break;
		}
		default :
		{
			Comment(ERROR,"No valid wave display type provided to gen_cxwave_data");
		}
	} // switch WdMode

	frame_counter++;
	offs = offs + param.drift * frame_counter;
	if ( offs > param.max_offs )
	{
		frame_counter 	= 0;
		offs			= 0;
	}
}

void Wavedisplay_class::WriteData()
{
	if( wd_data.adsrmode )
	{
		gen_wave_data( ADSRID );
		Sds_p->Write_adsr_arr( display_data );
	}

	gen_wave_data( WdMode );

	if ( WdMode == DEBUGID )
	{
		if ( not debug_right )
			Sds_p->Write_wave_arr( display_data );
	}
	else
	{
		Sds_p->Write_wave_arr( display_data );
	}
}

void Wavedisplay_class::set_Wdnames()
{
	rolename		= roleNames[Wd_ptr.role];
	oscname			= typeNames[Wd_ptr.type];
};
void Wavedisplay_class::set_Wdptr( RoleId_e role )
{
	Wd_ptr			= data_ptr_mtx[role][wd_data.oscId];
	set_Wdnames		();
	if ( Wd_ptr.ptr == nullptr )
	{
		Comment( WARN, "Cannot set Wavedisplay nullptr for [" , rolename, oscname , "]");
		return;
	}
	wd_ptr 			= Wd_ptr.ptr;
	wd_frames		= *Wd_ptr.frames;
}
void Wavedisplay_class::Set_WdData	()
{
	wd_data			= sds->WD_state;
	set_Wdptr		( wd_data.roleId );
	wd_data.frames	= wd_frames;
	sds->WD_state	= wd_data;
	set_Wdmode		();

	Comment( DEBUG, "Wavedisplay ptr set to: ", rolename, oscname, wd_frames );
}

// pos is frames 	-> unit = 1
// pos is records 	-> unit = min_frames
void Wavedisplay_class::Set_wdcursor( buffer_t pos, buffer_t unit )
{	// convert scanner position or record position into wavedisplay position
	if( WdMode == FFTID ) return;// no time coordinate for fftmode

	int	max 	= wd_frames / unit;
	set_Wdcursor( wavedisplay_len * pos / max );
}
void Wavedisplay_class::set_Wdcursor( uint16_t cursor )
{
	switch ( WdMode )
	{
		case DEBUGID 	: { cursor = wavedisplay_len / 2;  break; }
		case FULLID		: { break; }
		case CURSORID	: {	break; }
		default 		: { cursor = 0; break; }
	} // switch  WdMode
	wd_data.cursor.cur		= cursor;
	sds->WD_state.cursor.cur= cursor;
}

void Wavedisplay_class::set_Wdmode()
{
	if ( wd_data.fftmode )
		WdMode	= FFTID;
	else
		WdMode 	= wd_data.wd_mode;

	debug_right	= false;
}

void Wavedisplay_class::Add_role_ptr( 	RoleId_e wd_role,
										Data_t* ptr,
										buffer_t* wd_frames )
{
	// special case for StA role and audioserver. They do not have osc's
	for( OSCID_e osctype : oscIds )
	{
		Add_data_ptr( osctype, wd_role, ptr, wd_frames );
	}
}
void Wavedisplay_class::Add_data_ptr( 	OSCID_e		wd_type,
										RoleId_e 	wd_role,
										Data_t* 	ptr,
										buffer_t* 	frames )
{
	Comment( INFO, 	"adding wave display: ",
					(int)wd_role,
					(int)wd_type,
					(int)*frames );
	if ( ptr == nullptr )
	{
		Exception("Undefined Wavedisplay" );
	}
	set<int> osctype_set = range_set(0, 2);
	if ( not osctype_set.contains( wd_type ) )
	{
		Exception( "wd_type out of bounds" );
	}

	wd_ptr_t	wd									= { .role=wd_role, .type=wd_type,
														.ptr=ptr, .frames=frames };
				data_ptr_mtx[ wd_role ][ wd_type ]	= wd;
}

void Wavedisplay_class::TestWd()
{
	TEST_START( className );

	sds->WD_state.fftmode = true;
	sds->WD_state.oscId = OSCID;
	Set_WdData();
	Assert_lt( (buffer_t) 0, sds->WD_state.frames );
	Assert_equal( WdMode, FFTID );
	WriteData();

	sds->WD_state.wd_mode = wavedisplay_struct::ADSRID;
	Set_WdData();
	Assert_equal( WdMode, ADSRID );

	TEST_END( className );
}
