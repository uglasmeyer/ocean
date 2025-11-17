//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * processor.cpp
 *
 *  Created on: Apr 16, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Composer/Processor.h>

/**************************************************
 * print_struct
 *************************************************/
Processor_class::print_struct::print_struct( stack_struct _ps )
{
	ps = _ps;
	assign_addr_str();
	assign_val();
	Table.opt.Ident = 0;
	Table.opt.Crlf 	= '\n';
	Table.AddColumn( "line nr"	, 4);
	Table.AddColumn( "cmd"		, 4);
	Table.AddColumn( "addr"		, 16);
	Table.AddColumn( "value"	, 6);
	Table.AddColumn( "comment"	, 40 );

}
void Processor_class::print_struct::assign_val()
{
	value = ( ps.cmd == CMD_KEY ) ? ps.key : ps.value;
}
void Processor_class::print_struct::assign_addr_str()
{
	addr_str= Str.to_hex( (long)ps.chaddr );
}
void Processor_class::print_struct::Line( stack_struct ps )
{
	if ( ps.cmd == CMD_TEXT )
	{
		return;
	}
	if(commit_time > 0)
		ps.str += ", commit in " + to_string( commit_time) + " [msec]";
	print_struct_t 	print{ ps };
	Table.AddRow( 	print.ps.prgline,
					print.ps.cmdstr,
					print.addr_str,
					print.value,
					print.ps.str );
	commit_time		= 0;
}

/**************************************************
 * Processor_class
 *************************************************/
Processor_class::Processor_class( Application_class* app ) :
	Logfacility_class("Processor"),
	sdsstate_struct()
{
	className		= Logfacility_class::className;
	this->App		= app;
	this->Sds 		= app->Sds;
	this->sds		= app->sds;
	this->Sem 		= app->DaTA->Sem_p;
	this->Appstate 	= app->Appstate;

	process_stack.clear();

};
void Processor_class::Push_cmd( CMD_e cmd, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "cmd";
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );

}

void Processor_class::Push_str( EVENTKEY_e key, uint8_t value, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_STR;
	stack_item.cmdstr	= "str";
	stack_item.key 		= key;
	stack_item.value 	= value;
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
	stack.prgline	= prgline;
	stack.cmd 		= CMD_CHADDR;
	stack.cmdstr	= "ldc";
	process_stack.push_back( move( stack ) );
};

void Processor_class::Push_wait( CMD_e cmd, int value, string str )
{
	stack_struct stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "wait";
	stack_item.value 	= value;
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

void Processor_class::execute_str( stack_struct& ps )
{
	char* addr = nullptr;
	switch( ps.value )
	{
		case INSTRUMENTSTR_KEY 	: { addr = Sds->addr->Instrument;break; }
		case NOTESSTR_KEY 		: { addr = Sds->addr->Notes; break; }
		case OTHERSTR_KEY 		: { addr = Sds->addr->Other; break; }
		default 				: { addr = nullptr; break; }
	}
	assert( addr != nullptr );
	ps.chaddr = (sds_value_t*)addr;
	this->Sds->Write_str( ps.value, ps.str );
	if ( ps.value == NOTESSTR_KEY )
		Sds->addr->NotestypeId = NTE_ID;
	print.Line(  ps );

	ps.cmd		= CMD_KEY;
	ps.cmdstr	= "exec";
	ps.chaddr	= nullptr;
	ps.str 		= "set";
	Sds->Eventque.add( ps.key );
	print.commit_time = wait_for_commit() ;


}
int Processor_class::wait_for_commit()
{
	Timer.Start();
	if ( not LogMask[TEST] )
		Sem->Lock( PROCESSOR_WAIT );
	Value t_elapsed = Timer.Time_elapsed();
	return t_elapsed.val;
}

void Processor_class::Clear_process_stack()
{
	process_stack.clear();
}

void Processor_class::Set_prgline( int nr )
{

	prgline = nr;
}


auto wait_loop = []( int sec )
{
	for( int n = 0 ; n < sec; n++ )
	{
		coutf << "." ;
		this_thread::sleep_for( std::chrono::seconds( 1 ));
	}
	coutf << endl;
};

void Processor_class::Execute()
{
	auto intro = [this  ]( int stack_count )
	{
		if ( stack_count == 0 )
		{
			Comment( ERROR, "Cannot proceed empty stack");
			return;
		}
		else
		{
			Info( "Proceeding stack. Item count ", (int) stack_count ) ;
			Info( "waiting for Synthesizer to start" );
		}
	};
	auto log_init  = [ this ]( )
	{
		LOG.open( App->Cfg->fs->log_file, fstream::out );
		print.Table.opt.FILE = &LOG;
	};

	intro		( process_stack.size() );
	log_init 	();
	Sem->Init	();

	for ( stack_struct ps : process_stack )
	{
		if( Appstate->IsExitserver( Sds->addr, COMPID ) )
			return;

		switch ( ps.cmd )
		{
			case CMD_CHADDR : // write char address
			{
				if ( ps.chaddr )
					*ps.chaddr = (char) ps.value ;
				break;
			}
			case CMD_KEY : // write command key value
			{
				Sds->Eventque.add( ps.key );
				print.commit_time = wait_for_commit();
				break;
			}
			case CMD_EXE :
			{
				System_execute( ps.str );
				Timer.Wait(1);
				break;
			}
			case CMD_WAIT :
			{
				wait_loop( ps.value );
				break;
			}
			case CMD_COND_WAIT :
			{
				if  ( ps.value == 1 )
				{
					getc( stdin );
				}
				else
				{
					uint8_t sec = sds->Noteline_sec;
					this_thread::sleep_for( std::chrono::seconds(sec));
					sds->Noteline_sec = 0;
				}
				break;
			}
			case CMD_STR : // write string
			{
				execute_str( ps );
				break;
			}
			case CMD_TEXT :
			{
				coutf << ps.str << endl;
				LOG.flush() << ps.str << endl;
				break;
			}
			case CMD_EXIT :
			{
				coutf << ps.prgline << endl;
				print.Line( ps );

				LOG.flush() << ps.prgline << endl;;
				Sem->Release( SEMAPHORE_EXIT );
				return;
				break;
			}
			default :
			{
				Exception( to_string( ps.prgline) + " default SIGINT");
				break;
			}
		} // end switch

		print.Line( ps );
	}

	Comment( INFO, "Execute() terminated without exit/release");
	Comment( INFO, "Read composer log " + App->Cfg->fs->log_file );
	Comment( INFO, "for more information" );
}

void Processor_class::Test_Processor()
{
	TEST_START( className );
	Comment( TEST, "Wait for commit timeout" );
	Sds->Dump_ifd();

	Sem->Reset( PROCESSOR_WAIT );

	Push_str( SETINSTRUMENTKEY, INSTRUMENTSTR_KEY, "default" );
	Push_ifd( &sds->SHMID, 0_uint, "char" );
	Push_ifd( &sds->StA_state_arr[0].forget, true, "bool" );
	Push_ifd( &sds->adsr_arr[OSCID].bps, 0_uint, "beat duration" );
	Push_key( ADSR_KEY, "set beat duration" );

	Push_cmd( CMD_EXIT, "exit" );
	Execute();
	Sds->Restore_ifd();

	TEST_END( className );
}
