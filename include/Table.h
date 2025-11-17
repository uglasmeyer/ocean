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
