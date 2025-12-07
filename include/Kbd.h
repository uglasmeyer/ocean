/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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
 * keyboard.cpp
 *
 *  Created on: Dec 23, 2023
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef KBD_H
#define KBD_H

#include <Ocean.h>
#define NOKEY 0

enum  KEYCODE {
	ESC		= 0x01b,//27,
	RET		= 10,
	BACK	= 127,
	DOWN	= 4348699,
	UP		= 4283163,
	LEFT	= 4479771,
	RIGHT	= 4414235,
	DEL		= 2117294875,
	F1 = 0x00000504f1b,
	F2 = 0x00000514f1b,
	F3 = 0x00000524f1b,
	F4 = 0x00000534f1b,
	F5 = 0x07e35315b1b,
	F6 = 0x07e37315b1b,
	F7 = 0x07e38315b1b,
	F8 = 0x07e39315b1b,
	F9 = 0x07e30325b1b
};

template< typename T >
constexpr bool isTTY ( const T io )
{
    if (isatty(fileno( io )))
    	return true; // "stdout is tty"
    else
       return false; 	//"stdout is not tty");
};
const bool is_atty	= isTTY( stdout );
/**************************************************
https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b
 *************************************************/
inline void	ClearScreen()
{
	if ( is_atty )
		std::cout.flush() << "\x1B[2J\x1B[H";
}
inline void CursorHome()
{
	if ( is_atty )
		std::cout.flush() << "\x1B[H" ;
}
inline void CursorHomeCol()
{
	if ( is_atty )
	{
		std::cout.flush() << "\x1B[0G" ;
	}
}


/**************************************************
 * keymap_struct
 *************************************************/
struct keymap_struct
{
	typedef struct keymap_data_struct
	{
		kbdInt_t 	id;
		string		name;
		string		menu;
		kbdInt_t	qt;
	} keymap_data_t;

	const vector<keymap_data_t>	keymap_vec
	{
			{ KEYCODE::F1, 	"F1", "Key overview"	, 0x01000030 },
			{ KEYCODE::F2, 	"F2", "Processes"		, 0x01000031 },
			{ KEYCODE::F3, 	"F3", "States"			, 0x01000032 },
			{ KEYCODE::F4, 	"F4", "Que&Waveform"	, 0x01000033 },
			{ KEYCODE::F5, 	"F5", "Instrument"		, 0x01000034 },
			{ KEYCODE::F6, 	"F6", "ADSR"			, 0x01000035 },
			{ KEYCODE::F7, 	"F7", "Spectrum"		, 0x01000036 },
			{ KEYCODE::F8, 	"F8", "Semaphores"		, 0x01000037 },
			{ KEYCODE::F9, 	"F9", "Manage Proc."	, 0x01000038 }
	};
				keymap_struct	(){};
				~keymap_struct	() = default;
	string 		Name			( kbdInt_t key );
	kbdInt_t 	Qt_key			( kbdInt_t key );
	string 		Menu			( kbdInt_t key );
};

/**************************************************
 * Kbd_base
 *************************************************/
class Kbd_base
{
public:             // Access specifier

	typedef struct key_union_struct
	{
		union
		{
			kbdInt_t			Int;
			array<uint8_t,8> 	Arr;
		};
	} key_union_t;
	typedef struct key_struct :
		key_union_struct
	{
		bool			hold	= false;
		bool			nokey	= false;
		key_struct()
		{
			Int = 0;
		}
		key_struct( uint8_t a, uint8_t b, uint8_t c )
		{
			Int		= 0; // init long
			Arr[0] 	= a; // init short
			Arr[1] 	= b;
			Arr[2] 	= c;
			Arr[3] 	= 0;
		};
		virtual ~key_struct() = default;

	} kbdkey_t;

					Kbd_base	();
	virtual 		~Kbd_base	();

	void 			Reset		();
	void			Test		();
	void 			Init		();
	string 			GetString	( string txt );
	kbdInt_t	 	GetKeyInt	( bool debug = false );
	kbdInt_t 		GetKeyInt	( int waitms );

	string			ShowKey		( kbdkey_t key );
	string 			ShowKey		( kbdInt_t Int );
private:


	struct termios 	old_flags 	= {0};
	struct termios 	new_flags 	= {0};
	key_union_struct buf8		{};
	long int*		buf8_p		= &buf8.Int;
	kbdkey_t		key8		= key_struct();
};

typedef Kbd_base::key_struct
					key3struct_t;



#endif /* KEYBOARD_H_ */
