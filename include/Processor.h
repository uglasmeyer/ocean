/*
 * processor.h
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <data/Interface.h>
#include <data/Semaphore.h>
#include <Ocean.h>
#include <System.h>
#include <Time.h>

enum  {
	CMD_EXIT		,
	CMD_CHADDR 		,
	CMD_UIADDR 		,
	CMD_BOADDR		,
	CMD_KEY 		,
	CMD_EXE			,
	CMD_WAIT		,
	CMD_STR			,
	CMD_COND_WAIT 	,
	CMD_TEXT
};

class Processor_class :
		virtual Logfacility_class,
		public state_struct
{
	Interface_class* 	Sds ;
	interface_t* 		sds ;
	Semaphore_class*	Sem ;
	Appstate_class*		Appstate;
	Time_class			Timer{};
	Table_class 		Table{ defaultopt };

public:
	string				className = "";

	Processor_class( Interface_class* gui, Appstate_class* appstate ) :
		Logfacility_class("Processor"),
		state_struct()
	{
		className	= Logfacility_class::className;
		this->Sds 	= gui;
		this->sds	= gui->addr;
		this->Sem 	= gui->Sem_p;
		this->Appstate = appstate;
		process_stack.clear();

		Table.opt.Ident = 0;
		Table.opt.Crlf = '\n';
		Table.AddColumn( "line nr", 4);
		Table.AddColumn( "cmd", 4);
		Table.AddColumn( "addr", 16);
		Table.AddColumn( "value", 6);
		Table.AddColumn( "comment", 40 );
	};

	virtual ~Processor_class(){	} ;

	void Push_str( uint8_t, uint8_t, string);
	void Push_cmd( uint8_t, string);
	void Push_key( uint8_t, string );
	void Push_ifd( uint8_t*, uint8_t, string );
	void Push_ifd( bool*, bool, string );
	void Push_ifd( int*, int, string );
	void Push_wait( uint8_t, int, string );
	void Push_text( string );
	void Execute();
	void Clear_process_stack();
	void Set_prgline( int );

	void Test_Processor();


private:

	typedef struct stack_struct
	{
		int		prgline		= 0;
		int			cmd 	= 0;
		string		cmdstr	= "";
		int 		key    	= 0;
		bool*		boaddr	= nullptr;
		uint8_t*	chaddr	= nullptr;
		int*	 	uiaddr 	= nullptr;
		char*		staddr	= nullptr;
		int			value  	= 0;
		string		str		{};
	} stack_struct_t;
	const stack_struct_t default_stack 	= stack_struct();

	typedef struct print_struct
	{
		stack_struct_t 		ps			{};
		string 				addr_str 	= "";
		String 				Str 		{};
		int					value		= 0;

		print_struct( stack_struct_t _ps )
		{
			ps = _ps;
			assign_str();
			assign_val();
		}
		void assign_val()
		{
			value = ( ps.cmd == CMD_KEY ) ? ps.key : ps.value;

		}
		void assign_str()
		{
			for( long addr : { (long)ps.boaddr, (long)ps.chaddr, (long)ps.uiaddr, (long)ps.staddr } )
				if( addr > 0 )
					addr_str= Str.to_hex( addr );
		}
		~print_struct() {};

	} print_struct_t;

	int prgline = 0;
	typedef vector <stack_struct_t>
						stack_struct_vec_t;

	stack_struct_vec_t 	process_stack	{};
	stack_struct_t 		stack_item 		= stack_struct();
	int 				commit_time 	= 0;

	int 				wait_for_commit();
	void 				execute_str( stack_struct_t& ps);
	void 				printLn( stack_struct ps );
};

#endif /* PROCESSOR_H_ */
