	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <Wavedisplay_base.h>
#include <Wavedisplay.h>


Wavedisplay_class::Wavedisplay_class( Interface_class* sds )
: Logfacility_class("Wavedisplay")
{
	this->className = Logfacility_class::className;
	this->Sds_p = sds;
}

// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len
void Wavedisplay_class::gen_cxwave_data( )
{
	wd_frames		= *data_ptr_mtx[wd_status.roleId][wd_status.oscId].frames;
	if ( wd_frames == 0 )
	{
		Comment( WARN, "Zero wavedisplay frames", (int)wd_status.roleId, ",", (int)wd_status.oscId );
		return;
	}

	auto gen_debug = [ this ]( param_t param )
	{
		if ( debug_right ) // n = param.len ... 2*param.len - left data range -> back buffer range
		{	// fill this area thereafter
			debug_right = not debug_right;
			for ( buffer_t n = 0; n < param.len; n++ )
			{
				Data_t value = data_ptr[n];
				display_data[ n + param.len  ] = value;   // right half
			}
		}
		else // n=0 ... param.len - right data range -> front buffer range
		{	// fill this area first
			debug_right = not debug_right;
			for ( buffer_t n = wd_frames - param.len; n < wd_frames; n++ )
			{
				Data_t value = data_ptr[n];
				display_data[ n + param.len - wd_frames ] = value;
			}
		}
	};

	auto gen_full = [ this ]( param_t param )
	{

		uint 		idx				= 0;
		uint		step			= wd_frames / wavedisplay_len ;
		Assert_lt( (step-1) * wavedisplay_len, wd_frames );

		for ( buffer_t n = 0; n < wavedisplay_len; n++ )
		{
			display_data[ n ] 	= data_ptr[idx];
			idx += step;
		}
	};

	auto gen_fft = [ this ]( param_t param )
	{
		cd_vec_t cdv {};
		param.step	= rint( wd_frames / wavedisplay_len );
		for ( buffer_t n = 0; n < wd_frames; n = n + param.step )
		{
			Data_t value = data_ptr[n];
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
			Data_t value = data_ptr[n] ;
			display_data[ idx ] = value;
			idx++;
		}
	};

	if ( data_ptr == nullptr )
	{
		Comment(DEBUG, "wave display got nullptr at index " +
						to_string(wd_status.roleId) + "," +
						to_string(wd_status.oscId)) ;
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
		case DEBUGID :
		{
			param = param_split;
			gen_debug( param );
			break;
		}
		case FFTID :
		{
			gen_fft( param_full );
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

void Wavedisplay_class::Write_wavedata()
{
	auto _scale = [ this ](  )
	{
		if ( wd_status.roleId != ADSRROLE )
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

void Wavedisplay_class::SetDataPtr	( const WD_data_t& status  )
{
	wd_status 		= status;
	Data_t* ptr 	= data_ptr_mtx[status.roleId][status.oscId].ptr;

	if ( ptr == nullptr )
	{
		Comment( WARN, "Cannot set Wavedisplay ptr to null [" + to_string(status.roleId) + "]" +
														  "[" + to_string(status.oscId) + "]");
		return;
	}
	data_ptr 		= ptr;
	Comment( DEBUG, "wave display selected: "+
						roleNames[ status.roleId ] + " " +
						typeNames[ status.oscId ] );
	set_wdmode( status.wd_mode );
	setFFTmode( status.fftmode );
}

void Wavedisplay_class::Set_wdcursor(int pos, int max )
{
	set_wdcursor( wavedisplay_len * pos / max );
}
void Wavedisplay_class::Set_wdcursor(int pos )
{
	int max = *data_ptr_mtx[wd_status.roleId][wd_status.oscId].frames;
	set_wdcursor( wavedisplay_len * pos / max );
}

void Wavedisplay_class::set_wdcursor( uint16_t cursor )
{
	switch ( WdMode )
	{
		case DEBUGID 	: { cursor = wavedisplay_len / 2;  break; }
		case FULLID		: { break; }
		default 		: { cursor = 0; break; }
	} // switch  WdMode
	Sds_p->addr->WD_status.cursor = cursor;
}

void Wavedisplay_class::set_wdmode( const WdModeID_t& _mode )
{
	WdMode 		= _mode;
	debug_right	= false;
	set_wdcursor( 0 );
}

void Wavedisplay_class::setFFTmode( const bool& mode )
{
	if ( mode )
		WdMode		= FFTID;
}
void Wavedisplay_class::Add_role_ptr( 	OscroleId_t wd_role,
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
										OscroleId_t wd_role,
										Data_t* 	ptr,
										buffer_t* 	frames )
{
	Comment( INFO, "adding wave display: "	+
					to_string( wd_role ) 	+ " " +
					roleNames[ wd_role ]	+ " - " +
					typeNames[ wd_type ] 	+ " " +
					to_string( *frames));
	if ( ptr == nullptr )
	{
		Exception("Undefined Wavedisplay" );
	}
	set<int> osctype_set = range_set(0, 2);
	if ( not osctype_set.contains( wd_type ) )
	{
		Exception( "wd_type out of bounds" );
	}
	data_ptr_mtx[ wd_role ][wd_type ] = { ptr, frames };;



}


