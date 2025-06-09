	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
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
//	wd_frames 		= check_range( 	frames_range,
//									*data_ptr_mtx[wd_status.roleId][wd_status.oscId].frames,
//									"gen_cxwave_data " + to_string( wd_status.roleId ));
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
				display_buffer[ n + param.len  ] = value;   // right half
			}
		}
		else // n=0 ... param.len - right data range -> front buffer range
		{	// fill this area first
			debug_right = not debug_right;
			for ( buffer_t n = wd_frames - param.len; n < wd_frames; n++ )
			{
				Data_t value = data_ptr[n];
				display_buffer[ n + param.len - wd_frames ] = value;
			}
		}
	};

	auto gen_full = [ this ]( param_t param )
	{

		uint 		idx			= 0;
		uint		step		= wd_frames / param.len;
		for ( buffer_t n = 0; n < wd_frames; n = n + step )
		{
			if ( idx < param.len )
			{
				Data_t value = rint( data_ptr[n] );
				display_buffer[ idx ] = value;
			}
			else
				break;
			idx++;
		}
		index_counter++;
		if ( idx >= param.len )
		{
			index_counter = 0;
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
		display_buffer = fft( cdv, false );
	};

	auto gen_flow = [ this ]( param_t param )
	{
		uint idx 	= 0;
		uint step	= param.step;
		for ( buffer_t n = offs; n < param.len*step + offs; n = n + step )
		{
			Data_t value = rint( data_ptr[n] );
			display_buffer[ idx ] = value;
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
	gen_cxwave_data(  );
	if ( WdMode == DEBUGID )
	{
		if ( not debug_right )
			Sds_p->Write_arr( display_buffer );
	}
	else
	{
		Sds_p->Write_arr( display_buffer );
	}
}

void Wavedisplay_class::SetDataPtr	( const wd_status_t& status  )
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
						OscRole.roles[ status.roleId ] + " " +
						OscRole.types[ status.oscId ] );
	set_wdmode( status.wd_mode );
	setFFTmode( status.fftmode );
}

void Wavedisplay_class::set_wdmode		( const char& _mode )
{
	WdMode 			= _mode;
	debug_right	= false;
}

void Wavedisplay_class::setFFTmode( const bool& mode )
{
	if ( mode )
		WdMode		= FFTID;
//	fft_mode = mode;
}
void Wavedisplay_class::Add_role_ptr( 	const char& wd_role,
										Data_t* ptr,
										buffer_t* wd_frames )
{
	// special case for role external and audioserver. They do not have osc's
	for( char osctype : osctypeIds )
	{
		Add_data_ptr( osctype, wd_role, ptr, wd_frames );
	}
}
void Wavedisplay_class::Add_data_ptr( 	const char& wd_type,
										const char& wd_role,
										Data_t* 	ptr,
										buffer_t* 	frames )
{
	Comment( INFO, "adding wave display: " +
					to_string( wd_role ) 	+ " " +
					OscRole.roles[wd_role]	+  " - " +
					OscRole.types[wd_type] + " " +
					to_string( *frames));
	if ( ptr == nullptr )
	{

		EXCEPTION("Undefined Wavedisplay" );
	}
	set<int> osctype_set = range_set(0, 2);
	if ( not osctype_set.contains( wd_type ) )
	{
		EXCEPTION( "wd_type out of bounds" );
	}
	data_ptr_mtx[ wd_role ][wd_type ] = { ptr, frames };;



}


