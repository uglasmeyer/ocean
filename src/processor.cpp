/*
 * processor.cpp
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#include <Processor.h>

void Processor_class::Push_cmd( uint8_t cmd, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= cmd,
		.key    = 0,
		.boaddr = nullptr,
		.chaddr	= nullptr,
		.uiaddr = nullptr,
		.value  = 0,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
}
void Processor_class::Push_str( uint8_t key, char ch, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= CMD_STR,
		.key    = key,
		.boaddr = nullptr,
		.chaddr	= nullptr,
		.uiaddr = nullptr,
		.value  = ch,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_key( uint8_t key, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= CMD_KEY,
		.key    = key,
		.boaddr = nullptr,
		.chaddr	= nullptr,
		.uiaddr = nullptr,
		.value  = 0,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( uint8_t* chaddr, uint8_t value, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= CMD_CHADDR,
		.key    = 0,
		.boaddr = nullptr,
		.chaddr	= chaddr,
		.uiaddr = nullptr,
		.value  = value,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
};


void Processor_class::Push_ifd( bool* boaddr, bool value, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= CMD_BOADDR,
		.key    = 0,
		.boaddr	= boaddr,
		.chaddr = nullptr,
		.uiaddr = nullptr,
		.value  = (bool) value,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( float* uiaddr, float value, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= CMD_UIADDR,
		.key    = 0,
		.boaddr = nullptr,
		.chaddr	= nullptr,
		.uiaddr = uiaddr,
		.value  = (int) rint( value ),
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
};


void Processor_class::Push_wait( uint8_t cmd, int value, string str )
{
	stack_struct_t stack_item =
	{
		.prgline= prgline,
		.cmd 	= cmd,
		.key    = 0,
		.boaddr = nullptr,
		.chaddr	= nullptr,
		.uiaddr = nullptr,
		.value  = value,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );

}

void Processor_class::Push_text( string str )
{
	stack_struct_t stack_item =
	{
		.prgline = prgline,
		.cmd 	= CMD_TEXT,
		.key    = 0,
		.boaddr = nullptr,
		.chaddr	= nullptr,
		.uiaddr = nullptr,
		.value  = 0,
		.str	= str
	};
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::wait_for_commit()
{
	int i = 0;
	while ( (ifd->KEY != NULLKEY ) and ( ifd->Synthesizer == RUNNING ) )
	{
		Wait(1 * MILLISECOND );
		i++;
	}
	printf(", commit in %d [msec]",i);
}

void Processor_class::Clear_process_stack()
{
	process_stack.clear();
}

void Processor_class::Set_prgline( int nr )
{
	prgline = nr;
}
void Processor_class::Execute()
{
	int len = process_stack.size();
	if ( len == 0 )
	{
		Comment( ERROR, "Cannot proceed empty stack");
		return;
	}
	else
		Comment( INFO, "Proceeding stack. Item count " + to_string( len )) ;

	cout << "waiting for Synthesizer to start" << endl;
	sds->Reset_ifd();
	sds->Commit();
	stack_struct_vec::iterator itr;

	for( itr=process_stack.begin(); itr != process_stack.end(); itr++ )
	{
		stack_item = *itr;

		sds->addr->Composer = RUNNING;
		switch ( (int)stack_item.cmd )
		{
		case CMD_CHADDR : // write char address
		{
			printf("%d ldc %p %d \t| %s", stack_item.prgline, stack_item.chaddr, stack_item.value, stack_item.str.data() );
			if ( not stack_item.chaddr == 0 )
				*stack_item.chaddr = (char) stack_item.value ;
			break;
		}
		case CMD_BOADDR : // write char address
		{
			printf("%d ldc %p %d \t| %s", stack_item.prgline, stack_item.boaddr, stack_item.value, stack_item.str.data() );
			if ( not stack_item.boaddr == 0 )
				*stack_item.boaddr = (bool) stack_item.value ;
			break;
		}
		case CMD_UIADDR : // write uint16_t address
		{
			printf("%d ldu %p %d \t| %s", stack_item.prgline, stack_item.uiaddr, stack_item.value, stack_item.str.data() );
			if ( not stack_item.uiaddr == 0 )
				*stack_item.uiaddr = (uint16_t) stack_item.value;
			break;
		}
		case CMD_KEY : // write command key value
		{
			printf("%d ldc %p %d \t| %s", stack_item.prgline,  &ifd->KEY, stack_item.key, stack_item.str.data() );
			ifd->KEY = stack_item.key;
			wait_for_commit();
			break;
		}
		case CMD_EXE :
		{
			printf("%d exe %s ", stack_item.prgline, stack_item.str.data() );
			system_execute( stack_item.str );
			break;
		}
		case CMD_WAIT :
		{
			if  ( stack_item.value < 0 )
			{
				printf( "%d %s", stack_item.prgline, stack_item.str.data() );
				getc(stdin);
			}
			else
			{
				printf("%d wait %d", stack_item.prgline, stack_item.value );
				Wait( stack_item.value*SECOND);
			}
			break;
		}
		case CMD_COND_WAIT :
		{
			uint8_t sec = ifd->Noteline_sec;
			printf("%d conditional wait %d\t\t| %s", stack_item.prgline,  sec, stack_item.str.data() );
			Wait( sec * SECOND);
			ifd->Noteline_sec = 0;
			break;
		}
		case CMD_STR : // write string
		{
			char* addr = nullptr;
			switch( stack_item.value )
			{
				case INSTRUMENTSTR_KEY 	: { addr = sds->addr->Instrument;break; }
				case NOTESSTR_KEY 		: { addr = sds->addr->Notes; break; }
				case OTHERSTR_KEY 		: { addr = sds->addr->Other; break; }
				default 				: { addr = nullptr; break; }
			}
			assert( addr != nullptr );
			printf("%d ldc %p %s \n", stack_item.prgline, addr, stack_item.str.data() );
			this->sds->Write_str( stack_item.value, stack_item.str );

			printf("%d ldc %p %d \t| set %s ", stack_item.prgline, &ifd->KEY, stack_item.key, stack_item.str.data() );
			ifd->KEY = stack_item.key;
			wait_for_commit();
			break;
		}
		case CMD_TEXT :
		{
			printf("%s", stack_item.str.data());
			break;
		}
		case CMD_EXIT :
		{
			printf("%d \n", stack_item.prgline);
			return;
			break;
		}
		default :
		{
			printf("%d default SIGINT", stack_item.prgline);
			Exception( );//raise( SIGINT);
			break;
		}
		} // end switch
		printf("\n");
	}
}

