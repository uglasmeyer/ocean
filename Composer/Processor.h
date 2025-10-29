/*
 * processor.h
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <App.h>
//#include <data/Interface.h>
//#include <data/Semaphore.h>
//#include <data/Appstate.h>
//#include <Ocean.h>
#include <System.h>
#include <Time.h>

enum  {
	CMD_EXIT		,
	CMD_CHADDR 		,
	CMD_KEY 		,
	CMD_EXE			,
	CMD_WAIT		,
	CMD_STR			,
	CMD_COND_WAIT 	,
	CMD_TEXT
};

class Processor_class :
	public virtual 		Logfacility_class,
	public 				sdsstate_struct
{
	Interface_class* 	Sds ;
	interface_t* 		sds ;
	Semaphore_class*	Sem ;
	Time_class			Timer{};

	struct stack_struct
	{
		int		prgline		= 0;
		int			cmd 	= 0;
		string		cmdstr	= "";
		EVENTKEY_t	key    	= NULLKEY;
		char*		chaddr	= nullptr;
		int			value  	= 0;
		string		str		{};

		stack_struct(){};
		stack_struct( char* addr, char _value, string _str ) :
			chaddr( addr ), value( _value), str(_str) {};
		stack_struct( bool* addr, bool _value, string _str ) :
			chaddr( (char*)addr ), value( _value), str(_str) {};
		stack_struct( uint8_t* addr, uint8_t _value, string _str ) :
			chaddr( (char*)addr ), value( _value), str(_str) {};
		stack_struct( OscId_t* addr, OscId_t _value, string _str ) :
			chaddr( (char*)addr ), value( _value), str(_str) {};
		stack_struct( StateId_t* addr, StateId_t _value, string _str ) :
			chaddr( (char*)addr ), value( _value), str(_str) {};

		~stack_struct() = default;
	} ;
	typedef stack_struct stack_struct_t;

public:
	Appstate_class*		Appstate;
	string				className = "";
	Table_class 		Table{ defaultopt };
	fstream 			LOG ;

						Processor_class(	Application_class* app );
	virtual 			~Processor_class(){	DESTRUCTOR( className ) } ;

	void 				Push_str( EVENTKEY_t, uint8_t, string);
	void 				Push_cmd( uint8_t, string);
	void 				Push_key( EVENTKEY_t, string );

	void 				Push_ifd( uint8_t*, uint8_t, string );
	void 				Push_ifd( bool*, bool, string );
	void 				Push_ifd( char*, char, string  );
	void 				Push_ifd( StateId_t*, StateId_t, string  );
	void 				Push_ifd( OscId_t*, OscId_t, string  );

	void 				Push_wait( uint8_t, int, string );
	void 				Push_text( string );
	void 				Execute();
	void 				Clear_process_stack();
	void 				Set_prgline( int );

	void 				Test_Processor();


private:

	typedef struct 		print_struct
	{
		stack_struct_t 	ps			{};
		string 			addr_str 	= "";
		String 			Str 		{};
		int				value		= 0;

						print_struct( stack_struct_t _ps );
		virtual 		~print_struct() {};
		void 			assign_val();
		void 			assign_addr_str();

	} print_struct_t;

	int 				prgline = 0;
	typedef vector<stack_struct_t>
						stack_struct_vec_t;

	stack_struct_vec_t 	process_stack	{};
	stack_struct_t 		stack_item 		= stack_struct();
	int 				commit_time 	= 0;

	int 				wait_for_commit();
	void 				execute_str( stack_struct_t& ps);
	void 				printLn( stack_struct ps );
	void 				push_ifd( stack_struct_t );
};

#endif /* PROCESSOR_H_ */
