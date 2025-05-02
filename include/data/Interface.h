/*
 * GUIinterface.h
 *
 *  Created on: Jan 14, 2024
 *      Author: sirius
 */


#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

#include <Ocean.h>
#include <Config.h>
#include <Spectrum.h>
#include <Logfacility.h>
#include <Version.h>
#include <data/Semaphore.h>
#include <data/SharedDataSegment.h>
#include <data/Register.h>
#include <data/Memory.h>
#include <EventKeys.h>
#include <data/EventQue.h>




class Interface_class :
		virtual public Logfacility_class,
		state_struct
{

public:

	Frequency_class 		Frequency 		{};
	Spectrum_class			Spectrum 		{};

	interface_t 			ifd_data 	= interface_struct();
	Shm_base				SHM			{ sizeof( ifd_data ) };
	interface_t* 			addr		= nullptr;
	shm_ds_t				ds			= shm_data_struct();
	Semaphore_class*		Sem_p		= nullptr;
	Config_class*			Cfg_p		= nullptr;
	EventQue_class			Eventque	{};
	char					AppId		= NOID;
	bool					capture_flag= false;
	string					dumpFile	= "";

	Interface_class( char appid, uint8_t sdsid,  Config_class*, Semaphore_class* );
	virtual ~Interface_class();

	void	Setup_SDS( uint sdsid, key_t key );
	void 	Write_arr( const wd_arr_t& arr );
	void 	Write_str( char, string );
	string 	Read_str( char );
	void 	Commit();
	void	Dump_ifd();
	bool 	Restore_ifd();
	void 	Reset_ifd( );
	bool reject(char status, int id )
	{
		if (( status == RUNNING ) and ( id != COMPID ))
		{
/*			if( previous_status != status )
				cout << "Observer mode ON" << endl;
			previous_status = status;
*/
			return true;
		}
		else
		{
			return false;
		}
	};

	template < typename V >
	void Set( V& ref, V value )
	{
		if ( reject( addr->Composer, AppId ) ) return;
		ref = value;
	};
	void Event( uint8_t event )
	{
		Eventque.add( event );
	}
	void Test_interface();


private:
	size_t			sds_size		= sizeof( ifd_data );
	char 			previous_status = OFFLINE;


//	bool 	reject(char status, int id );


};



enum  { ESC=27, ASC=126, F1=239, F2=241, F3=243, F4=245, F5=53, F6=55 };

class ViewInterface_class :
	osc_struct,
	interface_struct, //Interface_class,
	virtual Frequency_class,
	Spectrum_class,
	state_struct
{
	string 				className 	= "";
	Interface_class*	Sds_p 		= nullptr;
	interface_t* 		sds 		= nullptr;
public:
	ViewInterface_class( char appid, Config_class* cfg, Semaphore_class* sem, Interface_class* sds_p )
	:
	  Frequency_class(),
	  Spectrum_class(),
	  state_struct()
	{
		className = Logfacility_class::className;
		Sds_p			= sds_p;
	};
	~ViewInterface_class() = default;


	void	ShowPage( interface_t* sds, int nr );

private:

	void 	showOSCs();
	void 	showProcesses();
	void 	showStates() ;

	void 	show_Que();
	void 	show_Adsr();
	void 	showKeys();


	void	printHeader();
	string	Decode( uint8_t idx);
};


#endif /* GUIINTERFACE_H_ */
