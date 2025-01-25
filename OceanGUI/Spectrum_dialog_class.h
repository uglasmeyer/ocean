#ifndef SPECTRUM_DIALOG_CLASS_H
#define SPECTRUM_DIALOG_CLASS_H

#include <QDialog>
#include <QTimer>

#include <Ocean.h>
#include <Spectrum.h>
#include <data/Interface.h>
#include <Keys.h>

namespace Ui {
class Spectrum_Dialog_class;
}

class Spectrum_Dialog_class : public QDialog, public virtual Spectrum_base
{
    Q_OBJECT

public:
    explicit Spectrum_Dialog_class(QWidget *parent = nullptr,
                                   Interface_class* gui = nullptr );
    ~Spectrum_Dialog_class();

    Spectrum_base Spectrum{};
    spectrum_t spectrum;

    Interface_class* Sds;
    interface_t* ifd;
    vector<Spectrum_base::spectrum_t*> ifd_spectrum_vec;

    string instrument{};
    uint8_t waveform_id;

    void setup_Widgets( Spectrum_base::spectrum_t );
    void Update_spectrum();

private slots:
    void vS1( int );
    void vS2( int );
    void vS3( int );
    void vS4( int );
    void vS5( int );
    void vS6( int );
    void vS7( int );
    void vS8( int );
    void save( );
    void select_spec_fmo();
    void select_spec_vco();
    void select_spec_main();

private:
    Ui::Spectrum_Dialog_class* ui;
    QTimer	status_timer_obj 	{ this };
    QTimer* status_timer 		= &status_timer_obj;

};

#endif // SPECTRUM_DIALOG_CLASS_H
