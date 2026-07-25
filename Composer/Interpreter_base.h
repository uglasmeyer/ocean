/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * Interpreter_base.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INTERPRETER_BASE_H_
#define INTERPRETER_BASE_H_

#include <String.h>

#define JMPUNDEF 	-1

const string_vec_t Keyword_vec
{
	"add",
	"adsr",
	"=",
	"call",
	"+",
	"debug",
	"dyn",
	"endif",
	"exit",
	"if",
	"instrument",
	"notes",
	"osc",
	"pause",
//	"play",
	"random",
	"rec",
	"record",
	"reg",
	"reset",
	"return",
	"set",
	"show",
	"sta",
	"start",
	"stop",
	":",
	"text"
};

namespace Interpreter_ns
{
	enum KEYWORD_e : Id_t
	{
		ADDMUSIC = 0,
		ADSR,
		ASSIGN,
		CALL,
		CONCAT,
		DEBUG,
		DYN,
		ENDIF,
		EXIT,
		IF,
		INSTRUMENT,
		NOTES,
		OSC,
		PAUSE,
//		PLAY,
		RANDOM,
		REC,
		RECORD,
		REG,
		RESET,
		RETURN,
		SET,
		SHOW,
		STA,
		START,
		STOP,
		SUB,
		TEXT,
		KEYW_SIZE
	};
	struct keyword_map_struct
	{
		KEYWORD_e 	key;
		string 		word;
	};
	typedef vector<keyword_map_struct> keyword_map_vec_t;

	constexpr keyword_map_vec_t map_keywords( )
	{
		assert( KEYW_SIZE == Keyword_vec.size() );
		keyword_map_vec_t 	vec {};
		KEYWORD_e 			key = KEYWORD_e(0);
		for( string word : Keyword_vec )
		{
			vec.push_back( { key, word } );
			key++;
		}
		return vec;
	}

	const keyword_map_vec_t keyword_map_vec = map_keywords();
	const uset_t 			Keywords		= vector2uset( Keyword_vec );

	typedef struct line_data_struct
	{
		int				Id	= 0; 		// Program Position
		uint 			no	= 0; 		// filename line no.
		int				jmp = JMPUNDEF;
		string 			name; 			// filename
		string 			line; 			// program line string
		string 			keyw; 			// keyword
		Interpreter_ns::KEYWORD_e
						key;
		string 			arg1; 			// unused
		string_vec_t	args; 			// program line arr
	} line_data_t;


	typedef struct line_struct :
		line_data_struct
	{
		line_struct( uint _no, string _line, string _name = "" );
	} line_struct_t;

	constexpr KEYWORD_e GetKeywordId( const string& word )
	{
		for( keyword_map_struct map : keyword_map_vec )
		{
			if( strEqual( word, map.word ) )
					return map.key;
		}
		if( word[0] == ':' )
			return SUB;
		return KEYW_SIZE;
	}

}




typedef vector<Interpreter_ns::line_struct_t> 	program_vec_t;



#endif /* INTERPRETER_BASE_H_ */
