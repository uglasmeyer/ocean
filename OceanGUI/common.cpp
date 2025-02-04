/*
 * common.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: sirius
 */

#include <Common.h>

#include <ui_File_Dialog.h>

void Set_button_color( QPushButton* pb, QColor color  )
{
	QPalette status_color = QPalette();
	status_color.setColor(QPalette::Button, color);
	pb->setPalette( status_color );
};

QStringList Qstringlist ( const list<string>& str_lst )
	{
		QStringList Qlist{};
		for ( string str : str_lst )
		{
            Qlist.push_back( ( QString::fromStdString( str ) ) );
		}
		return Qlist;

	};

QStringList	 	Qstringvector( const vector<string>& str_vec )
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

void Qread_filelist( QComboBox* CB, const string& path, const string& type )
{

    if ( not CB ) return;
    CB->clear();
    vector<string> files = List_directory( path, type );
    sort( files.begin(), files.end() );
    for ( string& file : files )
    {
    	size_t len = file.length() - type.length();
    	string name = file.substr(0, len);
    	CB->addItem( QString::fromStdString( name ) );
    }
}
