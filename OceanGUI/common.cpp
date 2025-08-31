/*
 * common.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: sirius
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
