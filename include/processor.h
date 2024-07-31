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

#define CMD_EXIT		0
#define CMD_CHADDR 		1
#define CMD_UIADDR 		2
#define CMD_KEY 		3
#define CMD_EXE			4
#define CMD_WAIT		5
#define CMD_STR			6
#define CMD_COND_WAIT 	7
#define CMD_TEXT		8

class Processor_class : public virtual Logfacility_class
{

public:

	GUI_interface_class* 	GUI ;
	ifd_t* 					ifd ;

	void push_str( uint8_t, char, string);
	void push_cmd( uint8_t, string);
	void push_key( uint8_t, string );
	void push_ifd( char*, char, string );
	void push_ifd( uint16_t*, uint16_t, string );
	void push_wait( uint8_t, int, string );
	void push_text( string );
	void execute();
	void clear_process_stack();
	void set_prgline( int );

	Processor_class( GUI_interface_class* gui ) : Logfacility_class("Processor")
	{
		this->GUI = gui;
		this->ifd = gui->addr;
		process_stack.clear();
	};

	virtual ~Processor_class(){	} ;

private:
	typedef struct stack_struct
	{
		int		prgline		= 0;
		int			cmd 	= 0;
		int 		key    	= 0;
		char* 		chaddr	= nullptr;
		uint16_t* 	uiaddr 	= nullptr;
		int			value  	= 0;
		string		str		{};
	} stack_struct_t;

	int prgline = 0;
	typedef vector <stack_struct_t>
						stack_struct_vec;
	stack_struct_vec 	process_stack{};
	stack_struct_t 		stack_item;

	void wait_for_commit();

};

#endif /* PROCESSOR_H_ */
