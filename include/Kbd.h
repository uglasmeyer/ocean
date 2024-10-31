/*
 * keyboard.cpp
 *
 *  Created on: Dec 23, 2023
 *      Author: sirius
 */

#ifndef KBD_H
#define KBD_H

#include <Ocean.h>
#include <Logfacility.h>

using namespace std;

static const int	NoKey 	= 0;
typedef struct key_struct
{
	int 	key	= NoKey;
	uint 	val	= 0;
} key_struct_t;


class Keyboard_base : virtual Logfacility_class
{
public:             // Access specifier
	const char			AppExit = '#';

	Keyboard_base();
	virtual ~Keyboard_base();

	key_struct_t 	keystruct;
	key_struct_t 	GetKey();
	void 			Set_ch( char ch );
	void 			Reset();
	void			Test();
	void 			Init();

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

};



#endif /* KEYBOARD_H_ */
