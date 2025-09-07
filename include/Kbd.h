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
	ESC	=	27,
	ASC	=	126,
	ALT =  -62,
	S0	=	49,
	S1	=	50,
	F1	=	80,
	F2	=	81,
	F3	=	82,
	F4	=	83,
	F5	=	53,
	F6	=	55,
	F7  =	56,
	F8  =	57,
	F9 	= 	48
};

class Kbd_base
	: virtual 	Logfacility_class
{
public:             // Access specifier
	typedef struct keybuf_struct
	{
		char			key		= NOKEY;
		char			val0	= 0;
		char			val1	= 0;
	} keybuf_t;
	typedef struct key3_struct :
		keybuf_struct
	{
		bool			hold	= false;
		bool			nokey	= false;
		friend bool operator==(const key3_struct& A, const key3_struct& B )
		{
		   	return (( A.key == B.key ) and ( A.val0 == B.val0 ));
		}
		key3_struct( char a, char b, char c ) :
			keybuf_struct{a, b, c}
		{};
		virtual ~key3_struct() = default;

		void set( keybuf_t buffer )
		{
			key 	= buffer.key;
			val0 	= buffer.val0;
			val1 	= buffer.val1;
		}
		keybuf_t get( )
		{
			return keybuf_struct	{ key,val0, val1 };
		}
		friend bool operator==( keybuf_t lhs, keybuf_t rhs )
		{
			bool 	equal 	= ( ( lhs.key  == rhs.key ) and
							( lhs.val0 == rhs.val0 ) and
							( lhs.val1 == rhs.val1 ) );
			return 	equal;
		}
	} key3struct_t;
	key3struct_t 	Nullkey3 	= key3_struct( 0, 0, 0 );
	key3struct_t	ESCkey		= key3_struct( ESC, 0, 0 );

					Kbd_base	();
	virtual 		~Kbd_base	();

	void 			Reset			();
	void			Test			();
	void 			Init			();
	string 			GetString		( string txt );
	key3struct_t 	GetKeystruct	( bool debug = false );
	string			ShowKey			( key3struct_t key );



private:

	struct termios 	old_flags 		= {0};
	struct termios 	new_flags 		= {0};
	keybuf_t		buf3			= keybuf_struct();
	keybuf_t*		buf3_p			= &buf3;
	key3struct_t	key3			= key3_struct(0,0,0);

//	char 			getch			(); //prototype


};

typedef Kbd_base::key3_struct key3struct_t;



#endif /* KEYBOARD_H_ */
