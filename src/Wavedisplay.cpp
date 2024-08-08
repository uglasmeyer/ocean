	/*^
 * Wavedisplayclass.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: sirius
 */

#include "Wavedisplay.h"


Wavedisplay_class::Wavedisplay_class( ifd_t* ifd )
: Logfacility_class("Wavedisplay")
{
	this->ifd = ifd;
	split_switch = false;
}

void Wavedisplay_class::Clear_data()
{
	for( buffer_t n = 0; n < wavedisplay_len; n++ )
	{
		ifd->wavedata[n] = 0;
	}
}

// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len
void Wavedisplay_class::Gen_cxwave_data( )
{

	if ( data_ptr_array[ ptr_index ] == NULL )
	{
		Comment(ERROR, "wave display got nullptr at index " + to_string(ptr_index)) ;
		return;
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
			return;
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
				Data_t value = rint(data_ptr_array[ ptr_index ][n]);
				display_buffer[ n + param.len  ] = value;
			}
			for( buffer_t n = 0 ; n < wavedisplay_len; n++ )
				ifd->wavedata[n] = display_buffer[n] ;
		}
		else // n=0 .. param.len - left half
		{
			split_switch = true;
			for ( buffer_t n = max_frames - param.len; n < max_frames; n++ )
			{
				Data_t value = rint(data_ptr_array[ ptr_index ][n]);
				display_buffer[ n + param.len - max_frames ] = value;
			}

		}

	}
	else // full, flow
	{
		int idx = 0;
		for ( buffer_t n = offs; n < param.len*param.step + offs; n=n+param.step )
		{
			Data_t value = rint( data_ptr_array[ ptr_index ][n]);
			ifd->wavedata[ idx ] = value;
			idx++;
		}
	}

	frame_counter++;
	offs = offs + param.drift * frame_counter;
	if ( offs >= param.max_offs )
		frame_counter = 0;

}

void Wavedisplay_class::Set_data_ptr( size_t select )
{
	if ( select < 0)
		select = 0;
	if ( select > wavedisplay_str_vec.size() - 1)
		select = wavedisplay_str_vec.size() - 1;
	ptr_index = select;
	Comment( DEBUG, "wave display selected: " + wavedisplay_str_vec[ ptr_index ] + " " + to_string(ptr_index));
}

void Wavedisplay_class::Set_type( int type )
{
	Type = type;
	split_switch	= false;
}

void Wavedisplay_class::Update( int select , Data_t* ptr )
{
	data_ptr_array[ select ] = ptr;
}

void Wavedisplay_class::Add_data_ptr( Data_t* ptr )
{
	if ( ptr == NULL ) return;
	data_ptr_array[ ptr_index ] = ptr;
	Comment( DEBUG, "adding wave display: " + to_string(ptr_index) + " - " + wavedisplay_str_vec[ ptr_index ] );

	ptr_index = ( ptr_index + 1) % data_ptr_array.size();
}


