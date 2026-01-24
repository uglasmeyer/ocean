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

	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <Wavedisplay_base.h>
#include <Wavedisplay.h>


Wavedisplay_class::Wavedisplay_class( Interface_class* _sds )
: Logfacility_class("Wavedisplay")
{
	this->Sds_p 		= _sds;
	this->sds			= _sds->addr;
	this->param_flow	= param_struct();
	this->param_full 	= param_struct();
	this->param_split	= { 0, wavedisplay_len / 2	, 1 , max_frames };
	this->param_cursor	= { 0, wavedisplay_len, 0, 0 };
	this->frame_counter	= 0;
	this->offs 			= 0;
	this->WdMode		= sds->WD_state.wd_mode;
	this->wd_ptr 		= nullptr;
	this->Wd_ptr		= wd_ptr_struct();
	this->wd_data		= WD_data_struct();
	this->debug_right	= true;
	Assert_equal( roleNames.size(), (size_t) ROLE_SIZE );

}

// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len
void Wavedisplay_class::gen_cxwave_data( )
{
	if ( wd_frames == 0 )
	{
		Comment( WARN, "Zero wavedisplay frames", (int)wd_data.roleId, ",", (int)wd_data.oscId );
		return;
	}

	auto gen_debug = [ this ]( param_t param )
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

	auto gen_full = [ this ]( param_t param )
	{

		uint 		idx				= 0;
		uint		step			= rint( wd_frames / wavedisplay_len );
		assert( step>0 );
		Assert_lt( int((step-1) * wavedisplay_len), int(wd_frames) );

		for ( buffer_t n = 0; n < wavedisplay_len; n++ )
		{
			display_data[ n ] 	= wd_ptr[idx];
			idx += step;
		}
	};

	auto gen_fft = [ this ]( param_t param )
	{
		cd_vec_t cdv {};
		param.step	= rint( wd_frames / wavedisplay_len );
		if ( param.step == 0 )
			return;
		for ( buffer_t n = 0; n < wd_frames; n = n + param.step )
		{
			Data_t value = wd_ptr[n];
			cdv.push_back( cd_t( (double)value, 0.0 ));
		}
		display_data = fft( cdv, false );
	};

	auto gen_flow = [ this ]( param_t param )
	{
		uint idx 	= 0;
		uint step	= param.step;
		for ( buffer_t n = offs; n < param.len*step + offs; n = n + step )
		{
			Data_t value = wd_ptr[n] ;
			display_data[ idx ] = value;
			idx++;
		}
	};
	if ( wd_ptr == nullptr )
	{
		Comment(ERROR, "wave display got nullptr at index " +
						to_string(wd_data.roleId) + "," +
						to_string(wd_data.oscId)) ;
		return;
	}

	param_t param;
	switch ( WdMode )
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
			param = param_full;
			gen_fft( param );
			break;
		}
		default :
		{
			Comment(ERROR,"No valid wave display type provided to gen_cxwave_data");
		}
	} // switch type

	frame_counter++;
	offs = offs + param.drift * frame_counter;
	if ( offs >= param.max_offs )
	{
		frame_counter 	= 0;
		offs			= 0;
	}
}

void Wavedisplay_class::WriteData()
{
	auto _scale = [ this ](  )
	{
		if ( wd_data.roleId != ADSRROLE )
			return;
		for( uint n = 0; n < wavedisplay_len; n++ )
		{
			display_data[n] = display_data[n] * 1024 * 8;
		}
	};
	gen_cxwave_data(  );
	_scale();
	if ( WdMode == DEBUGID )
	{
		if ( not debug_right )
			Sds_p->Write_arr( display_data );
	}
	else
	{
		Sds_p->Write_arr( display_data );
	}
}

void Wavedisplay_class::Set_WdData	()
{
	wd_data					= sds->WD_state;
	Wd_ptr					= data_ptr_mtx[wd_data.roleId][wd_data.oscId];

	if ( Wd_ptr.ptr == nullptr )
	{
		Comment( WARN, "Cannot set Wavedisplay ptr to null [" , (int)wd_data.roleId, "] ["
														  	  , (int)wd_data.oscId , "]");
		return;
	}
	wd_ptr 					= Wd_ptr.ptr;
	wd_frames				= *Wd_ptr.frames;
	wd_data.frames			= wd_frames;
	set_Wdmode				( wd_data.wd_mode, wd_data.fftmode );
	sds->WD_state			= wd_data;
	Comment( WARN, "Wavedisplay ptr set to [" 	, (int)wd_data.roleId, "] ["
												, (int)wd_data.oscId , "]");
}

// pos is frames 	-> unit = 1
// pos is records 	-> unit = min_frames
void Wavedisplay_class::Set_wdcursor( buffer_t pos, buffer_t unit )
{	// convert scanner position or record position into wavedisplay position
	if( WdMode == FFTID ) return;// no time coordinate for fftmode

	int max = wd_frames / unit;
	set_wdcursor( wavedisplay_len * pos / max );
}
void Wavedisplay_class::set_wdcursor( uint16_t cursor )
{
	switch ( WdMode )
	{
		case DEBUGID 	: { cursor = wavedisplay_len / 2;  break; }
		case FULLID		: { break; }
		case CURSORID	: {	break; }
		default 		: { cursor = 0; break; }
	} // switch  WdMode
	wd_data.cursor.cur			= cursor;
	sds->WD_state.cursor.cur 	= cursor;
}

void Wavedisplay_class::set_Wdmode( const WdModeID_t& _mode, bool fftmode )
{
	if ( fftmode )
		WdMode		= FFTID;
	else
		WdMode 		= _mode;
	debug_right	= false;
}

void Wavedisplay_class::Add_role_ptr( 	RoleId_e wd_role,
										Data_t* ptr,
										buffer_t* wd_frames )
{
	// special case for role external and audioserver. They do not have osc's
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

//	cout << "wd_role: " << (int) wd_role << endl;
	Comment( INFO, "adding wave display: ",
					(int)wd_role ,
					roleNames[ wd_role ],
					typeNames[ wd_type ],
					(int) *frames );
	if ( ptr == nullptr )
	{
		Exception("Undefined Wavedisplay" );
	}
	set<int> osctype_set = range_set(0, 2);
	if ( not osctype_set.contains( wd_type ) )
	{
		Exception( "wd_type out of bounds" );
	}
	data_ptr_mtx[ wd_role ][wd_type ] = wd_ptr_struct{ ptr, frames };

}


