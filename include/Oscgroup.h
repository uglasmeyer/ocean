/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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
 * Oscgroup.h
 *
 *  Created on: Jan 22, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSCGROUP_H_
#define OSCGROUP_H_

#include <notes/Notesbase.h>
#include <Osc.h>
#include <Wavedisplay.h>


/**************************************************
 * Oscgroup_class
 * function to control the Oscillator set
 *************************************************/

class 						Oscgroup_class
	: public virtual 		Logfacility_class
{
public:
	Oscillator 				vco					;
	Oscillator 				fmo					;
	Oscillator 				osc					;
	vector<Oscillator*>		member				;


							Oscgroup_class		(); //used by FileDialog, musicsml, variation
							Oscgroup_class		( RoleId_e role, buffer_t bytes  );
	void 					operator=			( Oscgroup_class& oscg );
	virtual 				~Oscgroup_class		();

	void 					ChordData			( interface_t* sds,
												chord_t chord );
	void 					ConnectionReset		();
	void 					DataReset			();
	Oscillator* 			GetOsc_byName		( const string& name );
	OSCID_e 				GetOscid_byName		( const string& name );
	void					KbdAttack			();
	void 					OscConnectState		( interface_t* sds );
	void 					ResetBeatCursor		();
	void 					RestorePhase		( pitch_t& pitch );
	void 					RunOSCs				( const buffer_t& offs );
	void 					RunAdsr				();
	void 					SetAdsr				( interface_t* sds );
	void					SetBeatcursor		( buffer_t pos );
	void 					SetDuration			( const uint& duration );
	void 					SetInstrument		( interface_t* sds );
	void 					SetFrqSlidevalue	( const uint8_t& value );
	void 					SetFeatures			( interface_t* sds );
	void					SetNoteFrequency	( interface_t* 	sds,
												const uint8_t& 	idx );
	void 					SetScanner			( const buffer_t& maxlen );
	void 					SetSliderFrequency	(interface_t* sds,
												const uint8_t& idx,
												const DYNAMIC_e& mode );
	void 					SetSpectrum			( interface_t* sds );
	void 					SetWd				( Wavedisplay_class* wd );
	void 					ShowSoundStack		();
	bool					Sync				();


private:
	void 					backup_phase			( pitch_t& pitch );
	void 					selfTest			();


};

typedef Oscgroup_class Oscgroup_t;

#endif /* OSCGROUP_H_ */
