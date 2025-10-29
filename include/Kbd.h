/*
 * keyboard.cpp
 *
 *  Created on: Dec 23, 2023
 *      Author: sirius
 */

#ifndef KBD_H
#define KBD_H

#include <Logfacility.h>

#define NOKEY 0

enum  KEYCODE {
	ESC= 0x01b,//27,
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

typedef struct keymap_struct
{
	kbdInt_t 	id;
	string		name;
	int 		qt;

} keymap_t;
const vector<keymap_t>	keymap_vec =
{
		{ KEYCODE::F1, 	"F1", 0x01000030 },
		{ KEYCODE::F2, 	"F2", 0x01000031 },
		{ KEYCODE::F3, 	"F3", 0x01000032 },
		{ KEYCODE::F4, 	"F4", 0x01000033 },
		{ KEYCODE::F5, 	"F5", 0x01000034 },
		{ KEYCODE::F6, 	"F6", 0x01000035 },
		{ KEYCODE::F7, 	"F7", 0x01000036 },
		{ KEYCODE::F8, 	"F8", 0x01000037 },
		{ KEYCODE::F9, 	"F9", 0x01000038 }
};
constexpr string map_key_name( kbdInt_t key )
{
	for( keymap_t map : keymap_vec )
	{
		if ( map.id == key )
			return map.name;
	}
	if (key < 0xF0 ) //(<128), 127
	{
		string str {};
		str.assign(1, (char)key );
		return str;
	}
	return to_string(key);
};

constexpr kbdInt_t map_qt_key( kbdInt_t key )
{
	for( keymap_t map : keymap_vec )
	{
		if ( map.qt == key )
			return map.id;
	}
	return key;
};


class Kbd_base
	: virtual 	Logfacility_class
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
