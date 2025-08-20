/*
 * Oscgroup.cpp
 *
 *  Created on: Jan 22, 2025
 *      Author: sirius
 */

#include <Oscgroup.h>

Oscgroup_class::Oscgroup_class( char role, buffer_t bytes )
	: Logfacility_class( "Oscgroup" )
	, Frequency_class()
	, Note_base()
	, vco( role, osc_struct::VCOID, bytes )
	, fmo( role, osc_struct::FMOID, bytes )
	, osc( role, osc_struct::OSCID, bytes )
{
	member 			= { &vco, &fmo, &osc };
	oscroleId 		= role;
	Data_Reset();
	selfTest();
}
void Oscgroup_class::operator=( Oscgroup_class& oscg )
{
	this->vco = oscg.vco;
	this->fmo = oscg.fmo;
	this->osc = oscg.osc;
}

Oscgroup_class::~Oscgroup_class() = default;

void Oscgroup_class::Instrument_fromSDS( interface_t* sds )
{
	osc.Setwp		( sds->OSC_wp );
	osc.Set_spectrum( sds->OSC_spectrum );

	vco.Setwp		( sds->VCO_wp );
	vco.Set_spectrum( sds->VCO_spectrum );

	fmo.Setwp		( sds->FMO_wp );
	fmo.Set_spectrum( sds->FMO_spectrum );

	SetAdsr			( sds );
	SetFeatures		( sds->OSC_features );
	Set_Connections	( sds );
}

void Oscgroup_class::SetFeatures( const feature_t& value )
{
	std::ranges::for_each( member, [ value ](Oscillator*  o)
			{ o->Set_feature( value);	});
}
void Oscgroup_class::SetAdsr( const interface_t* sds )
{
	osc.Set_adsr( sds->OSC_adsr );
	vco.Set_adsr( sds->VCO_adsr );
	fmo.Set_adsr( sds->FMO_adsr );
}

void Oscgroup_class::SetSlide( const uint8_t& value )
{
	std::ranges::for_each( member, [ value ](Oscillator*  o)
			{ o->Set_glide( value);	});
}

void Oscgroup_class::SetWd( Wavedisplay_class* wd )
{
	std::ranges::for_each( member, [ this, wd ](Oscillator*  osc )
			{ wd->Add_data_ptr( osc->typeId,
								osc->roleId,
								osc->MemData_p(),
								&osc->wp.frames ); });
	if( osc.roleId == osc_struct::INSTRID )
	{
		std::ranges::for_each( member, [ this, wd ](Oscillator*  osc )
				{ wd->Add_data_ptr( osc->typeId,
									osc_struct::ADSRID,
									osc->AdsrMemData_p(),
									&osc->adsr_frames ); });
		}
	}

void Oscgroup_class::SetScanner( const buffer_t& maxlen )
{
	std::ranges::for_each( member, [ maxlen ](Oscillator*  o)
	{
		o->scanner.Set_wrt_len( max_frames );
		o->scanner.Set_max_len( maxlen );
	});

}
void Oscgroup_class::Show_sound_stack() // show_status
{
	Table_class 	Table{"Sound stack" };
	Table.AddColumn( "Osc", 4 );
	Table.AddColumn( "Vol", 4 );
	Table.AddColumn( "Frq", 8 );
	Table.AddColumn( "Waveform", 10 );
	Table.AddColumn( "Vol", 4 );
	Table.AddColumn( "U", 8);
	Table.AddColumn( "Frq", 4 );
	Table.AddColumn( "U", 3);

	Table.PrintHeader();
	std::ranges::for_each( member, [ this, &Table ](Oscillator*  o)
			{ o->Get_sound_stack( &Table) ; });
}

void Oscgroup_class::Set_Note_Frequency( 	const frq_t& base_freq,
											const uint8_t& idx,
											const uint& mode )
{
	Set_Combine_Frequency( idx, mode );
	return;
	osc.Set_frequency( idx, mode );

	frq_t ratio = osc.wp.freq / base_freq;
	vco.Set_frequency( Index( ratio * vco.wp.freq ), mode );
	fmo.Set_frequency( Index( ratio * fmo.wp.freq ), mode );
}
void Oscgroup_class::Set_Combine_Frequency( const uint8_t& idx, const uint& mode )
{
	int diff = idx - osc.wp.frqidx;
	osc.Set_frequency( idx, mode );

	vco.Set_frequency( vco.wp.frqidx + diff, mode );
	fmo.Set_frequency( fmo.wp.frqidx + diff, mode );

	Comment( DEBUG, "Set_Combine_Frequency ", (int)diff);
}

void Oscgroup_class::Set_Duration( const uint& msec )
{
	uint duration = check_range( duration_range, msec, "Set_Duration");
	std::ranges::for_each( member, [ &duration ](Oscillator*  o)
			{ o->Setwp_frames( duration );});
}

void Oscgroup_class::Set_Osc_Note( 	const frq_t&	base_freq,
									const uint8_t& 	key,
									const uint& 	msec,
									const uint& 	volume,
									const uint& 	mode)
{
	Set_Duration		( msec );
//	SetAdsr				();
	Set_Note_Frequency	( base_freq, key, mode );
	osc.Set_volume		( volume, FIXED);//wp.volume	= volume ;
}

void Oscgroup_class::Phase_Reset()
{
	std::ranges::for_each( member, [](Oscillator*  o)
			{ o->Phase_reset() ;});
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
void Oscgroup_class::Set_Connections( interface_t* sds )
{
	std::ranges::for_each( member, [ this, sds](Oscillator*  o)
	{
		if ( sds->connect[ o->typeId ].frq )
			o->Connect_frq_data( member[ osc_struct::FMOID ] );
		if ( sds->connect[ o->typeId ].vol )
			o->Connect_vol_data( member[ osc_struct::VCOID ] );
	} );
}

void Oscgroup_class::Run_OSCs( const buffer_t& offs )
{
	std::ranges::for_each( member, [ &offs ](Oscillator* o)
			{ o->OSC( offs ) ;} );
}


Oscillator* Oscgroup_class::Get_osc_by_name( const string& name )
{
	Oscillator* ret = nullptr;

	std::ranges::for_each( member, [ name, &ret ](Oscillator* o)
			{ if ( strEqual( o->osctype_name, name ) )
				{
					ret = o;
				}
			} );
	if ( strEqual( "MAIN", name  ) ) // compatibility
		ret = member[ osc_struct::OSCID ];
	if ( ret == nullptr )
		{ EXCEPTION( "unknown Oscillator name: " + name ); }

	return ret;
}

void Oscgroup_class::selfTest()
{
	ASSERTION( fmo.spectrum.osc == osc_struct::FMOID, "fmo.spectrum.osc",
				(int) fmo.spectrum.osc, (int) osc_struct::FMOID );

	adsr_t adsr = fmo.Get_adsr();
	ASSERTION( adsr.spec.adsr == true, "fmo.adsr_data.spec.adsr",
				(int) adsr.spec.adsr, 1 );
}
