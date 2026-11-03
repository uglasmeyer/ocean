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
 * wavedisplay.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <Wavedisplay.h>
#include <Frequency.h>
#include <Fourier.h>
extern Frequency_class*	frequency;
extern Spectrum_class*	oscspectrum;

auto is_zeroPoint = []( Data_t* data, buffer_t n )
{
	return ( ( data[n-1] <= 0 ) and ( 0 <= data[n+1] ) );
};

float waveLength( Data_t* data, size_t size )
{

	buffer_t n = 1;
	do
	{
		n++;
	} while( is_zeroPoint( data, n ) );
	buffer_t len = size - n;
	int counter = 0;

	for( ;n < size-1; n++ )
	{
		if( is_zeroPoint( data, n ) )
		{
			counter++;
		}
	}

	cout << float(counter)/float(len)*24000 << ":" << len << "	" << data[0] << endl;
	return len/counter;
}

/**************************************************
 * Wavedisplay_class
 *************************************************/
Wavedisplay_class::Wavedisplay_class( SharedData_class* _sds )
	: Logfacility_class("Wavedisplay")
	, WD( _sds->addr )
{
	this->Sds_p 		= _sds;
	this->sds			= _sds->addr;
	this->param_flow	= param_struct();
	this->param_full 	= { 0, wavedisplay_len	, 0, 0 };
	this->param_cursor	= { 0, wavedisplay_len	, 0, 0 };
	this->param_split	= { 0, wavedisplay_len/2, 1, max_frames };
	this->param_adsr	= { 0, adsrdisplay_len	, 0, 0 };
	this->param_spec	= { 0, fft_display_len	, 0, 0, 1.0 };
	this->param_nil		= { 0, 0, 0, 0 };
	this->offs 			= 0;

	this->Wd_ptr		= wd_ptr_struct();
	this->Data_p		= nullptr;
	this->wd_frames		= 0;
	this->debug_right	= true;

	Assert_equal( roleNames.size()-1, (size_t) ROLE_SIZE );
}

Wavedisplay_class::~Wavedisplay_class()
{
}

void Wavedisplay_class::clear_display( size_t len )
{
	if ( wd_frames == 0 )
	{
		for( buffer_t n = 0; n < len; n++ )
			display_data[n] = 0;
	}
}


void Wavedisplay_class::generate_wavedata( WdModeID_t mode )
{// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len


	auto gen_debug	= [ this ]( param_t param )
	{
		if ( debug_right ) // n = param.len ... 2*param.len - left data range -> back buffer range
		{	// fill display data thereafter with front chunk of frames
			for ( buffer_t n = 0; n < param.len; n++ )
			{
				Data_t value = Data_p[n];
				display_data[ n + param.len  ] = value;   // right half
			}
		}
		else // n=0 ... param.len - right data range -> front buffer range
		{	// fill display data first with back chunk of frames
			for ( buffer_t n = wd_frames - param.len; n < wd_frames; n++ )
			{
				Data_t value = Data_p[n];
				display_data[ n + param.len - wd_frames ] = value;
			}
		}
		debug_right = not debug_right;

	};

	auto shift_data = [ this ]( Data_t* data, buffer_t size )
	{
		if( not WD.data.spectrum_view )
			return buffer_t( 0 );
		if( size == adsrdisplay_len )
			return buffer_t( 0 );
		buffer_t n = 1;
		do
		{
			if ( is_zeroPoint( data, n) )
				return n;
			n++;
		} while ( n < size );
		return buffer_t( 0 );
	};

	auto adjust_df = [ this ]( buffer_t len, buffer_t first, float df )
	{
		// last = first + ( last - first ) * df
		if( not WD.data.spectrum_view )
			return df;
		if( len == adsrdisplay_len )
			return df;
		buffer_t	last= len * df + first;
		if( last +1 > WD.data.frames )
			return df;
		if( fcomp( Data_p[last], 0.0f ) )
			return df;
		buffer_t	n	= last;
		do
		{
			n--;
			if (  ( Data_p[n-1] < 0 ) and ( Data_p[n+1] > 0  ) )
			{
				return float( n-1 - ( first )) / float( len );
			}
		} while ( n > first );
		return df;
	};

	auto gen_full	= [ this, shift_data, adjust_df ]( param_t param )
	{
		float 	df	= ( param.zoom * wd_frames ) / float( param.len ) ;
		float 	f	= shift_data( Data_p, wd_frames );
		df 			= adjust_df( param.len, buffer_t( f ), df );

		for_each( display_data, [ this, &f, df ]( Data_t& Data )
		{
			buffer_t 	m 	= f;
						Data= Data_p[m % wd_frames];
						f 	= f + df;
		} );
	};

	auto gen_symm	= [ this, shift_data, adjust_df ]( param_t param, int8_t pm )
	{
		buffer_t	len	= param.len;
		if( pm == 0 )	// do not symmetrys
			pm = 1;
		else			// symmetrys
			len			= param.len/2;

		if( pm > 0 )
		{
			float 		df		= ( param.zoom * wd_frames ) / float( len ) ;
			float 	f			= shift_data( Data_p, wd_frames );
			df 					= adjust_df( len, buffer_t( f ), df );
			if ( df * param.len + 1 > wd_frames )
				return;
			for( buffer_t n = 0; n < len; n++ )
			{
				buffer_t 	m 	= f;
				display_data[ n ] = Data_p[m];
							f 	= f + df;
			};
		}
		if( pm < 0 )	// fill reverse
		{
			for( buffer_t n = len; n < param.len; n++ )
			{
				display_data[ n ] = -display_data[param.len-1 - n];
			}
		}
	};

	auto gen_fftd = [ this, shift_data, adjust_df, gen_symm, gen_full ]( param_t param )
	{
		param.zoom	= sds->WD_state.zoom;
		gen_symm	( param, 0 );
//		gen_symm	( param,  1 );
//		gen_symm	( param, -1 ) ;
	};

	auto gen_flow 		= [ this ]( param_t& param )
	{
		uint	idx 	= 0;
		param.max_offs	= wd_frames - param.len*param.step;
		param.zoom 		= sds->WD_state.zoom;

		uint step		= param.zoom * param.step;
		for ( buffer_t n = offs; n < param.len*step + offs; n = n + step )
		{
			Data_t	value 				= Data_p[ n ] ;
					display_data[ idx ] = value;
					idx++;
		}
		offs = offs + param.drift;
		if ( offs > param.max_offs )
		{
			offs			= 0;
		}

	};

	auto gen_adsr = [ this ]( param_t param )	// adsr_mode is true
	{
					set_Wdptr	( ADSRROLE );	// define data ptr
		uint 		idx			= 0;
		uint		step		= rint( wd_frames / param.len );
		assert( step>0 );
		Assert_lt( int((step-1) * param.len), int(wd_frames) );

		for ( buffer_t n = 0; n < param.len; n++ )
		{
			display_data[ n ] 	= Data_p[idx];
			idx += step;
		}
		set_Wdptr( WD.data.roleId );
	};

	auto gen_spec = [ this ]( param_t param )
	{
		if ( WD.data.adsrmode ) return;
		for( buffer_t n = 0; n < param.len; n++ )
			display_data[n] = 0;
		spectrum_t*	Data 	= &sds->spectrum_arr[WD.data.oscId];
		uint8_t max_idx		= 256 / frequency->Frqadj_max;
		float volfactor		= oscspectrum->VolFactor( Data->volidx );
		for( uint8_t channel : osc_channel_vec )
		{
			buffer_t n = max_idx * frequency->Frqadj( channel, Data->frqidx[channel] );
			if( n < param.len )
				display_data[n] += Data->volidx[channel] * volfactor;
		}
	};

	if ( Data_p == nullptr )
	{
		Comment( ERROR, "wave display got nullptr for: ",
						rolename,
						oscname,
						wd_frames );
		return;
	}


	const vector<param_t> param_vec =
	{
		param_full, param_flow, param_split, param_nil,
		param_full,	param_adsr, param_full, param_adsr,
		param_spec
	};
	param_t	param 	= param_vec[mode];
	param.zoom		= sds->WD_state.zoom;
	clear_display	( param.len );
	switch ( mode )
	{
		case FULL_WD :
		{
			gen_full		( param );
			break;
		}
		case FLOW_WD :
		{
			gen_flow		( param_flow );
			break;
		}
		case DEBUG_WD :
		{
			gen_debug		( param_split );
			break;
		}
		case RANGE_WD :
		{
			gen_full		( param_full );
			break;
		}
		case FFTA_WD :		// show output of the fourier analysis
		{
			gen_full		( param );
			break;
		}
		case FFTD_WD :		// generate fft input data in main display
		{
			gen_fftd		( param_full );
			break;
		}
		case ADSR_WD :
		{
			gen_adsr		( param_adsr );
			break;
		}
		case SPEC_WD :
		{
			gen_spec		( param );
			break;
		}
		default :
		{
			Comment(ERROR,"No valid wave display type provided to gen_cxwave_data");
			assert( false );
		}
	} // switch wd_mode

}

void Wavedisplay_class::WriteData()
{
	auto write_displaydata = [ this ]( Data_t* dst, int len )
	{
		if ( debug_right ) return;

		float norm = 1.0f;
		switch ( WD.data.mode )
		{
			case FFTA_WD : { norm = 1.0f/max_data_amp	; break; }
			default : { break; }
		} // switch  WD.data.mode
		for( int n = 0; n < len; n++ )
		{
			dst[n]	= display_data[n] * norm;
		};
	};

	auto make = [ this, write_displaydata ]( WDMODE_t mode, Data_t* dst, buffer_t size )
	{
		WD.data.mode = mode;
		generate_wavedata( mode );
		write_displaydata( dst, size );
	};

	// MainWindow display related wavedata
	if ( WD.data.mode == DEBUG_WD )
	{
		generate_wavedata( DEBUG_WD );
		write_displaydata( &sds->wavedata[0], wavedisplay_len );
		return;
	}
	debug_right = false;
	if( WD.data.spectrum_view )
	{
		make( FFTD_WD, &sds->wavedata[0], wavedisplay_len ); // generate fft input data

		if( WD.data.adsrmode )
		{
			make( ADSR_WD, &sds->adsrdata[0], adsrdisplay_len );
		}
		else	// spectrum mode
		{
			if( sds->fourier.mode == fourier_ns::MANUAL )
				make( SPEC_WD, &sds->fft_data[0], fft_display_len );

			make( FFTA_WD, &sds->adsrdata[0], adsrdisplay_len );
		}
	}
	else // spectrum dialog is not active
	{
		make( WD.data.mode, &sds->wavedata[0], wavedisplay_len );
	}
}

void Wavedisplay_class::set_Wdnames()
{
	rolename		= roleNames[Wd_ptr.role];
	oscname			= typeNames[Wd_ptr.type];
};


void Wavedisplay_class::set_Wdptr( RoleId_e role )
{
	Wd_ptr			= data_ptr_mtx[role][WD.data.oscId];
	set_Wdnames		();
	if ( Wd_ptr.Data_p == nullptr )
	{
		Comment( WARN, "Cannot set Wavedisplay nullptr for [" , rolename, oscname , "]");
		return;
	}
	Data_p 			= Wd_ptr.Data_p;
	wd_frames		= *Wd_ptr.frames;
}
void Wavedisplay_class::Set_WdData	()
{
	WD.Read				( sds ); // data				= sds->WD_state;
	set_Wdptr			( WD.data.roleId );
	WD.data.frames		= wd_frames;
	WD.Save				( sds );//sds->WD_state.frames= wd_frames;

	Comment( DEBUG, "Wavedisplay ptr set to: ", rolename, oscname, wd_frames );
}

// pos is frames 	-> unit = 1
// pos is records 	-> unit = min_frames
void Wavedisplay_class::Set_wdcursor( buffer_t pos, buffer_t unit )
{	// convert scanner position or record position into wavedisplay position
	if( WD.data.mode == FFTA_WD ) return;// no time coordinate for fftmode

	int	max 	= wd_frames / unit;
	set_Wdcursor( wavedisplay_len * pos / max );
}
void Wavedisplay_class::set_Wdcursor( uint16_t cursor )
{
	switch ( WD.data.mode )
	{
		case DEBUG_WD 	: { cursor = wavedisplay_len / 2;  break; }
		case FULL_WD	: { break; }
		case RANGE_WD	: {	break; }
		default 		: { cursor = 0; break; }
	} // switch  WdMode
	WD.data.cursor.cur		= cursor;
	sds->WD_state.cursor.cur= cursor;
}

void Wavedisplay_class::Add_role_ptr( 	RoleId_e wd_role,
										Data_t* ptr,
										buffer_t* frames_p )
{
	// special case for StA role and audioserver. They do not have osc's
	for( OSCID_e wd_type : oscIds )
	{
		Add_data_ptr( wd_type, wd_role, ptr, frames_p );
	}
}
void Wavedisplay_class::Add_data_ptr( 	OSCID_e		wd_type,
										RoleId_e 	wd_role,
										Data_t* 	ptr,
										buffer_t* 	frames_p )
{
	Comment( INFO, 	"adding wave display: ",
					(int)wd_role,
					(int)wd_type,
					(int)*frames_p );
	if ( ptr == nullptr )
	{
		Exception("Undefined Wavedisplay" );
	}
	if ( not osctype_set.contains( wd_type ) )
	{
		Exception( "wd_type out of bounds" );
	}

	Wd_ptr = { wd_role, wd_type, ptr, frames_p };
				data_ptr_mtx[ wd_role ][ wd_type ]	= Wd_ptr;
}

void Wavedisplay_class::TestWd()
{
	TEST_START( className );

	vector<float> v = { 1,2,3,4,5 };
	normalize_vector( v, 1.0f );
	float s = std::accumulate( v.begin(), v.end(), 0.0f );
	Assert_equal( fcomp(s, 1.0f), true );

	sds->WD_state.oscId = OSCID;
	Set_WdData();
	Assert_lt( (buffer_t) 0, sds->WD_state.frames );
	Assert_equal( WD.data.mode, FULL_WD );
	WriteData();

	Set_WdData();
	Assert_equal( WD.data.mode, FULL_WD );

	TEST_END( className );

}

/**************************************************
 * WfDisplay_que_struct
 *************************************************/
WfDisplay_que_struct::WfDisplay_que_struct( interface_t* _sds, sta_role_map* _map )
{
	this->sds			= _sds;
	this->roleMap		= _map;
	this->debug			= true;
}

void WfDisplay_que_struct::Set_que( interface_t* _sds )
{	// call by select_sds
	this->sds 	= _sds;
	this->Que 	= Que_arr[sds->SDS_Id];
	set_prev	();
}
void WfDisplay_que_struct::Init( interface_t* _sds )
{	// call by mainwindow_init
	this->sds 			= _sds;
	this->Que.clear		();
	for( StAId_e staid : StAIds )
		if( sds->StA_state_arr[ staid ].play )
			Que.push_back( roleMap->GetRoleid( staid ) );
	set_prev			();
	Que_arr[sds->SDS_Id]= Que;
}
void WfDisplay_que_struct::Add( RoleId_e role )
{
	if( role == ROLE_SIZE )
		return;
	Que.push_back	( role );
	set_prev		();
}

void WfDisplay_que_struct::Remove( RoleId_e role )
{
	roleid_vec_t tmp{};
	for( RoleId_e _role : Que )
	{
		if( role != _role )
			tmp.push_back( _role );
	}
	Que = tmp;
	set_prev();
}

void WfDisplay_que_struct::set_prev()
{
	if ( Que.empty() )
		Que.push_back( sds->WD_state.roleId );
	else
		sds->WD_state.roleId = Que.back();

	show( debug );
}

void WfDisplay_que_struct::show( bool debug )
{
	if ( debug )
	{
		coutf << 	"Display_fifo RoleIds: (" <<
					(int)sds->WD_state.roleId << "," <<
					(int)Que.back() << ") " <<
					show_items( Que ) << endl;
		Assert_equal( (int)sds->WD_state.roleId, (int)Que.back() );
	}
}

void WfDisplay_que_struct::Test()
{
/*
 * dir id que active
 * +   0  0   0
 * +   1  01  1
 * +   5  015 5
 * -   0  15  5
 * -   1  5   5
 * -   5  -   5
 * -   0  -   5
*/
	debug	= true;
	Init( sds );
	Remove( RoleId_e(0) );
	Add( RoleId_e(0) );
	Add( RoleId_e(1) );
	Add( RoleId_e(5) );
	Remove( RoleId_e(0) );
	Remove( RoleId_e(1) );
	Remove( RoleId_e(5) );
	Remove( RoleId_e(0) );

}
