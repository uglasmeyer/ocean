/*
 * kbd.cpp
 *
 *  Created on: Jan 6, 2024
 *      Author: sirius
 */

#include <kbd.h>


Keyboard_base::key_struct_t Keyboard_base::GetKey()
{
	pressKey();
	return keystruct;
}

Keyboard_base::key_struct_t Keyboard_base::GetHold()
{
	KeyVector();
	return key_vector[0];
}

void Keyboard_base::KeyVector()
{
	char ch = 0;
	uint8_t i = 0;
	do
	{
		ch = GetKey().key;
		i++;
	} while ( not( ( ch == 0 ) or ( i < key_vector.size() ) ) );
	if ( ch == 0 )
		key_vector=default_key_vector;
	else
	{
		key_vector[0].key = ch;
		for( uint8_t i = 1 ; i<key_vector.size(); i++ )
		{
			key_vector[i] = GetKey();
		}
	}
}
void Keyboard_base::show_key_vector()
{
	for( key_struct_t keystr : key_vector )
		cout << keystr.key << " " << keystr.val << endl;
}
void Keyboard_base::Reset()
{
	struct termios 		tflags = {0};
	Comment(INFO , "Keyboard reset");
	if(tcgetattr (0, &tflags) < 0)
		perror("tcsetattr()");

	tflags.c_lflag 		|= ICANON; 	// standard mode
	tflags.c_lflag 		|= ECHO;	// enable echo and erase

	fflush(stdout);

	if( tcsetattr(0, TCSADRAIN, &tflags ) < 0)
		perror("tcsetattr ~ICANON");

}

char Keyboard_base::getch(void)
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
	keystruct.key = getch();

	if ( keystruct.key == 27 )
	{
		c2 = getch();
		c3 = getch();
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
		cout << GetHold().key ;
		Comment( TEST, " > Press # to finish keyboard test");

	}
	while( not ( key_vector[0].key == '#' ) );

	Comment( TEST, "Keyboard test finished");
//	assert( false );

}


