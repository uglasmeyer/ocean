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
 * lineedit.cpp
 *
 *  Created on: Nov 21, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <Composer/Lineedit.h>
#include <sys/ioctl.h>

inline winsize get_terminal_size()
{
	struct winsize w {0,0,0,0};
	if( not is_atty )
		return w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        std::cerr << "Error getting terminal size" << std::endl;
    }
    return w;
}


inline void CursorRight()
{
	if ( is_atty )
		printf("\x1b[1C" );//std::cout.flush() << "ESC[#C" ;
}
inline void CursorLeft()
{
	if ( is_atty )
		printf("\x1b[1D" );//std::cout.flush() << "ESC[#C" ;
}
inline void CursorPos( uint x, uint y )
{
	if ( is_atty )
		printf( "\x1b[%d;%dH", y , x );//std::cout.flush() << "\x1b[" << y << ";" << x << "H" ;
}
inline void EraseLine()
{
	if ( is_atty )
		printf( "\x1b[2K" );
}
auto Sout  = [  ] ( int x, int y, string str )
{
	CursorPos( x, y );
	EraseLine();
	cout << str;
};

/**************************************************
 * Lineedit_class
 *************************************************/

Lineedit_class::Lineedit_class	( string title, Kbd_base* kbd ) :
	Logfacility_class("Lineedit_class")
{
	className = Logfacility_class::className;
	this->Title	= title;
	this->Kbd	= kbd;
};
Lineedit_class::~Lineedit_class	()
{
	DESTRUCTOR( className );
};

string Lineedit_class::Line( string _str )
{
	string str = _str;
	int offs = Title.length() + 1;
	int cur = str.length() + offs;
	int pos = str.length();


	range_T<int> str_range { 0, pos -1};
	range_T<int> cur_range { offs, cur };

	auto inccursor = [ & ]()
	{
		cur_range.max = str.length()+offs + 1;
		cur = check_range( cur_range, ++cur, "" );
		pos = check_range( str_range, ++pos, "" );
	};
	auto deccursor = [ & ]()
	{
		cur = check_range( cur_range, --cur, "" );
		pos = check_range( str_range, --pos, "" );
	};
	auto inschar = [ & ]( int& pos, char ch )
	{
		str.insert( str.begin() + pos, ch );
		str_range.max = str.length();
		inccursor();
	};
	auto delchar = [ & ](  int& pos )
	{
		if ( str.length() == 0 ) return;
		if ( pos < 0 ) return;
		str.erase(str.begin() + pos );
	};
	auto delback = [ & ](  int& pos )
	{
		if( pos < 1 ) return;
		str.erase( str.begin() + pos -1 );
		deccursor();
	};

//	Sout( 1, 19, "pos " + to_string(pos) );
//	Sout( 1,20, Title + str);
	Sout( 1, get_terminal_size().ws_row, Title + str );

	kbdInt_t key = 0;
	while( key != KEYCODE::RET )
	{
		key = Kbd->GetKeyInt( delay );
		switch (key)
		{
			case KEYCODE::RIGHT	: { inccursor();  break; }
			case KEYCODE::LEFT	: { deccursor();  break; }
			case KEYCODE::BACK	: { delback( pos ); break;}
			case KEYCODE::DEL	: { delchar( pos ); break;}
			case KEYCODE::RET	: { return str; break; }
			case KEYCODE::ESC	: { return str; break; }
			default 			: { inschar( pos, char(key) ); break; }
		} // switch key
		str_range.max = str.length()-1;
		cur_range.max = str.length() + offs;
		if ( key > 0 )
		{
//			Sout( 1, 18, "key " + to_string(key));
//			Sout( 1, 19, "pos " + to_string(pos) );
			Sout( 1,get_terminal_size().ws_row, Title + str);
		}
	}
	return str;

}

void Lineedit_class::Text( vector_str_t& text )
{
	int pos = text.size();
	string line = "";
	text.push_back( line );
	kbdInt_t key = 0;
	Sout( 1, get_terminal_size().ws_row, text[pos] );
	range_T<int> text_range { 0, (int)text.size()-1 };
	while( key != KEYCODE::ESC )
	{
		key = Kbd->GetKeyInt( delay );
		switch (key)
		{
			case KEYCODE::UP	: { pos++;  break; }
			case KEYCODE::DOWN	: { pos--;  break; }
			case KEYCODE::ESC	: { return; }
			case KEYCODE::RET	: { return; }
			case KEYCODE::LEFT	: { text[pos] = Line( text[pos] ) ; break; }
			case KEYCODE::BACK	: { text[pos] = Line( text[pos] ) ; break; }
			default 			: { text[pos].push_back(char(key));
									text[pos] = Line( text[pos] ) ; break; }
		} // switch key
		pos = check_range( text_range, pos, "" );
		if ( key > 0 )
			Sout( 1, get_terminal_size().ws_row, text[pos] );
	}
}

void Lineedit_class::Program( program_vec_t& program )
{
	int pos = program.size();
	string text = "";
	program.push_back( line_struct( pos, text ));
	kbdInt_t key = 0;
	Sout( 1, get_terminal_size().ws_row, program[pos].line );
	range_T<int> text_range { 0, (int)program.size()-1 };
	while( key != KEYCODE::ESC )
	{
		string text = program[pos].line;
		key = Kbd->GetKeyInt( delay );
		switch (key)
		{
			case KEYCODE::UP	: { pos++;  break; }
			case KEYCODE::DOWN	: { pos--;  break; }
			case KEYCODE::ESC	: { return; }
			case KEYCODE::RET	: { return; }
			case KEYCODE::LEFT	: { text = Line( text ) ; break; }
			case KEYCODE::BACK	: { text = Line( text ) ; break; }
			default 			: { text.push_back(char(key));
									text = Line( text ) ; break; }
		} // switch key
		int No 					= program[pos].no;
		ls 						= line_struct( No, text );
		program[pos]			= ls;
//		program[pos].line		= ls.line;
//		program[pos].keyw		= ls.keyw;
//		program[pos].args		= ls.args;
		pos = check_range( text_range, pos, "" );
		if ( key > 0 )
			Sout( 1, get_terminal_size().ws_row, program[pos].line );
	}
}

