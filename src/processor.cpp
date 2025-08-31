/*
 * processor.cpp
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#include <Processor.h>

Processor_class::print_struct::print_struct( stack_struct_t _ps )
{
	ps = _ps;
	assign_addr_str();
	assign_val();
}
void Processor_class::print_struct::assign_val()
{
	value = ( ps.cmd == CMD_KEY ) ? ps.key : ps.value;

}
void Processor_class::print_struct::assign_addr_str()
{
	for( long addr : { (long)ps.boaddr, (long)ps.chaddr, (long)ps.uiaddr, (long)ps.staddr } )
		if( addr > 0 )
			addr_str= Str.to_hex( addr );
}


void Processor_class::printLn( stack_struct ps )
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
void Processor_class::Push_cmd( uint8_t cmd, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "cmd";
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );

}

void Processor_class::Push_str( uint8_t key, uint8_t value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_STR;
	stack_item.cmdstr	= "str";
	stack_item.key 		= key;
	stack_item.value 	= value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_key( uint8_t key, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_KEY;
	stack_item.cmdstr	= "exec";
	stack_item.key 		= key;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( uint8_t* chaddr, uint8_t value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_CHADDR;
	stack_item.cmdstr	= "ldc";
	stack_item.chaddr 	= chaddr;
	stack_item.value 	= (char) value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( char* chaddr, char value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_CHADDR;
	stack_item.cmdstr	= "ldc";
	stack_item.staddr 	= chaddr;
	stack_item.value 	= value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
};
void Processor_class::Push_ifd( bool* boaddr, bool value, string str )
{
	stack_struct_t 	stack_item 	= stack_struct();
	stack_item.prgline 	= prgline;
	stack_item.cmd 		= CMD_BOADDR;
	stack_item.cmdstr	= "ldb";
	stack_item.boaddr 	= boaddr;
	stack_item.value 	= (bool) value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( int* uiaddr, int value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_UIADDR;
	stack_item.cmdstr	= "ldu";
	stack_item.uiaddr 	= uiaddr;
	stack_item.value 	= value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_wait( uint8_t cmd, int value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= cmd;
	stack_item.cmdstr	= "wait";
	stack_item.value 	= value;
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_text( string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline	= prgline;
	stack_item.cmd 		= CMD_TEXT;
	stack_item.cmdstr	= "txt";
	stack_item.str 		= str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::execute_str( stack_struct_t& ps )
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
	ps.staddr = addr;
	this->Sds->Write_str( ps.value, ps.str );
	if ( ps.value == NOTESSTR_KEY )
		Sds->addr->NotestypeId = NTE_ID;
	printLn(  ps );

	ps.cmd		= CMD_KEY;
	ps.cmdstr	= "exec";
	ps.staddr	= nullptr;
	ps.str 		= "set";
	Sds->Event( ps.key );
	commit_time = wait_for_commit() ;


}
int Processor_class::wait_for_commit()
{
	Timer.Start();
	if ( not LogMask[TEST] )
		Sem->Lock( PROCESSOR_WAIT );
	Value t_elapsed = Timer.Time_elapsed();
//	printf(" > commit in %d [msec]\n",t_elapsed.val );
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

auto intro = [ ]( Processor_class* P, int stack_count )
{
	if ( stack_count == 0 )
	{
		P->Comment( ERROR, "Cannot proceed empty stack");
		return;
	}
	else
	{
		P->Info( "Proceeding stack. Item count ", (int) stack_count ) ;
		P->Info( "waiting for Synthesizer to start" );
	}
};
auto log_init  = [ ]( Processor_class* P )
{
	//	P->Set_Loglevel( TEST , false );
	P->LOG.open( file_structure().log_file, fstream::out );
	P->Table.opt.FILE = &P->LOG;
};

void Processor_class::Execute()
{
	intro	( this, process_stack.size() );
	log_init( this );

	Sem->Init();

	for ( stack_struct_t ps : process_stack )
	{
		if( Appstate->IsExitserver( Sds->addr, COMPID ) )
			return;

		switch ( ps.cmd )
		{
			case CMD_CHADDR : // write char address
			{
				if ( not ps.chaddr == 0 )
					*ps.chaddr = (char) ps.value ;
				break;
			}
			case CMD_BOADDR : // write char address
			{
				if ( not ps.boaddr == 0 )
					*ps.boaddr = (bool) ps.value ;
				break;
			}
			case CMD_UIADDR : // write uint16_t address
			{
				if ( not ps.uiaddr == 0 )
					*ps.uiaddr = (uint16_t) ps.value;
				break;
			}
			case CMD_KEY : // write command key value
			{
				Sds->Event( ps.key );
				commit_time = wait_for_commit();
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
				if  ( ps.value < 0 )
				{
					getc(stdin);
				}
				else
				{
					this_thread::sleep_for( std::chrono::seconds( ps.value));
				}
				break;
			}
			case CMD_COND_WAIT :
			{
				uint8_t sec = sds->Noteline_sec;
				this_thread::sleep_for( std::chrono::seconds(sec));
				sds->Noteline_sec = 0;
				break;
			}
			case CMD_STR : // write string
			{
				execute_str( ps );
				break;
			}
			case CMD_TEXT :
			{
				printf("%s\n", ps.str.data());
				LOG.flush() << ps.str << endl;  ;
				break;
			}
			case CMD_EXIT :
			{
				printf("%d \n", ps.prgline);
				printLn( ps );

				LOG.flush() << ps.prgline << endl;;
				Sem->Release( SEMAPHORE_EXIT );
				return;
				break;
			}
			default :
			{
				EXCEPTION( to_string( ps.prgline) + " default SIGINT");
				break;
			}
		} // end switch

		printLn( ps );
	}

	Comment( INFO, "Execute() terminated without exit/release");
	Comment( INFO, "Read composer log " + file_structure().log_file );
	Comment( INFO, "for more information" );
}

void Processor_class::Test_Processor()
{
	TEST_START( className );
	Comment( TEST, "Wait for commit timeout" );
	Sem->Reset( PROCESSOR_WAIT );

	Push_str( SETINSTRUMENTKEY, INSTRUMENTSTR_KEY, "default" );
	Push_cmd( CMD_EXIT, "exit" );
	Execute();
	TEST_END( className );
}
