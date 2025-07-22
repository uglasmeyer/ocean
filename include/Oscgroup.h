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



class 						Oscgroup_class :
	public virtual 			Logfacility_class,
	public virtual 			Note_base
{
public:
	Oscillator 				vco;
	Oscillator 				fmo;
	Oscillator 				osc;

	vector<Oscillator*>		member				{};// = { &vco, &fmo, &osc };

	char					oscroleId;

							Oscgroup_class		( char role, buffer_t bytes  );
	void 					operator=			( const Oscgroup_class& oscg );
	virtual 				~Oscgroup_class		();

	void					Set_Note_Frequency	( const frq_t& base_freq,
												  const uint8_t& idx,
												  const uint& mode );
	void 					Set_Combine_Frequency(const uint8_t& idx,
												  const uint& mode );
	void 					Set_Osc_Note		( const frq_t&	base_freq,
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
	void 					SetWd				( Wavedisplay_class* wd, buffer_t* frames );
	void 					SetScanner			( const buffer_t& maxlen );
	string 					Show_Spectrum		();
	Oscillator* 			Get_osc_by_name		( const string& name );
	void 					SetSlide			( const uint8_t& value );
	void 					Show_sound_stack	();
	void 					Instrument_fromSDS			( interface_t* sds );

};

#endif /* OSCGROUP_H_ */
