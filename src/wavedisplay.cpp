	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
 */

#include "Wavedisplay.h"

Wavedisplay_class::Wavedisplay_class( Interface_class* sds )
: Logfacility_class("Wavedisplay")
{
	this->className = Logfacility_class::module;
	split_switch = false;
	wdId = 0;
	this->Sds_p = sds;
}

void Wavedisplay_class::Clear_data()
{
	for( buffer_t n = 0; n < wavedisplay_len; n++ )
	{
		display_buffer[n] = 0.0;
	}
}

// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len
wd_arr_t Wavedisplay_class::gen_cxwave_data( )
{
	if ( data_ptr_arr[ wdId ] == NULL )
	{
		Comment(ERROR, "wave display got nullptr at index " + to_string(wdId)) ;
		return display_buffer;
	}
	param_t param;
	switch ( Type )
	{
	case FULLID :
		{
			param = param_full;
			break;
		}
	case FLOWID :
		{
			param = param_flow;
			break;
		}
	case DEBUGID :
		{
			param = param_split;
			break;
		}
	default :
		{
			Comment(ERROR,"No valid wave display type provided to gen_cxwave_data");
			return display_buffer;
		}
	} // switch type

	offs 		= param.drift*frame_counter;

	if ( Type == DEBUGID ) // split
	{
		if ( split_switch ) // n = param.len . 2*param.len - right half
		{
			split_switch = false;
			for ( buffer_t n = 0; n < param.len; n++ )
			{
				Data_t value = rint(data_ptr_arr[ wdId ][n]);
				display_buffer[ n + param.len  ] = value;
			}
		}
		else // n=0 .. param.len - left half
		{
			split_switch = true;
			buffer_t frames = Sds_p->addr->audioframes;
			for ( buffer_t n = frames - param.len; n < frames; n++ )
			{
				Data_t value = rint(data_ptr_arr[ wdId ][n]);
				display_buffer[ n + param.len - frames ] = value;
			}

		}

	}
	else // full, flow
	{
		int idx 	= 0;
		int step	= param.step*Sds_p->addr->audioframes / max_frames;
		for ( buffer_t n = offs; n < param.len*step + offs; n=n+step )
		{
			Data_t value = rint( data_ptr_arr[ wdId ][n]);
			display_buffer[ idx ] = value;
			idx++;

		}
	}

	frame_counter++;
	offs = offs + param.drift * frame_counter;
	if ( offs >= param.max_offs )
		frame_counter = 0;
	return display_buffer;
}

void Wavedisplay_class::SetId( size_t wd_id )
{
	if ( wd_id < 0)
		wd_id = 0;
	if ( wd_id > WD_SIZE - 1)
		wd_id = WD_SIZE - 1;
	wdId = wd_id;
	Comment( DEBUG, "wave display selected: " + wavedisplay_str_arr[ wdId ] + " " + to_string(wdId));
}

void Wavedisplay_class::Set_type( int type )
{
	Type = type;
	split_switch	= false;
}

void Wavedisplay_class::Add_data_ptr( const string& name, Data_t* ptr )
{
	auto get_index = [ this ]( string name )
	{
		int idx = 0;
		for( string v : wavedisplay_str_arr )
			{
				if ( strEqual(v, name ) ) return idx;
				idx++;
			}
		return -1;
	};
	if ( ptr == nullptr )
	{
		Exception("Undefined Wavedisplay with index " + to_string( wdId) );
	}
	int idx = get_index( name );
	if (  idx < 0 )
	{
		Exception("Wavedisplay name " + name + " is unknown" );
	}

	data_ptr_arr.at(idx) = ptr ;

	Comment( INFO, "adding wave display: " + to_string( idx ) + " - " + name );

	wdId = idx;
}

void Wavedisplay_class::Write_wavedata()
{
	wd_arr_t display_data = gen_cxwave_data(  );
	Sds_p->Write_arr( display_data );
}


