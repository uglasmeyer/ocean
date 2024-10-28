/*
 * processor.cpp
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#include <Processor.h>

void Processor_class::Push_cmd( uint8_t cmd, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = cmd;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_str( uint8_t key, uint8_t value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = CMD_STR;
	stack_item.key = key;
	stack_item.value = value;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_key( uint8_t key, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = CMD_KEY;
	stack_item.key = key;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( uint8_t* chaddr, uint8_t value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = CMD_CHADDR;
	stack_item.chaddr = chaddr;
	stack_item.value = (char) value;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( bool* boaddr, bool value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = CMD_BOADDR;
	stack_item.boaddr = boaddr;
	stack_item.value = (bool) value;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_ifd( float* uiaddr, float value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = CMD_UIADDR;
	stack_item.uiaddr = uiaddr;
	stack_item.value = value;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
};

void Processor_class::Push_wait( uint8_t cmd, int value, string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = cmd;
	stack_item.value = value;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::Push_text( string str )
{
	stack_struct_t stack_item = stack_struct();
	stack_item.prgline = prgline;
	stack_item.cmd = CMD_TEXT;
	stack_item.str = str;
	process_stack.push_back( move( stack_item ) );
}

void Processor_class::wait_for_commit()
{
	Timer.Start();
	Sem->Lock( PROCESSOR_WAIT );
	long int t_elapsed = Timer.Time_elapsed();
	printf(", commit in %ld [msec]",t_elapsed );
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
	int stack_count = process_stack.size();
	if ( stack_count == 0 )
	{
		Comment( ERROR, "Cannot proceed empty stack");
		return;
	}
	else
		Comment( INFO, "Proceeding stack. Item count " + to_string( stack_count )) ;

	cout << "waiting for Synthesizer to start" << endl;

	sds->Reset_ifd();
//	Exception( "uninitialized memory");
	sds->Commit();

	FILE* LOG;
	LOG = fopen( file_structure().log_file.data(), "w");

	Sem->Init();

	for ( stack_struct_t SI : process_stack )
	{
		sds->addr->Composer = RUNNING;
		char* str = SI.str.data();

		switch ( (int)SI.cmd )
		{
		case CMD_CHADDR : // write char address
		{
			printf("%d ldc %p %d \t| %s", SI.prgline, SI.chaddr, SI.value, str );
			fprintf(LOG, "%d ldc %p %d \t| %s", SI.prgline, SI.chaddr, SI.value, str );
			if ( not SI.chaddr == 0 )
				*SI.chaddr = (char) SI.value ;
			break;
		}
		case CMD_BOADDR : // write char address
		{
			printf("%d ldc %p %d \t| %s", SI.prgline, SI.boaddr, SI.value, str );
			fprintf(LOG, "%d ldc %p %d \t| %s", SI.prgline, SI.boaddr, SI.value, str );
			if ( not SI.boaddr == 0 )
				*SI.boaddr = (bool) SI.value ;
			break;
		}
		case CMD_UIADDR : // write uint16_t address
		{
			printf("%d ldu %p %d \t| %s", SI.prgline, SI.uiaddr, SI.value, str );
			fprintf(LOG, "%d ldu %p %d \t| %s", SI.prgline, SI.uiaddr, SI.value, str );
			if ( not SI.uiaddr == 0 )
				*SI.uiaddr = (uint16_t) SI.value;
			break;
		}
		case CMD_KEY : // write command key value
		{
			printf("%d ldc %p %d \t| %s", SI.prgline,  &ifd->KEY, SI.key, str );
			fprintf(LOG, "%d ldc %p %d \t| %s", SI.prgline,  &ifd->KEY, SI.key, str );
			ifd->KEY = SI.key;
			wait_for_commit();
			break;
		}
		case CMD_EXE :
		{
			printf("%d exe %s ", SI.prgline, str );
			fprintf(LOG, "%d exe %s ", SI.prgline, str );
			system_execute( SI.str );
			break;
		}
		case CMD_WAIT :
		{
			if  ( SI.value < 0 )
			{
				printf( "%d %s", SI.prgline, str );
				fprintf(LOG, "%d %s", SI.prgline, str );
				getc(stdin);
			}
			else
			{
				printf("%d wait %d", SI.prgline, SI.value );
				fprintf(LOG, "%d wait %d", SI.prgline, SI.value );
			    this_thread::sleep_for(chrono::seconds( SI.value));
			}
			break;
		}
		case CMD_COND_WAIT :
		{
			uint8_t sec = ifd->Noteline_sec;
			printf("%d conditional wait %d sec\t| %s (%d)", SI.prgline,  sec, str, sec );
			fprintf(LOG, "%d conditional wait %d sec\t| %s (%d)", SI.prgline,  sec, str, sec );
		    this_thread::sleep_for(chrono::seconds(sec));
			ifd->Noteline_sec = 0;
			break;
		}
		case CMD_STR : // write string
		{
			char* addr = nullptr;
			switch( SI.value )
			{
				case INSTRUMENTSTR_KEY 	: { addr = sds->addr->Instrument;break; }
				case NOTESSTR_KEY 		: { addr = sds->addr->Notes; break; }
				case OTHERSTR_KEY 		: { addr = sds->addr->Other; break; }
				default 				: { addr = nullptr; break; }
			}
			assert( addr != nullptr );
			printf("%d ldc %p %s \n", SI.prgline, addr, str );
			fprintf(LOG, "%d ldc %p %s \n", SI.prgline, addr, str );
			this->sds->Write_str( SI.value, SI.str );

			printf("%d ldc %p %d \t| set %s ", SI.prgline, &ifd->KEY, SI.key, str );
			fprintf(LOG, "%d ldc %p %d \t| set %s ", SI.prgline, &ifd->KEY, SI.key, str );
			ifd->KEY = SI.key;
			wait_for_commit();
			break;
		}
		case CMD_TEXT :
		{
			printf("%s", str);
			fprintf(LOG, "%s", str);
			break;
		}
		case CMD_EXIT :
		{
			printf("%d \n", SI.prgline);
			fprintf( LOG , "%d \n", SI.prgline);
			Sem->Release( SEMAPHORE_EXIT );
			fclose( LOG );
			return;
			break;
		}
		default :
		{
			fclose( LOG );
			Exception( to_string( SI.prgline) + " default SIGINT");//raise( SIGINT);
			break;
		}
		} // end switch
		printf("\n");
		fprintf(LOG, "\n");
	}

	fclose( LOG );
	Comment( INFO, "Execute() terminated without exit/release");
}

