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

QStringList Qread_filenames( const string& path, const string& type )
{
	QStringList qlist{};
    for ( string file : List_directory( path, type ) )
    {
    	size_t len = file.length() - type.length();
    	string name = file.substr(0, len);
    	qlist.push_back( QString::fromStdString( name ) );
    }
    return qlist;
}
