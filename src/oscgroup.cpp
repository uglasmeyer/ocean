/*
 * Oscgroup.cpp
 *
 *  Created on: Jan 22, 2025
 *      Author: sirius
 */

#include <Oscgroup.h>

Oscgroup_class::Oscgroup_class( char id ) :
  Logfacility_class( "Oscgroup" ),
  Note_base()
{
	oscroleId = id;
	vco.SetId( oscroleId, osc_struct::VCOID );
	fmo.SetId( oscroleId, osc_struct::FMOID );
	osc.SetId( oscroleId, osc_struct::OSCID );

	member = { &vco, &fmo, &osc };

}

Oscgroup_class::~Oscgroup_class()
{

}

void Oscgroup_class::SetWd( Wavedisplay_class* wd )
{
	wd->Add_data_ptr( osc_struct::VCOID , oscroleId, vco.Mem.Data);
	wd->Add_data_ptr( osc_struct::FMOID , oscroleId, fmo.Mem.Data);
	wd->Add_data_ptr( osc_struct::OSCID , oscroleId, osc.Mem.Data);

}
void Oscgroup_class::Set_Osc_Note( pitch_t& pitch, const uint& duration, const uint& volume )
{
	const float vco_wp_frequency = vco.wp.frequency;
	const float fmo_wp_frequency = fmo.wp.frequency;

	float
	fnew = CalcFreq( vco_wp_frequency, pitch );
	vco.Set_start_freq(fnew);
	vco.wp.frequency 	= fnew;
	vco.wp.msec 		= duration;

	fnew = CalcFreq( fmo_wp_frequency, pitch );
	fmo.Set_start_freq(fnew);
	fmo.wp.frequency	= fnew;
	fmo.wp.msec 		= duration;

	fnew = CalcFreq(  oct_base_freq , pitch );
	osc.Set_start_freq( fnew);
	osc.wp.frequency	= fnew;
	osc.wp.volume 		= volume ;
	osc.wp.msec 		= duration;

	vco.wp.frequency = vco_wp_frequency;
	fmo.wp.frequency = fmo_wp_frequency;
}

void Oscgroup_class::Run_Oscgroup( buffer_t offs )
{

	for ( Oscillator* osc : member )
	{
		if ( oscroleId == osc_struct::INSTRID )
			osc->Mem.Clear_data( max_data_amp * osc->wp.adjust * 0.01 );
		osc->OSC( offs );
	}
}
