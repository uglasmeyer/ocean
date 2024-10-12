/*
 * kbd.cpp
 *
 *  Created on: Jan 6, 2024
 *      Author: sirius
 */

#include <Kbd.h>

Keyboard_base::Keyboard_base() : Logfacility_class("Kbd")
{
	init();
};

Keyboard_base::~Keyboard_base()
{
	Reset();
};


Keyboard_base::key_struct_t Keyboard_base::GetKey()
{
	pressKey();

	return keystruct;
}


void Keyboard_base::init()
{
	fflush(stdout);

	if( tcgetattr (0, &old_flags) < 0 )
		perror("tcsetattr()");
	if( tcgetattr (0, &new_flags) < 0 )
		perror("tcsetattr()");

	new_flags.c_lflag 		&= ~ICANON;	// non-standard mode
	new_flags.c_lflag 		&= ~ECHO;	// disable echo and erase

	new_flags.c_cc[VMIN] 		= 0;//1;
	new_flags.c_cc[VTIME] 	= 1;//0; //0->HIGH cpu load
	// In this case TIME is a total read timeout. (timeout in multiples of 0.1 sec)
	// The read returns if either MIN bytes are received before the timer expires
	// or if the timer expires. In the latter case the number of bytes received
	// is returned even if that number is zero.
	// If an interrupt occurs before the timer expires and no bytes are read
	// the read returns -1 with errno set to EINTR.
	if(tcsetattr(0, TCSANOW, &new_flags) < 0)
		perror("tcsetattr ICANON");

}

void Keyboard_base::Reset()
{
	Comment(INFO , "Keyboard reset");
	/*
	struct termios 		tflags = {0};
	if(tcgetattr (0, &tflags) < 0)
		perror("tcsetattr()");

	tflags.c_lflag 		|= ICANON; 	// standard mode
	tflags.c_lflag 		|= ECHO;	// enable echo and erase

	fflush(stdout);

	if( tcsetattr(0, TCSADRAIN, &tflags ) < 0)
		perror("tcsetattr ~ICANON");
	*/
	if( tcsetattr(0, TCSADRAIN, &old_flags ) < 0)
		perror("tcsetattr ~ICANON");
}

char Keyboard_base::getkey()
{

	fflush(stdout);
	buf = 0;
	if(read(0, buf_p, 1) < 0)
		perror("read()");
	return *buf_p;
}

void Keyboard_base::Set_ch( char ch )
{
	buf = ch;
}
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

void Keyboard_base::pressKey()
{
	keystruct.key = getkey();
//	keystruct.key = getch();

	if ( keystruct.key == 27 )
	{
		c2 = getkey();
		c3 = getkey();
		keystruct.val = (c2) + (c3<<1) ;
	}
	else
		keystruct.val = 0;
}

void Keyboard_base::Test()
{
	Set_Loglevel( TEST, true );
	Comment( TEST, "Keyboard test running");
	do
	{
		cout << GetKey().key ;
		Comment( TEST, " > Press # to finish keyboard test");

	}
	while( not ( GetKey().key == '#' ) );

	Comment( TEST, "Keyboard test finished");
//	assert( false );

}


