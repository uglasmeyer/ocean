/*
 * Oscgroup.h
 *
 *  Created on: Jan 22, 2025
 *      Author: sirius
 */

#ifndef OSCGROUP_H_
#define OSCGROUP_H_

#include <Ocean.h>
#include <notes/Notesbase.h>
#include <Osc.h>
#include <Wavedisplay.h>


typedef vector<Oscillator*> oscvec_t;

class Oscgroup_class :
		public virtual Logfacility_class,
		public virtual Note_base
{
public:
	Oscillator 				vco;//{ osc_struct::VCOID };
	Oscillator 				fmo;//{ osc_struct::FMOID };
	Oscillator 				osc;//{ osc_struct::OSCID };

	oscvec_t				member{};// = { &vco, &fmo, &osc };

	char					oscroleId;

	Oscgroup_class( char role );
	virtual ~Oscgroup_class();

	void Set_Frequency( const uint8_t& idx,
						const uint& mode );
	void Set_Osc_Note( 	const uint8_t& key,
						const uint& duration,
						const uint& volume,
						const uint& mode);
	void Run_OSCs( 	const buffer_t& offs );
	void Set_Duration( 	const uint& duration );

	void Data_Reset();
	void Connection_Reset();
	void SetWd( Wavedisplay_class* wd );
	string Show_Spectrum( );
	Oscillator* Get_osc_by_name( const string& name );
	void SetSlide( const uint8_t& value );
	void Show_sound_stack();



};

#endif /* OSCGROUP_H_ */
