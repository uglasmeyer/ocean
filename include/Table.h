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
 * Table.h
 *
 *  Created on: Mar 10, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <Logfacility.h>

typedef struct 			tableOpt_struct
{
	uint 				Ident 		= 0;
	string				Titel 		= "";
	fstream* 			FILE 		= nullptr;
	char 				Separator 	= ' ';
	char				Crlf		= '\n';
} tableopt_t;
const tableopt_t		defaultopt	= tableOpt_struct();

class Table_class :
	virtual Logfacility_class
{
	string		 			className 	= "";

	typedef struct 			header_struct
	{
		string 				txt;
		uint 				width;
	} header_t;
	vector<header_t> 		header_v 	{};

public:

	tableopt_t 				opt 		= defaultopt;

							Table_class	( string title = "", uint ident = LOGINDENT );
							Table_class	( tableopt_t _opt );
	virtual 				~Table_class();

	void 					PrintHeader	();
	void 					TestTable	();
	void 					AddColumn	( string name, uint width );

	template< typename... 	Args>
	string AddRow( Args... 	args)
	{
		uint argc = sizeof...(args);
		if ( argc > header_v.size() )
		{
			Comment(ERROR, "Invalid number of columns: ", argc, " expected: ", header_v.size());
			return "";
		}

		stringstream strs {};
		uint n	= 0;

		strs << setw( opt.Ident ) << dec << left << "" ;
		(( strs << setw(header_v[n++].width)
				<< args
				<< opt.Separator
				), ...);

		string row = cout_row( strs.str() );

		return row;

	};



private:

	string 					cout_row	( string txt );
};



#endif /* TABLE_H_ */
