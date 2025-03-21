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

void Oscgroup_class::Set_Frequency( const uint8_t& idx, const uint& mode )
{
	if ( idx < 0 ) return;
//	if ( idx == osc.wp.frqidx ) return; // nothing to do but  what about notes

	int diff = idx - osc.wp.frqidx;
	osc.Set_frequency( idx, mode );
	vco.Set_frequency( vco.wp.frqidx + diff, mode );
	fmo.Set_frequency( fmo.wp.frqidx + diff, mode );
}

void Oscgroup_class::Set_Osc_Note( 	const uint8_t& key,
									const uint& duration,
									const uint& volume,
									const uint& mode)
{
	Set_Frequency( key, mode );
	osc.wp.volume	= volume ;
	std::ranges::for_each( member,
			[ &duration ](Oscillator*  o){ o->Set_duration( duration );});
}
void Oscgroup_class::Data_Reset()
{
	std::ranges::for_each( member,
			[](Oscillator*  o){ o->Data_reset() ;});
}
void Oscgroup_class::Connection_Reset()
{
	std::ranges::for_each( member,
			[](Oscillator*  o){ o->Connection_reset() ;});
}

void Oscgroup_class::Run_Oscgroup( const buffer_t& offs )
{
	std::ranges::for_each( member,
			[ &offs ](Oscillator* o){ o->OSC( offs ) ;} );
}
