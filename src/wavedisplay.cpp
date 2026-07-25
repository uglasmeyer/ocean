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
 * Wavedisplay.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */
#include <Wavedisplay.h>
#include <Frequency.h>
#include <Fourier.h>

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
	this->param_full 	= param_struct();
	this->param_split	= { 0, wavedisplay_len/2, 1, max_frames };
	this->param_fft		= { 0, wavedisplay_len  , 0, 0 };
	this->param_cursor	= { 0, wavedisplay_len	, 0, 0 };
	this->param_adsr	= { 0, adsrdisplay_len	, 0, 0 };
	this->frame_counter	= 0;
	this->offs 			= 0;

//	this->wd_mode		= FULL_WD;
	this->Wd_ptr		= wd_ptr_struct();
	this->Data_p		= nullptr;
	this->wd_frames		= 0;
	this->debug_right	= true;
	set_fourierfile		();
	fourier->Load_coeff	( fourierBinFile );

	Assert_equal( roleNames.size()-1, (size_t) ROLE_SIZE );
}

Wavedisplay_class::~Wavedisplay_class()
{
	fourier->Save_coeff( fourierBinFile );
}

void Wavedisplay_class::set_fourierfile()
{
	string 	dir 			= Sds_p->fs->vardir + "SDS_" + to_string( sds->SDS_Id ) + "/";
			fourierBinFile 	= dir + "fourier.bin";
}

void Wavedisplay_class::gen_wave_data( WdModeID_t mode )
{// max_fames - step*len - _offs > 0 => max_offs = max_frames - step*len

	if ( wd_frames == 0 )
	{
		Comment( WARN, "Zero wavedisplay frames: ",	rolename, oscname, wd_frames );
		return;
	}

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

	auto gen_full	= [ this ]( param_t param )
	{
		const float 	df	= sds->WD_state.zoom * ( wd_frames ) / float( param.len ) ;
		if ( df < 1.0 )
			return;
		float 			f	= 0.0;
		std::ranges::for_each( display_data, [ this, &f, df ]( Data_t& Data )
		{
			buffer_t 	m 	= f;
						Data= Data_p[m];
						f 	= f + df;
		} );
	};

	auto gen_fft 	= [ this, gen_full ]( param_t param )
	{
		uint8_t idx 		= sds->spectrum_arr[OSCID].frqidx[0];// Main frequency of the oscillation
		frq_t 	frq 		= max( (frq_t)16.0, frqArray[idx] );
				param.len 	= update_frequency * frq ;
		gen_full			( param );
		fourier->Amplitude	( display_data );
	};

	auto gen_flow 	= [ this ]( param_t param )
	{
		uint idx 	= 0;
		uint step	= sds->WD_state.zoom * param.step;
		for ( buffer_t n = offs; n < param.len*step + offs; n = n + step )
		{
			Data_t	value 				= Data_p[ n ] ;
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
			display_data[ n ] 	= Data_p[idx];
			idx += step;
		}
		set_Wdptr( WD.data.roleId );
	};

	if ( Data_p == nullptr )
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
		case FULL_WD :
		{
			param = param_full;
			gen_full( param );
			break;
		}
		case FLOW_WD :
		{
			param = param_flow;
			param.max_offs = wd_frames - param.len*param.step;
			gen_flow( param );
			break;
		}
		case RANGE_WD :
		{
			param = param_full;
			gen_full( param );
			break;
		}
		case DEBUG_WD :
		{
			param = param_split;
			gen_debug( param );
			break;
		}
		case FFT_WD :
		{
			param = param_fft;
			gen_fft( param );
			break;
		}
		case ADSR_WD :
		{
			param = param_adsr;
			gen_adsr( param );
			break;
		}
		default :
		{
			Comment(ERROR,"No valid wave display type provided to gen_cxwave_data");
		}
	} // switch wd_mode

	frame_counter++;
	offs = offs + param.drift * frame_counter;
	if ( offs > param.max_offs )
	{
		frame_counter 	= 0;
		offs			= 0;
	}
}
void Wavedisplay_class::write_display_arr( Data_t* dst, int len )
{
	for( int n = 0; n < len; n++ )
	{
		dst[n]	= display_data[n];
	}
}


void Wavedisplay_class::WriteData()
{
	if( WD.data.adsrmode )
	{
		gen_wave_data		( ADSR_WD );
		write_display_arr	( &sds->adsrdata[0], adsrdisplay_len );
		if( WD.data.fftmode )
		{
			gen_wave_data			( FFT_WD );
			write_display_arr		( &sds->fft_data[0], fft_display_len );
			if( WD.data.fftmode )
			{
				fourier->SetResolution	( sds->resolutionIdx );
				fourier->GenCoeff		();
				WD.data.fftmode			= false;
				sds->WD_state.fftmode 	= false;
			}
		}
		WD.data.adsrmode		= false;
		sds->WD_state.adsrmode	= false;
	}

	if ( WD.data.mode == DEBUG_WD )
	{
		gen_wave_data( DEBUG_WD );
		if ( not debug_right )
			write_display_arr( &sds->wavedata[0], wavedisplay_len );
	}
	else
	{
		gen_wave_data( WD.data.mode );
		write_display_arr( &sds->wavedata[0], wavedisplay_len );
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
	set_Wdmode			();

	Comment( DEBUG, "Wavedisplay ptr set to: ", rolename, oscname, wd_frames );
}

// pos is frames 	-> unit = 1
// pos is records 	-> unit = min_frames
void Wavedisplay_class::Set_wdcursor( buffer_t pos, buffer_t unit )
{	// convert scanner position or record position into wavedisplay position
	if( WD.data.mode == FFT_WD ) return;// no time coordinate for fftmode

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

void Wavedisplay_class::set_Wdmode()
{
	if (( WD.data.mode == DEBUG_WD ) or ( WD.data.roleId == AUDIOROLE ) )
	{
		WD.data.fftmode = false;
		WD.data.adsrmode= false;
	}
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
	set<int> osctype_set = range_set(0, 2);
	if ( not osctype_set.contains( wd_type ) )
	{
		Exception( "wd_type out of bounds" );
	}

//	wd_ptr_struct	wd {};
	Wd_ptr = { wd_role, wd_type, ptr, frames_p };
				data_ptr_mtx[ wd_role ][ wd_type ]	= Wd_ptr;
}

void Wavedisplay_class::TestWd()
{
	TEST_START( className );

	sds->WD_state.fftmode = true;
	sds->WD_state.oscId = OSCID;
	Set_WdData();
	Assert_lt( (buffer_t) 0, sds->WD_state.frames );
	Assert_equal( WD.data.mode, FULL_WD );
	WriteData();

	sds->WD_state.adsrmode = true;
	Set_WdData();
	Assert_equal( WD.data.mode, FULL_WD );

	TEST_END( className );

	fourier->TestFourier();
}

/**************************************************
 * WfDisplay_que_struct
 *************************************************/
WfDisplay_que_struct::WfDisplay_que_struct( interface_t* _sds, sta_role_map* _map )
{
	this->sds			= _sds;
	this->roleMap		= _map;
	Init				( sds );
}
void WfDisplay_que_struct::Set_que( interface_t* _sds )
{
	this->sds = _sds;
	this->Que = Que_arr[sds->SDS_Id];
	Add( sds->WD_state.roleId );
}
void WfDisplay_que_struct::Add( RoleId_e role )
{
	if( role == ROLE_SIZE )
		return;
	if ( Que.size() == 0 )
	{
		Que.push_back	( role );
	}
	if( Que.back() != role )
	{
		Que.push_back	( role );
	}
	prev_role		= Que.back();
	Set_prev();
}
void WfDisplay_que_struct::Init( interface_t* _sds )
{
	Set_que		( _sds );
	Que.clear	();
	for( StAId_e staid : StAIds )
	{
		if( sds->StA_state_arr[ staid ].play )
			Add( roleMap->GetRoleid( staid) );
	}
	Set_prev	();
}
void WfDisplay_que_struct::Remove( RoleId_e role )
{
	if( Que.empty() )
	{
		Add( sds->WD_state.roleId );
		return;
	}
	else
	{
		roleid_vec_t tmp{};
		for( RoleId_e _role : Que )
		{
			if( role != _role )
				tmp.push_back( _role );
		}
		Que = tmp;
	}
	Set_prev();
}
void WfDisplay_que_struct::Set_prev()
{
	if( Que.size() == 0 )
		prev_role = sds->WD_state.roleId;
	else
		prev_role = Que.back();
	sds->WD_state.roleId = prev_role;

	Show( true );
}

void WfDisplay_que_struct::Show( bool debug )
{
	if ( debug )
		coutf << 	"Display_fifo RoleIds: (" <<
					(int)sds->WD_state.roleId << "," <<
					(int)prev_role << ") " <<
					show_items( Que ) << endl;
}

