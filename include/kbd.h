/*
 * keyboard.cpp
 *
 *  Created on: Dec 23, 2023
 *      Author: sirius
 */

#ifndef KBD_H
#define KBD_H

#include <synthesizer.h>

using namespace std;


typedef struct key_struct
{
	char 	key	= 0;
	uint_t 	val	= 0;
} key_struct_t;

class Kbd_class : virtual Logfacility_class
{
public:             // Access specifier
	Kbd_class() : Logfacility_class("Kbd")
	{
		c2 = 0;
		c3 = 0;
	}
	virtual ~Kbd_class(){

	}
	key_struct_t GetKey();
	void Reset();
	bool NextKey();

private:

	key_struct_t 	keystruct;
	char 			NoKey 		= '$';
	char 			MinorKey	= NoKey;
	char 			ExitKey 	= '#';
	bool 			Status	 	= true;
	int 			m_counter 	= 0;
	uint8_t 		c2,c3,c4 	= 0;

	char getch(void);
	void PressKey();

};



#endif /* KEYBOARD_H_ */
