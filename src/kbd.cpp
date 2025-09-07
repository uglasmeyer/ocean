/*
 * kbd.cpp
 *
 *  Created on: Jan 6, 2024
 *      Author: sirius
 */

#include <Kbd.h>

Kbd_base::Kbd_base() :
	Logfacility_class("Kbd_base")
{
	Init();
};

Kbd_base::~Kbd_base()
{
	Reset();
};

string Kbd_base::ShowKey( key3struct_t key )
{
	if ( key.key == 0 )
		return "";
	stringstream strs {};
	strs << dec <<
			(int) key.key <<":" <<
			(int) key.val0 << ":" <<
			(int) key.val1 << " ";
	cout << strs.str() << endl;
	return strs.str();
}

Kbd_base::key3struct_t Kbd_base::GetKeystruct	( bool debug )
{
	fflush(stdout);
	buf3 			= Nullkey3;//{ 0,0,0 };

	if(read(0, buf3_p, 3) < 0)
		perror("read()");

	key3.nokey 		= ( buf3.key == 0 );
	if ( key3.nokey )
		key3.hold	= false;
	else
		key3.hold 	= ( ( buf3.key == buf3.val0 ) or ( buf3.key == key3.val1 ));

	key3.set( buf3);

	if ( debug )
		ShowKey( key3 );
	return key3;
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
		perror("tcsetattr ICANON");

}

void Kbd_base::Reset()
{
	if ( is_atty )
	{
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
	key3struct_t pressed = key3_struct( 0,0,0 );
	do
	{
		pressed = GetKeystruct( true ) ;
		Comment( TEST, " > Press <ESC> to finish keyboard test");
		nr++;
	}
	while( not ( pressed.key == ESC ) and ( nr < 10 ));

	Comment( TEST, "Keyboard test finished");
//	assert( false );
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
