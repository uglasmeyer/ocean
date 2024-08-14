/*
 * processor.h
 *
 *  Created on: Apr 16, 2024
 *      Author: sirius
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <synthesizer.h>
#include <Interface.h>
#include <common.h>

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

class Processor_class : public virtual Logfacility_class
{
	Interface_class* 	GUI ;
	ifd_t* 					ifd ;
public:

	Processor_class( Interface_class* gui ) : Logfacility_class("Processor")
	{
		this->GUI = gui;
		this->ifd = gui->addr;
		process_stack.clear();
	};

	virtual ~Processor_class(){	} ;

	void Push_str( uint8_t, char, string);
	void Push_cmd( uint8_t, string);
	void Push_key( uint8_t, string );
	void Push_ifd( char*, char, string );
	void Push_ifd( bool*, bool, string );
	void Push_ifd( float*, float, string );
	void Push_wait( uint8_t, int, string );
	void Push_text( string );
	void Execute();
	void Clear_process_stack();
	void Set_prgline( int );

private:
	typedef struct stack_struct
	{
		int		prgline		= 0;
		int			cmd 	= 0;
		int 		key    	= 0;
		bool*		boaddr	= nullptr;
		char* 		chaddr	= nullptr;
		float*	 	uiaddr 	= nullptr;
		int			value  	= 0;
		string		str		{};
	} stack_struct_t;

	int prgline = 0;
	typedef vector <stack_struct_t>
						stack_struct_vec;
	stack_struct_vec 	process_stack{};
	stack_struct_t 		stack_item = stack_struct();

	void wait_for_commit();

};

#endif /* PROCESSOR_H_ */
