/**************************************************************************
MIT License

Copyright (c) 2025, 2026 Ulrich Glasmeyer

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
 * Spectrum_dialog.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */



#ifndef SPECTRUM_DIALOG_CLASS_H
#define SPECTRUM_DIALOG_CLASS_H

// Qt
//#include <QDialog>
#include <QTimer>

// Ocean
#include <data/Interface.h>
#include <data/DataWorld.h>
#include <DataGraphic.h>
#include <Spectrum.h>
#include <EventKeys.h>
#include <Fourier.h>

// oceanGUI
#include <GUIcommon.h>
#include <ui_spectrum_dialog_class.h>

using namespace spectrum_ns;
using namespace fourier_ns;

/**************************************************
 * Mode_struct combines the wd mode and the fourier mode
 * in terms of predefined and dynamic data
 *************************************************/
struct Mode_struct
{
private:
	typedef struct mode_map_struct
	{
		WFID_e					wfid = WFID_e::ADSR;
		QString					text = "Manual";
		datagraphics_param_t	param{ 1.0, false, false };
	} mode_map_t;
	map <MODE_e, mode_map_t>	at_mode_map;
	map <MODE_e, mode_map_t>	aw_mode_map;
public:

	virtual 				~Mode_struct() = default;
							Mode_struct();

	datagraphics_param_t 	At_param( bool adsr );
	datagraphics_param_t 	Aw_param( MODE_e mode );
	WFID_e 					Wfid( bool adsr, MODE_e mode );
	QString 				Text( bool adsr, MODE_e mode );
};

/**************************************************
 * Spectrum_Dialog_class
 *************************************************/
QT_BEGIN_NAMESPACE
namespace Ui
{
	class Spectrum_Dialog_class;
}
QT_END_NAMESPACE

class Spectrum_Dialog_class :
		public 			QDialog,
		public virtual	Interface_base

{
    Q_OBJECT
    spectrum_t 				spectrum			;// active selected spectrum data
    spectrum_t				reset_spectrum		;// previouse spectrum data
    spectrum_t				adsr_reset_spectrum	; // previouse adsr spectrum data
    OSCID_e					OscId				;// selected OSC
    uint8_t					Channel 			;//= 1; // active slot last selected Wafeform out of 4
    bool 					adsrmode 			;// distinguish between Spectrum and ADSR mode
    bool					highFrequencies		;
    Waveform_class*			Waveform			;
    QGraphicsScene*     	atScene				;
    QGraphicsScene*			awScene				;
	Mode_struct				Mode				;
	QStringList				Qfilter_str_lst		;


public:

    explicit Spectrum_Dialog_class(QWidget*			parent	= nullptr,
                                   Dataworld_class* data	= nullptr,
								   EventLog_class* 	_log	= nullptr);
    virtual ~Spectrum_Dialog_class();


	Ui::Spectrum_Dialog_class*	ui;
	EventLog_class*			Eventlog_p;

    vector<QSlider*>		fS_vec 			{};
    vector<QSlider*>		vS_vec			{};
    vector<QSpinBox*>		sb_vec 			{};
    vector<QRadioButton*> 	rb_vec 			{};
    vector<QString> 		QWaveform_vec	{};

    DataGraphic_class*		AtDataDisplay	; // created by new DataGraphic_class;
    DataGraphic_class*		AwDataDisplay;
    fourier_res_t			Resolution		;

	void Dialog ( bool flag );
    void Select_oscid( OSCID_e oscid );
	void SetSds();
    void Set_adsr_flag( bool flag );
	void Setup_widgets();

public slots:

	void UpdateDataDisplay();
	void SetInstrument(); // Dispatcher

private slots:

	void shift_frequency( int );
	void fourierfilter_slot ( int );
	void pB_Fourier_slot();
	void resolution_slot( int idx );
	void fS1( int );
    void vS1( int );
    void fS2( int );
    void vS2( int );
    void fS3( int );
    void vS3( int );
    void fS4( int );
    void vS4( int );
    void rb_high_low_slot();

    void sb_wf1(int);
    void sb_wf2(int);
    void sb_wf3(int);
    void sb_wf4(int);
    void cb_bps_slot(int);


    void attack(int);
    void decay (int);


    void saveInstrument( );
    void select_oscid_fmo();
    void select_oscid_vco();
    void select_oscid_main();
    void reset();

    void adsr_slot( bool flag );



private:


    void add_event( EVENTKEY_e key );
    void change_spectrum_view();
	void initDataDisplay();
	void initDialog();
	void read_spectrum();
	void save_spectrum_data();
	uint8_t select_channel( int8_t ch );
    void set_fourier_mode( fourier_ns::MODE_e );
    void setWaveformLabel( WFID_e wfid );
    void set_waveform_vec();
    void spec_vol_slider( int channel, int value );
    void spec_frq_slider( int channel, int value );
    void update_lcd( float value, QString label );
    void waveform_spinbox( uint8_t id, int value  );

};

#endif // SPECTRUM_DIALOG_CLASS_H
