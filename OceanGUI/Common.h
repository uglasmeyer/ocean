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

extern void 			Set_button_color( QPushButton* pb, QColor color  );
extern QStringList 		Qstringlist( const list<string>& str_lst );
extern vector<QString> 	Vstringvector( const vector<string>& str_vec );
extern QStringList	 	Qstringvector( const vector<string>& str_vec );

extern void Qread_filelist( QComboBox* CB, const string& path, const string& type );

