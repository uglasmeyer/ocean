/*
 * processor.h
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <App.h>
#include <System.h>
#include <Time.h>

enum CMD_e
{
	CMD_EXIT		,
	CMD_CHADDR 		,
	CMD_KEY 		,
	CMD_EXE			,
	CMD_WAIT		,
	CMD_STR			,
	CMD_COND_WAIT 	,
	CMD_TEXT		,
	CMD_NULL
};

typedef uint8_t			sds_value_t;

template<class T>
concept AddressConcept = requires( T a )
{
	sizeof(a) == sizeof( sds_value_t );
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
		int				prgline	= 0;
		CMD_e			cmd 	= CMD_NULL;
		string			cmdstr	= "";
		EVENTKEY_e		key    	= NULLKEY;
		sds_value_t*	chaddr	= nullptr;
		sds_value_t		value	= 0;
		string			str		{};

		stack_struct(){};

		template<AddressConcept T>
		stack_struct( T* addr, T _value, string _str )
		{
			chaddr 	= reinterpret_cast<sds_value_t*> (addr);
			value	= static_cast<sds_value_t> 	(_value);
			str		= _str;
		};
		~stack_struct() = default;
	} ;

	Appstate_class*		Appstate;
	Application_class*	App;
	string				className = "";

public:
	fstream 			LOG ;

						Processor_class(	Application_class* app );
	virtual 			~Processor_class(){	DESTRUCTOR( className ) } ;

	void 				Push_str( EVENTKEY_e, uint8_t, string);
	void 				Push_cmd( CMD_e cmd, string);
	void 				Push_key( EVENTKEY_e, string );

	template<AddressConcept T >
	void 				Push_ifd( T* addr, T value, string str )
	{
		stack_struct stack_item = stack_struct( addr, value, str );
		push_ifd( stack_item );
	}
	void 				Push_wait( CMD_e cmd, int, string );
	void 				Push_text( string );
	void 				Execute();
	void 				Clear_process_stack();
	void 				Set_prgline( int );

	void 				Test_Processor();


private:

	typedef struct 		print_struct
	{
		Table_class 	Table{ defaultopt };
		stack_struct 	ps			{};
		string 			addr_str 	= "";
		String 			Str 		{};
		int				value		= 0;
		int 			commit_time = 0;

						print_struct( stack_struct _ps );
		virtual 		~print_struct() {};
		void 			assign_val();
		void 			assign_addr_str();
		void 			Line( stack_struct ps );

	} print_struct_t;
	print_struct		print	{ stack_struct() };

	int 				prgline = 0;

	vector<stack_struct>process_stack	{};
	stack_struct 		stack_item 		= stack_struct();

	int 				wait_for_commit();
	void 				execute_str( stack_struct& ps);
	void 				push_ifd( stack_struct );
};

#endif /* PROCESSOR_H_ */
