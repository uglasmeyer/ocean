//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * kbd.cpp
 *
 *  Created on: Jan 6, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Kbd.h>

/**************************************************
 * keymap_struct
 *************************************************/
string keymap_struct::Name( kbdInt_t key )
{
	for( keymap_data_t map : keymap_vec )
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
kbdInt_t keymap_struct::Qt_key( kbdInt_t key )
{
	for( keymap_data_t map : keymap_vec )
	{
		if ( map.qt == key )
			return map.id;
	}
	return key;
}
string keymap_struct::Menu( kbdInt_t key )
{
	for( keymap_data_t map : keymap_vec )
	{
		if ( map.id == key )
			return map.menu;
	}
	return "";
};

/**************************************************
 * Kbd_base
 *************************************************/
Kbd_base::Kbd_base() :
	Logfacility_class("Kbd_base")
{
	Init();
};

Kbd_base::~Kbd_base()
{
	Reset();
};

string Kbd_base::ShowKey( kbdInt_t Int )
{
	kbdkey_t key{};
	key.Int = Int;
	return ShowKey( key );
}

string Kbd_base::ShowKey( kbdkey_t key )
{
	if ( key.Int == 0 )
		return "";
	stringstream strs {};

	strs << hex << "0x0" <<
			key.Int << " : ";// <<
//			map_key_name( key.Int ) << " ";
	for( uint n = 0; n < key.Arr.size(); n++ )
	{
		strs << (int)key.Arr[n] << " ";
	}

	cout << strs.str() << endl;
	return strs.str();
}

kbdInt_t Kbd_base::GetKeyInt	( bool debug )
{
	fflush(stdout);
	buf8.Int 			= 0x0;//Nullkey3;//{ 0,0,0 };

	if(read(0, buf8_p, 8) < 0)
		perror("GetKeystruct()");

	key8.nokey 		= ( buf8.Int == 0 );
	if ( key8.nokey )
		key8.hold	= false;
	else
		key8.hold 	= ( buf8.Int == key8.Int );

	key8.Int = buf8.Int;

	if ( debug )
		ShowKey( key8 );
	return key8.Int;
}

kbdInt_t Kbd_base::GetKeyInt( int waitms )
{
	key3struct_t key { };
	while( key.Int == 0 )
	{
		key.Int = GetKeyInt( false );
		this_thread::sleep_for( std::chrono::milliseconds( waitms ) );
	}
	return key.Int;

}
void Kbd_base::Init()
{
	fflush(stdout);

	if( tcgetattr (0, &old_flags) < 0 )
		perror("tcsetattr()");
	if( tcgetattr (0, &new_flags) < 0 )
		perror("tcsetattr()");

	new_flags.c_lflag 		&= ~ICANON;	// non-standard mode
	new_flags.c_lflag 		&= ~ECHO;	// disable echo and erase

	new_flags.c_cc[VMIN] 	= 0;//1;
	new_flags.c_cc[VTIME] 	= 0;//1;//0; //0->HIGH cpu load // set break with micro timer???
	// In this case TIME is a total read timeout. (timeout in multiples of 0.1 sec)
	// The read returns if either MIN bytes are received before the timer expires
	// or if the timer expires. In the latter case the number of bytes received
	// is returned even if that number is zero.
	// If an interrupt occurs before the timer expires and no bytes are read
	// the read returns -1 with errno set to EINTR.
	if(tcsetattr(0, TCSANOW, &new_flags) < 0)
		perror("Kbd_base::Init() ICANON");

}

void Kbd_base::Reset()
{
	if ( is_atty )
	{
		if( LogMask[DEBUG] )
			cout << "Keyboard reset" << endl;
		if( tcsetattr(0, TCSADRAIN, &old_flags ) < 0)
			perror("tcsetattr ~ICANON");
	}
}

string Kbd_base::GetString( string txt )
{
	cout.flush() << txt;
	Reset();
	string s {};
	cin >> s;
	Init();
	return s;
}

void Kbd_base::Test()
{
	TEST_START( className );
	uint nr = 0;
	kbdkey_t pressed = key_struct( ESC,0,0 );
	do
	{
		Comment( TEST, " > Press <ESC> to finish keyboard test");
		nr++;
	}
	while( not ( pressed.Arr[0] == ESC ) and ( nr < 10 ));

	Comment( TEST, "Keyboard test finished");
	ASSERTION( pressed.Int == 27L, "Keyboard test ", (long int) pressed.Int, ESC );
	TEST_END( className );

}


/* prototype
char Keyboard_base::getch()
//https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux
//https://www.mkssoftware.com/docs/man5/struct_termios.5.asp
{
	char 				buf = 0;
	struct termios 		old = {0};

	fflush(stdout);

	if(tcgetattr(0, &old) < 0)
		perror("tcsetattr()");

	old.c_lflag 		&= ~ICANON;	// non-standard mode
	old.c_lflag 		&= ~ECHO;	// disable echo and erase

	old.c_cc[VMIN] 		= 0;//1;
	old.c_cc[VTIME] 	= 1;//0; //0->HIGH cpu load
	// In this case TIME is a total read timeout. (timeout in multiples of 0.1 sec)
	// The read returns if either MIN bytes are received before the timer expires
	// or if the timer expires. In the latter case the number of bytes received
	// is returned even if that number is zero.
	// If an interrupt occurs before the timer expires and no bytes are read
	// the read returns -1 with errno set to EINTR.


	if(tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");

	if(read(0, &buf, 1) < 0)
		perror("read()");

	old.c_lflag 		|= ICANON; 	// standard mode
	old.c_lflag 		|= ECHO;	// enable echo and erase

	if(tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");

	return buf;
};
*/
