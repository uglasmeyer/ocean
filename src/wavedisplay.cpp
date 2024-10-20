	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
 */

#include "Wavedisplay.h"



Wavedisplay_class::Wavedisplay_class(  )
: Logfacility_class("Wavedisplay")
{
//	this->wd_arr = wd_arr;
	split_switch = false;
	ptr_index = 0;
}

void Wavedisplay_class::Clear_data()
{
	for( buffer_t n = 0; n < wavedisplay_len; n++ )
	{
		display_buffer[n] = 0.0;
	}
}

// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len
wd_arr_t Wavedisplay_class::Gen_cxwave_data( )
{
	if ( data_ptr_arr[ ptr_index ] == NULL )
	{
		Comment(ERROR, "wave display got nullptr at index " + to_string(ptr_index)) ;
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
				Data_t value = rint(data_ptr_arr[ ptr_index ][n]);
				display_buffer[ n + param.len  ] = value;
			}
		}
		else // n=0 .. param.len - left half
		{
			split_switch = true;
			for ( buffer_t n = max_frames - param.len; n < max_frames; n++ )
			{
				Data_t value = rint(data_ptr_arr[ ptr_index ][n]);
				display_buffer[ n + param.len - max_frames ] = value;
			}

		}

	}
	else // full, flow
	{
		int idx = 0;
		for ( buffer_t n = offs; n < param.len*param.step + offs; n=n+param.step )
		{
			Data_t value = rint( data_ptr_arr[ ptr_index ][n]);
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

void Wavedisplay_class::Set_data_ptr( size_t select )
{
	if ( select < 0)
		select = 0;
	if ( select > wavedisplay_str_arr.size() - 1)
		select = wavedisplay_str_arr.size() - 1;
	ptr_index = select;
	Comment( DEBUG, "wave display selected: " + wavedisplay_str_arr[ ptr_index ] + " " + to_string(ptr_index));
}

void Wavedisplay_class::Set_type( int type )
{
	Type = type;
	split_switch	= false;
}

void Wavedisplay_class::Update( int select , Data_t* ptr )
{
	data_ptr_arr[ select ] = ptr;
}

void Wavedisplay_class::Add_data_ptr( const string& name, Data_t* ptr )
{
	auto get_index = [ this ]( string name )
	{
		int idx = 0;
		for( string v : wavedisplay_str_arr )
			{
				if ( cmpstr(v, name ) ) return idx;
				idx++;
			}
		return -1;
	};
	if ( ptr == NULL )
	{
		Exception("Undefined Wavedisplay with index " + to_string( ptr_index) );
	}
	int idx = get_index( name );
	if (  idx < 0 )
	{
		Exception("Wavedisplay name " + name + " is unknown" );
	}

	data_ptr_arr.at(idx) = ptr ;

	Comment( INFO, "adding wave display: " + to_string( idx ) + " - " + name );

	ptr_index = idx;
}


