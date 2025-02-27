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

void Processor_class::Push_ifd( int* uiaddr, int value, string str )
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

	Sds->Reset_ifd( );
	Sds->Commit();

	FILE* LOG;
	LOG = fopen( file_structure().log_file.data(), "w");

	Sem->Init();

	for ( stack_struct_t ps : process_stack )
	{
		Sds->addr->Composer = RUNNING;
		const char* str = ps.str.data();

		switch ( ps.cmd )
		{
		case CMD_CHADDR : // write char address
		{
			const char* fmt = "%d ldc %p %d \t| %s";
			printf(		fmt, ps.prgline, ps.chaddr, ps.value, str );
			fprintf(LOG,fmt, ps.prgline, ps.chaddr, ps.value, str );
			if ( not ps.chaddr == 0 )
				*ps.chaddr = (char) ps.value ;
			break;
		}
		case CMD_BOADDR : // write char address
		{
			const char* fmt = "%d ldc %p %d \t| %s";
			printf( 	fmt, ps.prgline, ps.boaddr, ps.value, str );
			fprintf(LOG,fmt, ps.prgline, ps.boaddr, ps.value, str );
			if ( not ps.boaddr == 0 )
				*ps.boaddr = (bool) ps.value ;
			break;
		}
		case CMD_UIADDR : // write uint16_t address
		{
			const char* fmt = "%d ldu %p %d \t| %s";
			printf( 	fmt , ps.prgline, ps.uiaddr, ps.value, str );
			fprintf(LOG,fmt , ps.prgline, ps.uiaddr, ps.value, str );
			if ( not ps.uiaddr == 0 )
				*ps.uiaddr = (uint16_t) ps.value;
			break;
		}
		case CMD_KEY : // write command key value
		{
			const char* fmt = "%d ldc %p %d \t| %s";
			printf( 	fmt, ps.prgline,  &sds->EVENT, ps.key, str );
			fprintf(LOG,fmt, ps.prgline,  &sds->EVENT, ps.key, str );
			Sds->Event( ps.key );
			wait_for_commit();
			break;
		}
		case CMD_EXE :
		{
			const char* fmt = "%d exe %s ";
			printf(		fmt, ps.prgline, str );
			fprintf(LOG,fmt, ps.prgline, str );
			system_execute( ps.str );
			break;
		}
		case CMD_WAIT :
		{
			if  ( ps.value < 0 )
			{
				printf( 	"%d %s", ps.prgline, str );
				fprintf(LOG,"%d %s", ps.prgline, str );
				getc(stdin);
			}
			else
			{
				printf(		"%d wait %d", ps.prgline, ps.value );
				fprintf(LOG,"%d wait %d", ps.prgline, ps.value );
			    this_thread::sleep_for(chrono::seconds( ps.value));
			}
			break;
		}
		case CMD_COND_WAIT :
		{
			uint8_t sec = sds->Noteline_sec;
			const char* fmt = "%d conditional wait %d sec\t| %s (%d)";
			printf( 	fmt, ps.prgline,  sec, str, sec );
			fprintf(LOG,fmt, ps.prgline,  sec, str, sec );
		    this_thread::sleep_for(chrono::seconds(sec));
			sds->Noteline_sec = 0;
			break;
		}
		case CMD_STR : // write string
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

			const char* fmt1 ="%d ldc %p %s \n";
			printf( 	fmt1, ps.prgline, addr, str );
			fprintf(LOG,fmt1, ps.prgline, addr, str );
			this->Sds->Write_str( ps.value, ps.str );
			if ( ps.value == NOTESSTR_KEY )
				Sds->addr->NotestypeId = 1;

			const char* fmt2 = "%d ldc %p %d \t| set %s ";
			printf( 	fmt2, ps.prgline, &sds->EVENT, ps.key, str );
			fprintf(LOG,fmt2, ps.prgline, &sds->EVENT, ps.key, str );
			Sds->Event( ps.key );
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
			printf("%d \n", ps.prgline);
			fprintf( LOG , "%d \n", ps.prgline);
			Sem->Release( SEMAPHORE_EXIT );
			fclose( LOG );
			return;
			break;
		}
		default :
		{
			fclose( LOG );
			EXCEPTION( to_string( ps.prgline) + " default SIGINT");//raise( SIGINT);
			break;
		}
		} // end switch
		printf("\n");
		fprintf(LOG, "\n");
	}

	fclose( LOG );
	Comment( INFO, "Execute() terminated without exit/release");
	Comment( INFO, "Read composer log " + file_structure().log_file );
	Comment( INFO, "for more information" );
}

