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
 * Oscgroup.h
 *
 *  Created on: Jan 22, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef OSCGROUP_H_
#define OSCGROUP_H_

#include <Ocean.h>
#include <notes/Notesbase.h>
#include <Osc.h>
#include <Wavedisplay.h>
#include <data/SharedDataSegment.h>



class 						Oscgroup_class :
	public virtual 			Logfacility_class,
	virtual					Frequency_class,
	public virtual 			Note_base,
	virtual					osc_struct
{
	string					className			= "";
public:
	Oscillator 				vco;
	Oscillator 				fmo;
	Oscillator 				osc;

	vector<Oscillator*>		member				{};// = { &vco, &fmo, &osc };



	OscroleId_t				oscroleId;

							Oscgroup_class		( OscroleId_t role, buffer_t bytes  );
	void 					operator=			( Oscgroup_class& oscg );
	virtual 				~Oscgroup_class		();

	void					Set_Note_Frequency	( interface_t* sds,
												  const uint8_t& idx,
												  const uint& mode );
	void 					Set_Combine_Frequency(const uint8_t& base_idx,
												  const uint8_t& idx,
												  const uint& mode );
	void 					Set_Osc_Note		( interface_t* sds,
												  const uint8_t& key,
												  const uint& duration,
												  const uint& volume,
												  const uint& mode);
	void 					Run_OSCs			( const buffer_t& offs );
	void 					Set_Duration		( const uint& duration );

	void 					Data_Reset			();
	void 					Phase_Reset			();
	void 					Connection_Reset	();
	void 					Set_Connections		( interface_t* sds );
	void 					SetWd				( Wavedisplay_class* wd );
	void 					SetScanner			( const buffer_t& maxlen );
	string 					Show_Spectrum		();
	Oscillator* 			Get_osc_by_name		( const string& name );
	OSCID_e 				Get_oscid_by_name	( const string& name );

	void 					SetSlide			( const uint8_t& value );
	void 					SetFeatures			( interface_t* sds );
	void 					SetAdsr				( interface_t* sds );
	void 					SetSpectrum			( interface_t* sds );

	void 					Show_sound_stack	();
	void 					SetInstrument		( interface_t* sds );

private:
	void 					selfTest			();


};

#endif /* OSCGROUP_H_ */
