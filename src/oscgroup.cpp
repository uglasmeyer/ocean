/*
 * Oscgroup.cpp
 *
 *  Created on: Jan 22, 2025
 *      Author: sirius
 */

#include <Oscgroup.h>
#include <data/Device.h>

Oscgroup_class::Oscgroup_class( OscroleId_t role, buffer_t bytes )
	: Logfacility_class( "Oscgroup_class" )
	, Frequency_class()
	, Note_base()
	, vco( role, VCOID, bytes )
	, fmo( role, FMOID, bytes )
	, osc( role, OSCID, bytes )
{
	className		= Logfacility_class::className;
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

Oscgroup_class::~Oscgroup_class()
{
	DESTRUCTOR( className )
}

void Oscgroup_class::SetSpectrum( interface_t* sds )
{
	for( char oscid : oscIds )
	{
		member[oscid]->Set_spectrum	( sds->spectrum_arr[ oscid ]);
	}
}

void Oscgroup_class::SetInstrument( interface_t* sds )
{

	SetAdsr			( sds );
	SetFeatures		( sds );
	Set_Connections	( sds );
	SetSpectrum		( sds );
}

void Oscgroup_class::Connection_Reset()
{
	std::ranges::for_each( member, [](Oscillator*  o)
			{ o->Connection_reset() ;});
}
void Oscgroup_class::Set_Connections( interface_t* sds )
{
	// OSC <- SEC
	for( Oscillator* osc_p : member )
	{
		connectId_t Connect = sds->connect_arr[osc_p->typeId];
		osc_p->Set_connection( member[ Connect.frq ], member[ Connect.vol] );
	}
}

void Oscgroup_class::SetFeatures( interface_t* sds )
{
	for( char oscid : oscIds )
	{
		member[ oscid ]->features = sds->features[ oscid ];
	}
}
void Oscgroup_class::SetAdsr( interface_t* sds )
{
	for( char oscid : oscIds )
	{
		member[oscid]->Set_adsr	( sds->adsr_arr[oscid] );
	}
}

void Oscgroup_class::SetSlide( const uint8_t& value )
{
	std::ranges::for_each( member, [ value ](Oscillator*  o)
			{ o->Set_glide( value);	});
}

void Oscgroup_class::SetWd( Wavedisplay_class* Wd )
{
	std::ranges::for_each( member, [ this, Wd ](Oscillator*  osc )
			{ Wd->Add_data_ptr( osc->typeId,
								osc->roleId,
								osc->MemData_p(),
								&osc->wp.frames ); });
	if( osc.roleId == INSTRROLE )
	{
		std::ranges::for_each( member, [ this, Wd ](Oscillator*  osc )
				{ Wd->Add_data_ptr( osc->typeId,
									ADSRROLE,
									osc->AdsrMemData_p(),
									&osc->adsr_frames ); });
		}
	}

void Oscgroup_class::SetScanner( const buffer_t& maxlen )
{
	std::ranges::for_each( member, [ maxlen ](Oscillator*  o)
	{
		o->scanner.Set_wrt_len( max_frames );
		o->scanner.Set_fillrange( maxlen );
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

void Oscgroup_class::Set_Note_Frequency	( 	interface_t*	sds,
											const uint8_t& idx,
											const uint& mode )
{
	int diff = idx - sds->spectrum_arr[OSCID].frqidx[0];
	osc.Set_frequency( idx, mode );

	vco.Set_frequency( sds->spectrum_arr[VCOID].frqidx[0] + diff, mode );
	fmo.Set_frequency( sds->spectrum_arr[FMOID].frqidx[0] + diff, mode );
}

void Oscgroup_class::Set_Combine_Frequency( const uint8_t& base_idx,
											const uint8_t& idx,
											const uint& mode )
{
	int diff = idx - base_idx;
	osc.Set_frequency( idx, mode );

	vco.Set_frequency( vco.spectrum.frqidx[0] + diff, mode );
	fmo.Set_frequency( fmo.spectrum.frqidx[0] + diff, mode );

	Comment( DEBUG, "Set_Combine_Frequency ", (int)diff);
}

void Oscgroup_class::Set_Duration( const uint& msec )
{
	uint duration = check_range( duration_range, msec, "Set_Duration");
	std::ranges::for_each( member, [ &duration ](Oscillator*  o)
			{ o->Setwp_frames( duration );});
}

void Oscgroup_class::Set_Osc_Note(  interface_t*	sds,
									const uint8_t& 	key,
									const uint& 	msec,
									const uint& 	volume,
									const uint& 	mode)
{
	Set_Duration		( msec );
	Set_Note_Frequency	( sds, key, mode );
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

void Oscgroup_class::Run_OSCs( const buffer_t& offs )
{
	std::ranges::for_each( member, [ &offs ](Oscillator* o)
			{ o->OSC( offs ) ;} );
}

OSCID_e Oscgroup_class::Get_oscid_by_name( const string& name )
{
	for( OSCID_e id : oscIds )
	{
		if ( strEqual( typeNames[id], name ) )
			return id;
	}
	Exception( "unknown Oscillator name: " + name );
	return NOOSCID;
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
		ret = member[ OSCID ];
	if ( ret == nullptr )
		{ Exception( "unknown Oscillator name: " + name ); }

	return ret;
}

void Oscgroup_class::selfTest()
{
	ASSERTION( fmo.spectrum.osc == FMOID, "fmo.spectrum.osc",
				(int) fmo.spectrum.osc, (int) FMOID );

	adsr_t adsr = fmo.Get_adsr();
	ASSERTION( adsr.spec.adsr == true, "fmo.adsr_data.spec.adsr",
				(int) adsr.spec.adsr, 1 );
}
