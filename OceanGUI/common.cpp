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
 * common.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <include/Common.h>



QStringList Qstringlist ( const list<string>& str_lst )
{
	QStringList Qlist{};
	for ( string str : str_lst )
	{
		Qlist.push_back( ( Qstring( str ) ) );
	}
	return Qlist;
};

QStringList	Qstringvector( const vector<string>& str_vec )
{
	QStringList Qlist{};
	for ( string str : str_vec )
	{
        Qlist.push_back( ( Qstring( str ) ) );
	}
	return Qlist;
	;
}


vector<QString> Vstringvector( const vector<string>& str_vec )
{
	vector<QString> Qvec{};
	for ( string str : str_vec )
	{
		Qvec.push_back( ( Qstring( str ) ) );
	}
	return Qvec;

};

QStringList Qread_filenames( const Path_t dir_ext )
{
	QStringList qlist{};
	vector<string> dir_vec = List_directory( dir_ext.dir, dir_ext.ext );
    for ( string str :  dir_vec )
    {
    	std::filesystem::path path{ str };
   		qlist.push_back( Qstring( path.stem() ) );
    }
    return qlist;
}
