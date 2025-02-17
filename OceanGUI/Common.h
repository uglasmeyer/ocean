/*
 * Common.h
 *
 *  Created on: Jan 7, 2025
 *      Author: sirius
 */

#include <Ocean.h>
#include <System.h>
// Qt includes
#include <QDialog>
#include <QComboBox>
#include <QString>
#include <QStringList>

extern QStringList 		Qstringlist( const list<string>& str_lst );
extern vector<QString> 	Vstringvector( const vector<string>& str_vec );
extern QStringList	 	Qstringvector( const vector<string>& str_vec );

extern QStringList		Qread_filenames( const string& path, const string& type );

