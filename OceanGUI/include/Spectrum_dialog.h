/**************************************************************************
MIT License

Copyright (c) 2025 Ulrich Glasmeyer

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

#include "ui_spectrum_dialog_class.h"


#include <QDialog>
#include <QTimer>

#include <Ocean.h>
#include <Spectrum.h>
#include <data/Interface.h>
#include <EventKeys.h>
#include <data/DataWorld.h>
#include <include/Common.h>

namespace Ui {
class Spectrum_Dialog_class;
}

class Spectrum_Dialog_class :
		public 			QDialog,
		public virtual 	Spectrum_class

{
    Q_OBJECT
	string className			= "";
public:
    unique_ptr<Ui::Spectrum_Dialog_class> 		ui;

    explicit Spectrum_Dialog_class(QWidget *parent = nullptr,
                                   Interface_class* gui = nullptr,
								   EventLog_class* _log = nullptr);
    virtual ~Spectrum_Dialog_class();

    spectrum_t 				spectrum;

    Interface_class* 		Sds;
    interface_t* 			sds_p;

    EventLog_class*			Eventlog_p;

    adsr_t					adsr_data;
    OSCID_e					OscId;
    Id_t					Channel = 1; // active slot last selected Wafeform out of 4

    vector<QSlider*>		fS_vec {};
    vector<QSpinBox*>		sb_vec {};
    vector<QRadioButton*> 	rb_vec {};
    Frequency_class			Frequency {};
    string 					instrument{};
    uint8_t 				waveform_id;
    vector<QString> 		Waveform_vec {};
    uint 					waveform_vec_len = 0;
    uint8_t 				SDS_ID = 0;


    void Setup_widgets( spectrum_t );
    void Update_instrument();
    void SetLabelWaveform();
    void SetLabelInstrument( const QString& instr );
    void SetSds( Interface_class* Sds, int8_t id );
    void Set_adsr_flag( bool flag );

private slots:
    void fS1( int );
    void vS2( int );
    void fS3( int );
    void vS4( int );
    void fS5( int );
    void vS6( int );
    void fS7( int );
    void vS8( int );

    void sb_wf1(int);
    void sb_wf2(int);
    void sb_wf3(int);
    void sb_wf4(int);
    void cb_bps_slot(int);


    void attack(int);
    void decay (int);

    void save( );
    void select_spec_fmo();
    void select_spec_vco();
    void select_spec_main();
    void reset();

    void adsr_slot( bool flag );



private:
    bool ADSR_flag = false;

    void select_spec( OSCID_e oscid );
    void set_spectrum_data();
    void set_spectrum_view();
    void waveform_spinbox( uint id, int value  );
    void spec_vol_slider( int channel, int value );
    void spec_frq_slider( int channel, int value );
    void set_waveform_vec( vector<string> waveform_vec );
};

#endif // SPECTRUM_DIALOG_CLASS_H
