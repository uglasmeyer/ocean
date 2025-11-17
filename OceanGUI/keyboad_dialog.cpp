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
 * keyboard_dialog.cpp
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */


#include "ui_keyboad_dialog.h"
#include <include/Common.h>
#include <include/Keyboad_dialog.h>

Keyboad_Dialog_class::Keyboad_Dialog_class(
		QWidget* 			parent,
		Dataworld_class* 	_data,
		EventLog_class*		_log
		)
    : QDialog(parent)
    , ui(new Ui::Keyboad_Dialog_class)
{

	this->DaTA 		= _data;
	this->Sds		= DaTA->Sds_master;
	this->sds_p		= Sds->addr;
	this->Eventlog_p= _log;

	ui->setupUi(this);
    ui->cB_buffer_mode->addItems( QStringList{ "persist", "forget" } );
	connect( ui->cB_buffer_mode, SIGNAL(activated( int )), this, SLOT( buffer_mode( int )));
    connect( ui->sb_base_octave, SIGNAL(valueChanged(int)), this, SLOT( base_octave(int) ));
    connect( ui->sb_flats, SIGNAL(valueChanged(int)), this, SLOT( flats(int) ));
    connect( ui->sb_sharps, SIGNAL(valueChanged(int)), this, SLOT( sharps(int) ));
    connect( ui->cb_decay_mode	, SIGNAL( clicked(bool)), 	this, SLOT( decay_mode(bool) ));
    connect( ui->cb_sliding_mode	, SIGNAL( clicked(bool)), 	this, SLOT( sliding_mode(bool) ));
    connect( ui->pB_Save, SIGNAL(clicked()), this, SLOT( save()));
	Setup_Widget();
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

Keyboad_Dialog_class::~Keyboad_Dialog_class()
{
    delete ui;
}

void Keyboad_Dialog_class::buffer_mode( int value )
{
	Sds->Set( sds_p->StA_state_arr[ STA_KEYBOARD ].forget, (bool)value );
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

void Keyboad_Dialog_class::base_octave(int value )
{
	Sds->Set( sds_p->Kbd_state.base_octave, value );
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

void Keyboad_Dialog_class::flats(int value )
{
	Sds->Set( sds_p->Kbd_state.flats, (uint)value );
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

void Keyboad_Dialog_class::sharps(int value )
{
	Sds->Set( sds_p->Kbd_state.sharps, (uint)value );
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

void Keyboad_Dialog_class::decay_mode(bool value )
{
	Sds->Set( sds_p->Kbd_state.ADSR_flag, value );
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

void Keyboad_Dialog_class::sliding_mode(bool value )
{
	Sds->Set( sds_p->Kbd_state.sliding, value );
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}
void Keyboad_Dialog_class::save()
{
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}
void Keyboad_Dialog_class::Setup_Widget()
{
	stringstream strs{};
	strs << setprecision(5) << sds_p->Kbd_state.frq ;
	char bmode = (char)sds_p->StA_state_arr[ STA_KEYBOARD ].forget;
	ui->cB_buffer_mode->setCurrentIndex( bmode );
	ui->cb_decay_mode->setChecked( sds_p->Kbd_state.ADSR_flag );
	ui->cb_sliding_mode->setChecked( sds_p->Kbd_state.sliding);
	ui->sb_base_octave->setValue( sds_p->Kbd_state.base_octave );
	ui->sb_flats->setValue( sds_p->Kbd_state.flats );
	ui->sb_sharps->setValue( sds_p->Kbd_state.sharps );
	ui->lbl_key->setText( keyboard_key );
	ui->lbl_frq->setText( Qstring( strs.str() ));
	ui->lbl_note->setText( Qstring( sds_p->Kbd_state.note ));

	char chord_type = sds_p->Kbd_state.chord_type;
	QString Qstr = Qstring( get<1>( Kbd_pitch.Chords_map[chord_type] ) );
	ui->lbl_chord->setText( Qstr );
}
