/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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

/*
 * processor.cpp
 *
 *  Created on: Apr 16, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Composer/Processor.h>
#include <fstream>

namespace Processor
{
	fstream 			LOG;
}
/**************************************************
 * print_struct
 *************************************************/
Table_class 		PrintTable		{ defaultopt };

Processor_class::print_struct::print_struct( stack_struct _ps )
{
	this->Table= &PrintTable;
	ps = _ps;
	assign_val();
	Table->opt.Ident = 0;
	Table->opt.Crlf 	= '\n';
	Table->AddColumn( "line nr"	, 4);
	Table->AddColumn( "cmd"		, 4);
	Table->AddColumn( "addr"		, 16);
	Table->AddColumn( "value"	, 6);
	Table->AddColumn( "comment"	, 40 );

}
void Processor_class::print_struct::assign_val()
{
	value = ( ps.cmd == CMD_KEY ) ? ps.key : ps.sds_value;
}
void Processor_class::print_struct::assign_addr_str( long value )
{
	addr_str= to_hex( value );
}
void Processor_class::print_struct::Line( stack_struct stack, interface_t* sds )
{
	if ( stack.cmd == CMD_TEXT )
	{
		return;
	}

	if(commit_time > 0)
		stack.str += ", commit in " + to_string( commit_time) + " [msec]";

	print_struct_t 	print{ stack };

	if( stack.chaddr == nullptr )
		print.assign_addr_str( (long)sds );
	else
		print.assign_addr_str( (long)stack.chaddr );

	Table->AddRow( 	print.ps.prgline,
					print.ps.cmdstr,
					print.addr_str,
					print.value,
					print.ps.str );
	commit_time		= 0;
}

/**************************************************
 * Assembler_class
 *************************************************/
Assembler_class::Assembler_class() :
	Logfacility_class("Assembler_class")
{
	pos_stack_struct::Clear();
	Regid			= REGUNDEF;
};
Assembler_class::~Assembler_class()
{
	DESTRUCTOR( className );
};

void Assembler_class::Ldx( regid_t id, reg_t value )
{
	register_arr[id] = value;
}
void Assembler_class::Add( regid_t id, reg_t value )
{
	register_arr[ id ]	+= value;
}
bool Assembler_class::Cmp( regid_t id )
{
	return ( register_arr[ id ] == 0 );
}
void Assembler_class::Cpy( regid_t id, uint8_t* addr )
{
	*addr = register_arr[ id ];
}
int Assembler_class::Out( regid_t id )
{
	return register_arr[ id ];
}

/**************************************************
 * Processor_class
 *************************************************/

Processor_class::Processor_class( Application_class* app )
	: Logfacility_class	( "Processor" )
	, Interface_base	( app->DaTA_p )
{
	this->App				= app;
	this->Sem 				= app->DaTA_p->Sem_p;
	this->Appstate 			= app->Appstate;
	this->Debug_Processing	= false;
	this->Autostart			= true;

	process_stack.clear		();

};
void Processor_class::Push_sys( CMD_e cmd, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "cmd";
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );

}
void Processor_class::Assembler_cmd( CMD_e cmd, regid_t regid, int value, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "ASX";
	stack_item.int_value= value;
	stack_item.sds_value= regid;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );

}

void Processor_class::Push_str( CSTRKEY_e value, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_STR;
	stack_item.cmdstr	= "str";
//	stack_item.key 		= key;
	stack_item.sds_value= value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_key( EVENTKEY_e key, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_KEY;
	stack_item.cmdstr	= "exec";
	stack_item.key 		= key;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::push_ifd( stack_struct stack )
{
	stack.prgline		= prgline;
	if( Assembler.Regid == Assembler.REGUNDEF )
	{
		stack.cmd 		= CMD_CHADDR;
		stack.cmdstr	= "ldc";
	}
	else
	{
		stack.cmd		= CMD_MOV;
		stack.sds_value = Assembler.Regid;
		stack.cmdstr	= " $"+to_string( Assembler.Regid );
		Assembler.Regid	= Assembler.REGUNDEF;
	}
	process_stack.push_back( move( stack ) );
};

void Processor_class::Push_wait( CMD_e cmd, int value, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "wait";
	stack_item.int_value= value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_text( string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_TEXT;
	stack_item.cmdstr	= "txt";
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::show_sta_state( string staid_str )
{
	StAId_e	staid 	= StAId_e( stoi( staid_str ) );
	if( staid == STAID_ALL )
		return;

	Table_class	T 	{};
	T.AddColumn		( "StA Info Field", 15 );
	T.AddColumn		( "Value", 5 );
	T.PrintHeader	();

	T.AddRow( "filled"	, sds_p->StA_state_arr[ staid ].filled );
	T.AddRow( "play"	, sds_p->StA_state_arr[ staid ].play );
	T.AddRow( "store"	, sds_p->StA_state_arr[ staid ].store);
	T.AddRow( "forget"	, sds_p->StA_state_arr[ staid ].forget );
	T.AddRow( "start"	, sds_p->StA_state_arr[ staid ].start );
	T.AddRow( "Volume%"	, (int)sds_p->StA_amp_arr[ staid ].amp );
};

void Processor_class::execute_cmd( EVENTKEY_e key )
{
 	Sds->Eventque.Add	( key );
	print.commit_time 	= wait_for_commit() ;
}

int Processor_class::wait_for_commit()
{
	if ( LogMask[TEST] ) return 0;

	if( not Appstate->IsRunning( sds_p, SYNTHID ) )
	{
		if( Autostart )
		{
			App->StartApp( SYNTHID, "nohup" );
		}
		else
		{
			Exception( "Synthesizer" + to_string( sds_p->SDS_Id ) + "not running", SIGILL );
		}
	}
	Timer.Start();
	Sem->Lock( PROCESSOR_WAIT );
	Value t_elapsed = Timer.Time_elapsed();
	return t_elapsed.val;
}

void Processor_class::Clear_process_stack()
{
	process_stack.clear();
}

void Processor_class::Set_prgline( int no, int nr )
{
	txtline = no;
	prgline = nr;
}
bool Processor_class::Set_regid( regid_t id )
{
	regid_t regid = check_range( Assembler.regid_range, id, "" );
	if ( regid == id )
	{
		Assembler.Regid	= id;
		return true;
	}
	else
	{
		Assembler.Regid = Assembler.REGUNDEF;
		return false;
	}
}

inline int slide_duration( uint8_t percent )
{	// return slide duration [msec]
	int duration = percent * 10 * max_slide_sec;
	return duration;
}
inline void sec_sleep( int sec )
{
	std::this_thread::sleep_for( std::chrono::seconds( sec ));
}
inline void msec_sleep( int msec )
{
	std::this_thread::sleep_for( std::chrono::milliseconds( msec ));
}
inline void wait_loop( int msec )
{
	if( msec < 1000 )
	{
//		coutf << "sleep " << msec << " [msec]" << endl ;
		msec_sleep( msec );
		return;
	}
	for( int n = 0 ; n < msec; n = n + 1000 )
	{
		coutf << "." ;
		sec_sleep( 1 );
	}
	msec = msec % 1000;
	msec_sleep( msec );
	coutf << endl;
};

void Processor_class::Execute()
{
	auto intro = [this  ]( int stack_count )
	{
		if ( stack_count == 0 )
		{
			Exception( "Cannot proceed empty stack", SIGILL );
		}
		if( not Appstate->IsRunning( sds_master, AUDIOID ) )
		{
			Exception( "AudioServer is down", SIGILL );
		}

		Info( "Proceeding stack. Item count ", (int) stack_count ) ;
		Info( "waiting for Synthesizer to start" );
	};

	auto log_init  = [ this ]( )
	{
		Processor::LOG.open( fs->composer_logfile, fstream::out );
		print.Table->opt.FILE = &Processor::LOG;
	};
	set<SDSId_t>	knownIds	{};

	auto clean_exit = [ this, &knownIds ](  )
	{
		Info( "cleanup SDS" );
		sds_master->Record_state 						= INACTIVE;
		sds_master->StA_state_arr[ STA_EXTERNAL ].store	= false;
		coutf << "Exit by user request" << endl;
		Appstate->Shutdown( knownIds );
		raise( SIGHUP );
	};

	SDSId_t sdscomId = sds_master->comId;
	Interface_base::Set_Sds();
	auto set_comid = [ this, &sdscomId, &knownIds ]( SDSId_t sdsid )
	{
		if( sdscomId == sdsid ) return;
		sdscomId	= sdsid;
		knownIds.insert( sdscomId );
		Interface_base::Set_Sds();
		sds_master->mixer_state.composer = true;
	};

	intro		( process_stack.size() );
	log_init 	();
	Sem->Init	();

	for ( size_t pos = 0; pos < process_stack.size(); pos++ )
	{
		stack_struct ps = process_stack[ pos ];
		set_comid( sds_master->comId );
		if( Appstate->IsExitserver( sds_master, COMPOSERID ) )
			clean_exit();

		if( Debug_Processing )
		{
			coutf << "Debug wait until <return> key";
			getc( stdin );
		}

		switch ( ps.cmd )
		{
			case CMD_CHADDR : // write char address
			{
				if ( ps.chaddr )
				{
					*ps.chaddr = ps.sds_value ;
				}
				break;
			}
			case CMD_STR : // write string
			{
				SharedData_class* Sdsp;
				switch (( ps.sds_value ))
				{
					case SESSIONSTR :
					case WAVFILESTR: { Sdsp = Sds_master	; break; }
					default 		: { Sdsp = Sds			; break; }
				} // switch ( ps.sds_value )
				Sdsp->Write_str( CSTRKEY_e( ps.sds_value ), ps.str );
				break;
			}
			case CMD_KEY : // write command key value
			{
				execute_cmd( ps.key );
				break;
			}
			case CMD_STASTATE :
			{
				show_sta_state( ps.str );
				break;
			}
			case CMD_SYS :
			{
				System_execute( ps.str );
				Timer.Wait(1);
				break;
			}
			case CMD_WAIT :
			{
				wait_loop( ps.int_value );
				break;
			}
			case CMD_COND_WAIT :
			{
				switch ( ps.int_value )
				{
					case DEBUGOFF 	: { Debug_Processing= false		; break; }
					case DEBUGON 	: { Debug_Processing= true		; break; }
					case KEY 		: { getc			( stdin )	; break; }
					case AUTO		: { uint8_t value	= sds_p->Noteline_sec;
										ps.str = "Pause "+ to_string(value) + "[sec]";
										sec_sleep		( value );
										break; }
					case DYNVOL		: { uint8_t value	= sds_p->StA_amp_arr[sds_p->StA_Id].slide;
										int 	msec	= slide_duration( value );
										ps.str 			= "Pause "+ to_string(msec) + "[msec]";
										msec_sleep		( msec );
										break; }
					case DYNFRQ		: { uint8_t value	= sds_p->features[OSCID].frq_slide_value;
										int 	msec	= slide_duration( value );
										ps.str 			= "Pause "+ to_string(msec) + "[msec]";
										msec_sleep		( msec );
										break; }
				} 	// switch  ps.int_value
				break;
			}
			case CMD_TEXT :
			{
				coutf << ps.str << endl;
				Processor::LOG.flush() << ps.str << endl;
				break;
			}
			case CMD_JMP :
			{	// if ( reg sds != 0 )
				if( not Assembler.Cmp( ps.sds_value ) )
					// return to the beginning of the sub routine
					// and execute it again
					pos = Assembler.Return();

				else
					// continue with next process stack and
					// forget the beginning of the sub routine
					Assembler.Pop();
				break;
			}
			case CMD_POS :
			{	// store this pos as a return pos
				Assembler.Push( pos );
				break;
			}
			case CMD_ADD :
			{				   // reg sds = reg sds + int
				Assembler.Add( ps.sds_value, ps.int_value );
				break;
			}
			case CMD_LDX :
			{	// reg sds = int
				Assembler.Ldx( ps.sds_value, ps.int_value );
				break;
			}
			case CMD_MOV :
			{
				regid_t regid = ps.sds_value;
				ps.str += to_string( Assembler.Out( regid ) );
				Assembler.Cpy( regid, ps.chaddr );
				break;
			}
			case CMD_OUT :
			{
				string out = to_string( Assembler.Out( ps.sds_value ) );
				ps.str = "$" + to_string( ps.sds_value) + " = " + out;
				coutf << ps.str << endl;
				break;
			}
			case CMD_EXIT :
			{
				coutf << ps.prgline << endl;
				print.Line( ps, sds_p );

				Processor::LOG.flush() << ps.prgline << endl;;
				Sem->Release( SEMAPHORE_EXIT );
				clean_exit();
				break;
			}
			default :
			{
				Exception( to_string( ps.prgline) + " default SIGINT", SIGILL);
				break;
			}

		} // end switch
		print.Line( ps, sds_p );

	}

	Comment( INFO, "Execute() out of program loop");
	Comment( INFO, "Details are in composer log " + fs->composer_logfile );
	clean_exit();
}

void Processor_class::Test_Processor()
{
	TEST_START( className );
	Comment( TEST, "Wait for commit timeout" );
	Sds->Dump_ifd();

	Sem->Reset( PROCESSOR_WAIT );

	Push_str( INSTRUMENTSTR, "default" );
	Push_key( SETINSTRUMENTKEY, "SETINSTRUMENTKEY" );
	Push_ifd( &sds_p->SHMID, 0_uint, "char" );
	Push_ifd( &sds_p->StA_state_arr[0].forget, true, "bool" );
	Push_ifd( &sds_p->adsr_arr[OSCID].bps, 0_uint, "beat duration" );

	Push_sys( CMD_EXIT, "exit" );
	Execute();
	Sds->Restore_ifd();

	TEST_END( className );
}
