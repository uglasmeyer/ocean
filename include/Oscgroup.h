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
	oscvec_t				oscgroup;
	char					oscroleId;

	Oscgroup_class( char id );
	virtual ~Oscgroup_class();

	void Set_Osc_Note( pitch_t& pitch, const uint& duration, const uint& volume  );
	void Run_Oscgroup( buffer_t offs );
	void SetWd( Wavedisplay_class* wd );

};

#endif /* OSCGROUP_H_ */
