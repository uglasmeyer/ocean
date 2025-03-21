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
	Oscillator 				osc;
	Oscillator 				vco;
	Oscillator 				fmo;
	oscvec_t				member;
	char					oscroleId;

	Oscgroup_class( char id );
	virtual ~Oscgroup_class();

	void Set_Frequency( const uint8_t& idx,
						const uint& mode );
	void Set_Osc_Note( 	const uint8_t& key,
						const uint& duration,
						const uint& volume,
						const uint& mode);
	void Run_Oscgroup( 	const buffer_t& offs );
	void Data_Reset();
	void Connection_Reset();
	void SetWd( Wavedisplay_class* wd );

};

#endif /* OSCGROUP_H_ */
