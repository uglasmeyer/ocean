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
 * processor.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <App.h>
#
#include <limits>

enum CMD_e
{
	CMD_EXIT		,
	CMD_CHADDR 		,
	CMD_KEY 		,
	CMD_SYS			,
	CMD_WAIT		,
	CMD_STR			,
	CMD_COND_WAIT 	,
	CMD_TEXT		,
	CMD_POS			,
	CMD_JMP			,
	CMD_ADD			,
	CMD_LDX			,
	CMD_OUT			,
	CMD_MOV			,
	CMD_STASTATE	,
	CMD_NULL
};

enum WAIT_e : int
{
	DEBUGON,
	DEBUGOFF,
	AUTO,
	KEY,
	DYNVOL,
	DYNFRQ
};

typedef uint8_t			sds_value_t;

/**************************************************
 * pos_stack_struct
 * controls a vector of integers
 *************************************************/
typedef struct pos_stack_struct
{
private:
	vector<int> vec {};

public:
	void Push( int val )
	{
		vec.push_back( val );
	}
	int Pop()
	{
		if( vec.size() == 0 )
			Exception("Empty position stack...");
		int val = vec.back();
		vec.pop_back();
		return val;
	}
	void Clear()
	{
		vec.clear();
	}
	int Return()
	{
		return vec.back();
	}
} pos_stack_t;


/**************************************************
 * Assembler_class
 *************************************************/
typedef int reg_t;
typedef sds_value_t regid_t;
const regid_t			REG_SIZE		= 8;

class Assembler_class :
	virtual public Logfacility_class,
	public pos_stack_struct
{
	array<reg_t, REG_SIZE>	register_arr	{};
public:
	regid_t					Regid			;
	const regid_t			REGUNDEF 		= numeric_limits<regid_t>::max();
	const range_T<regid_t>	regid_range		{ 0, REG_SIZE };
				Assembler_class();
	virtual 	~Assembler_class();

	void		Ldx( regid_t id, reg_t value );
	void		Add( regid_t id, reg_t value );
	bool		Cmp( regid_t id );
	void		Cpy( regid_t id, uint8_t* addr );
	int			Out( regid_t id );
private:
};
/**************************************************
 * Processor_class
 * Provides all functionality to execute the process stack
 * and manage the composer log file
 *************************************************/
template<class T>
concept AddressConcept = requires( T a )
{
	sizeof(a) == sizeof( sds_value_t );
};

class Processor_class :
	public virtual 		Logfacility_class,
	virtual				Interface_base
{
	Semaphore_class*	Sem ;
	Time_class			Timer{};


	struct stack_struct
	{
		int				prgline		= 0;
		CMD_e			cmd 		= CMD_NULL;
		string			cmdstr		= "";
		EVENTKEY_e		key    		= NULLKEY;
		sds_value_t*	chaddr		= nullptr;
		sds_value_t		sds_value	= 0;
		int				int_value	= 0;
		string			str			{};

		stack_struct(){};

		template<AddressConcept T>
		stack_struct( T* addr, T _value, string _str )
		{
			chaddr 		= reinterpret_cast<sds_value_t*> (addr);
			sds_value	= static_cast<sds_value_t> 	(_value);
			str			= _str;
		};
		~stack_struct() = default;
	} ;

	bool				Autostart;
	Appstate_class*		Appstate;
	Application_class*	App;
	Assembler_class		Assembler;

public:
	bool				Debug_Processing	;
	int					txtline 			= 0;

						Processor_class(	Application_class* app );
	virtual 			~Processor_class(){	DESTRUCTOR( className ) } ;

	void 				Push_str( CSTRKEY_e, string);
	void 				Push_sys( CMD_e cmd, string);
	void 				Push_key( EVENTKEY_e, string );

	template<AddressConcept T >
	void 				Push_ifd( T* addr, T value, string str )
	{
		stack_struct stack_item = stack_struct( addr, value, str );
		push_ifd( stack_item );
	}
	void 				Push_wait			( CMD_e cmd, int, string );
	void 				Push_text			( string );
	void 				Assembler_cmd		( CMD_e cmd, regid_t id, int value, string str );
	void 				Execute				();
	void 				Clear_process_stack	();
	void 				Set_prgline			( int no, int id );
	bool				Set_regid			( regid_t id );
	void 				Test_Processor		();

private:


	typedef struct 		print_struct
	{
		Table_class* 	Table		;

		stack_struct 	ps			{};
		string 			addr_str 	= "";
		int				value		= 0;
		int 			commit_time = 0;

						print_struct( stack_struct _ps );
		virtual 		~print_struct() {};
		void 			assign_val();
		void 			assign_addr_str( long value );

		void 			Line( stack_struct ps, interface_t* sds );

	} print_struct_t;
	print_struct		print	{ stack_struct() };

	int 				prgline = 0;

	vector<stack_struct>process_stack	{};
	stack_struct 		stack_item 		= stack_struct();

	int 				wait_for_commit();
	void				execute_cmd( EVENTKEY_e key );
	void				show_sta_state( string staid_str );
	void 				push_ifd( stack_struct );
};

#endif /* PROCESSOR_H_ */
