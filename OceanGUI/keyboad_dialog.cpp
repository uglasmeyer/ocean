#include "Keyboad_dialog.h"
#include "ui_keyboad_dialog.h"
#include <include/Common.h>

Keyboad_Dialog_class::Keyboad_Dialog_class(
		QWidget* 			parent,
		Dataworld_class* 	_data,
		EventLog_class*		_log)
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

	Setup_Widget();
	Eventlog_p->add( SDS_ID, KBD_EVENT_KEY );
}

Keyboad_Dialog_class::~Keyboad_Dialog_class()
{
    delete ui;
}

void Keyboad_Dialog_class::buffer_mode( int value )
{
	Sds->Set( sds_p->StA_state[ STA_KEYBOARD ].forget, (bool)value );
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
void Keyboad_Dialog_class::Setup_Widget()
{
	stringstream strs{};
	strs << setprecision(5) << sds_p->Kbd_state.frq ;
	char bmode = (char)sds_p->StA_state[ STA_KEYBOARD ].forget;
	ui->cB_buffer_mode->setCurrentIndex( bmode );
	ui->cb_decay_mode->setChecked( sds_p->Kbd_state.ADSR_flag );
	ui->cb_sliding_mode->setChecked( sds_p->Kbd_state.sliding);
	ui->sb_base_octave->setValue( sds_p->Kbd_state.base_octave );
	ui->sb_flats->setValue( sds_p->Kbd_state.flats );
	ui->sb_sharps->setValue( sds_p->Kbd_state.sharps );
	ui->lbl_key->setText( keyboard_key );
	ui->lbl_frq->setText( Qstring( strs.str() ));
	ui->lbl_note->setText( Qstring( sds_p->Kbd_state.note ));
}
