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
 * Interpreter_base.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INTERPRETER_BASE_H_
#define INTERPRETER_BASE_H_

#include <String.h>

typedef struct line_struct
{
	int				Id	= 0; // Program Position
	uint 			no	= 0; // filename line no.
	string 			name; // filename
	string 			line; // program line string
	string 			keyw; // keyword
	string 			arg1; // unused
	vector_str_t	args {}; // program line arr

	line_struct( uint _no, string _line, string _name = "" )
	{
		no		= _no;
		line 	= _line;
		name	= ( _name.length() > 0 ) ? filesystem::path(_name).stem() : "stdin" ;
		String Str { _line };
		Str.replace_comment();
		Str.replace_char('\t' , ' ');
		args 	= Str.to_unique_array( ' ' );
		keyw 	= ( args.size() > 0 ) ? args[0] : "";
		Str.to_lower( keyw );
	}
	void show()
	{
		coutf 	<< dec << left
				<< setw( 4) << no << ":"
				<< setw(16) << name
				<< setw(60) << line
				<<endl;
	}
} line_struct_t;
typedef vector<line_struct_t> 	program_vec_t;

const set<string> Keywords {
						"add",
						"adsr",
						"call",
						"exit",
						"instrument",
						"notes",
						"osc",
						"play",
						"pause",
						"random",
						"rec",
						"record",
						"return",
						"set",
						"show",
						"start",
						"stop",
						"text"
					};





#endif /* INTERPRETER_BASE_H_ */
