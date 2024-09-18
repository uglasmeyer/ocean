/*
 * keyboard.cpp
 *
 *  Created on: Dec 23, 2023
 *      Author: sirius
 */

#ifndef KBD_H
#define KBD_H

#include <Synthesizer.h>
#include <Logfacility.h>

using namespace std;



class Keyboard_base : virtual Logfacility_class
{
public:             // Access specifier
	const static int	NoKey 	= 0;

	typedef struct key_struct
	{
		int 	key	= NoKey;
		uint 	val	= 0;
	} key_struct_t;
	key_struct_t 	keystruct;

	array<key_struct_t, 6> key_vector {key_struct()};
	array<key_struct_t, 6> default_key_vector = key_vector;

	Keyboard_base();
	virtual ~Keyboard_base();

	key_struct_t 	GetKey();
	void 			Set_ch( char ch );
	key_struct_t 	GetHold();
	void			KeyVector();
	void 			Reset();
	void			Test();

private:

	uint8_t 		c2			= 0;
	uint8_t 		c3 			= 0;
	struct termios 	old_flags 	= {0};
	struct termios 	new_flags 	= {0};
	char 			buf 		= 0;
	char*			buf_p		= &buf;

	char getch();
	char getkey();
	void pressKey();
	void show_key_vector();
	void init();

};



#endif /* KEYBOARD_H_ */
