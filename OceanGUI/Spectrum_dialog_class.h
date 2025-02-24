#ifndef SPECTRUM_DIALOG_CLASS_H
#define SPECTRUM_DIALOG_CLASS_H

#include <QDialog>
#include <QTimer>

#include <Ocean.h>
#include <Spectrum.h>
#include <data/Interface.h>
#include <EventKeys.h>

namespace Ui {
class Spectrum_Dialog_class;
}

class Spectrum_Dialog_class :
		public QDialog,
		public virtual Spectrum_class
{
    Q_OBJECT

public:
    unique_ptr<Ui::Spectrum_Dialog_class> 		ui;

    explicit Spectrum_Dialog_class(QWidget *parent = nullptr,
                                   Interface_class* gui = nullptr );
    virtual ~Spectrum_Dialog_class();

    Spectrum_class Spectrum{};
    spectrum_t spectrum;

    Interface_class* Sds;
    interface_t* sds_p;

    vector<Spectrum_class::spectrum_t*> ifd_spectrum_vec;

    Frequency_class	Frequency {};

    string instrument{};
    uint8_t waveform_id;
    vector<QString> Waveform_vec {};
    uint waveform_vec_len = 0;


    void Setup_widgets( Spectrum_class::spectrum_t );
    void Update_spectrum();
    void SetLabelWaveform( const QString& wf );
    void SetLabelInstrument( const QString& instr );
    void SetSds( Interface_class* Sds, int8_t id );

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

    void save( );
    void select_spec_fmo();
    void select_spec_vco();
    void select_spec_main();
    void reset();

private:

};

#endif // SPECTRUM_DIALOG_CLASS_H
