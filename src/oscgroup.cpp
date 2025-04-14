/*
 * Oscgroup.cpp
 *
 *  Created on: Jan 22, 2025
 *      Author: sirius
 */

#include <Oscgroup.h>

Oscgroup_class::Oscgroup_class( char role ) :
  Logfacility_class( "Oscgroup" ),
  Note_base(),
  vco( role, osc_struct::VCOID ),
  fmo( role, osc_struct::FMOID ),
  osc( role, osc_struct::OSCID )
{
	member = { &vco, &fmo, &osc };

	oscroleId = role;

}
Oscgroup_class::~Oscgroup_class() = default;

void Oscgroup_class::SetSlide( const uint8_t& value )
{
	std::ranges::for_each( member, [ value ](Oscillator*  o)
			{ o->Set_glide( value);	});
}

void Oscgroup_class::SetWd( Wavedisplay_class* wd )
{

	std::ranges::for_each( member, [ this, wd ](Oscillator*  o)
			{ wd->Add_data_ptr(o->osctype_id, oscroleId, o->MemData() ); });
}

void Oscgroup_class::Set_Frequency( const uint8_t& idx, const uint& mode )
{
	int diff = idx - osc.wp.frqidx;
	osc.Set_frequency( idx, mode );
	vco.Set_frequency( vco.wp.frqidx + diff, mode );
	fmo.Set_frequency( fmo.wp.frqidx + diff, mode );
}

void Oscgroup_class::Set_Duration( const uint& duration )
{
	std::ranges::for_each( member, [ &duration ](Oscillator*  o)
			{ o->Set_duration( duration );});
}
void Oscgroup_class::Set_Osc_Note( 	const uint8_t& key,
									const uint& duration,
									const uint& volume,
									const uint& mode)
{
	Set_Frequency( key, mode );
	Set_Duration( duration );
	osc.Set_adsr( osc.adsr );
	osc.wp.volume	= volume ;
}
void Oscgroup_class::Data_Reset()
{
	std::ranges::for_each( member, [](Oscillator*  o)
			{ o->Data_reset() ;});
}
void Oscgroup_class::Connection_Reset()
{
	std::ranges::for_each( member, [](Oscillator*  o)
			{ o->Connection_reset() ;});
}

void Oscgroup_class::Run_OSCs( const buffer_t& offs )
{
	std::ranges::for_each( member, [ &offs ](Oscillator* o)
			{ o->OSC( offs ) ;} );
}
string Oscgroup_class::Show_Spectrum( )
{
	stringstream strs{};
	std::ranges::for_each( member,[ &strs ](Oscillator* o)
			{ strs << o->Show_this_spectrum() ;} );
	return strs.str();
}

Oscillator* Oscgroup_class::Get_osc_by_name( const string& name )
{
	Oscillator* ret = nullptr;

	std::ranges::for_each( member, [ name, &ret ](Oscillator* o)
			{ if ( strEqual( o->osc_type, name ))
				ret = o;} );

	if ( strEqual( "MAIN", name  ) ) // compatibility
		ret = member[ osc_struct::OSCID ];
	if ( ret == nullptr )
		{ EXCEPTION( "unknown Oscillator name: " + name ); }

	return ret;
}
