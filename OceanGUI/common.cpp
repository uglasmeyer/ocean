/*
 * common.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: sirius
 */

#include <Common.h>



QStringList Qstringlist ( const list<string>& str_lst )
{
	QStringList Qlist{};
	for ( string str : str_lst )
	{
		Qlist.push_back( ( QString::fromStdString( str ) ) );
	}
	return Qlist;
};

QStringList	Qstringvector( const vector<string>& str_vec )
{
	QStringList Qlist{};
	for ( string str : str_vec )
	{
        Qlist.push_back( ( QString::fromStdString( str ) ) );
	}
	return Qlist;
	;
}


vector<QString> Vstringvector( const vector<string>& str_vec )
{
	vector<QString> Qvec{};
	for ( string str : str_vec )
	{
		Qvec.push_back( ( QString::fromStdString( str ) ) );
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
   		qlist.push_back( QString::fromStdString( path.stem() ) );
    }
    return qlist;
}
