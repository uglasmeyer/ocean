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
	this->className = Logfacility_class::module;
	debug_switch = false;
	this->Sds_p = sds;
}



// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len
wd_arr_t Wavedisplay_class::gen_cxwave_data( )
{
	auto gen_debug = [ this ]( param_t param )
	{
		if ( debug_switch ) // n = param.len ... 2*param.len - right half
		{
			debug_switch = false;
			for ( buffer_t n = 0; n < param.len; n++ )
			{
				Data_t value = rint(data_ptr[n]);
				display_buffer[ n + param.len  ] = value;
			}
		}
		else // n=0 ... param.len - left half
		{
			debug_switch = true;
			buffer_t frames = Sds_p->addr->audioframes;
			for ( buffer_t n = frames - param.len; n < frames; n++ )
			{
				Data_t value = rint(data_ptr[n]);
				display_buffer[ n + param.len - frames ] = value;
			}
		}
	};

	auto gen_full = [ this ]( param_t param )
	{
		buffer_t audioframes= Sds_p->addr->audioframes;
		int ratio			= max_frames / audioframes ;
		uint idx			=  param.len / ratio * index_counter ;
		for ( buffer_t n = 0; n < audioframes; n = n + param.step )
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

	auto gen_flow = [ this ]( param_t param )
	{
		int idx 	= 0;
		int step	= param.step;
		for ( buffer_t n = offs; n < param.len*step + offs; n = n + step )
		{
			Data_t value = rint( data_ptr[n] );
			display_buffer[ idx ] = value;
			idx++;
		}
	};

	if ( data_ptr == nullptr )
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
			gen_full( param );
			break;
		}
	case FLOWID :
		{
			param = param_flow;
			gen_flow( param );
			break;
		}
	case DEBUGID :
		{
			param = param_split;
			gen_debug( param );
			break;
		}
	default :
		{
			Comment(ERROR,"No valid wave display type provided to gen_cxwave_data");
			return display_buffer;
		}
	} // switch type

	frame_counter++;
	offs = offs + param.drift * frame_counter;
	if ( offs >= param.max_offs )
	{
		frame_counter 	= 0;
		offs			= 0;
	}
	return display_buffer;
}

void Wavedisplay_class::Write_wavedata()
{
	wd_arr_t display_data = gen_cxwave_data(  );
	if (( not debug_switch ) and ( Type == DEBUGID ))
		Sds_p->Write_arr( display_data );
	if ( not ( Type == DEBUGID ))
		Sds_p->Write_arr( display_data );
}

void Wavedisplay_class::SetDataPtr		( const char& osctype, const char& wd_role  )
{

	data_ptr = data_ptr_arr[wd_role][osctype];
	if ( data_ptr == nullptr )
		Comment( WARN, "Cannot set Wavedisplay ptr to null [" + to_string(wd_role) + "]" +
														  "[" + to_string(osctype) + "]");
	else
		Comment( INFO, "wave display selected: "+
						osc_struct().roles[ wd_role ] + " " +
						osc_struct().types[ osctype ] );
}

void Wavedisplay_class::Set_type		( const char& wd_type )
{
	Type 			= wd_type;
	debug_switch	= false;
}

void Wavedisplay_class::Add_role_ptr	( const char& wd_role, Data_t* ptr )
{
	for( char osctype : { osc_struct::VCOID, osc_struct::FMOID, osc_struct::OSCID } )
	{
		Add_data_ptr( osctype, wd_role, ptr );
	}
}
void Wavedisplay_class::Add_data_ptr	( const char& osctype, const char& wd_role, Data_t* ptr )
{
	set<int> osctype_set = range_set(0, 2);
	if ( ptr == nullptr )
	{
		Exception("Undefined Wavedisplay with index " + to_string( wdId) );
	}
	if ( not osctype_set.contains( osctype ) )
		Exception( "osctype out of bounds" );
	data_ptr_arr[ wd_role ][osctype ] = ptr;

	Comment( INFO, "adding wave display: " + to_string( wd_role ) 	+ " " +
					osc_struct().roles[wd_role]						+  " - " +
					osc_struct().types[osctype] );

}


