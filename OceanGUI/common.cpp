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


