/*
 * Appstate.h
 *
 *  Created on: May 4, 2025
 *      Author: sirius
 */

#ifndef VIEWINTERFACE_H_
#define VIEWINTERFACE_H_

#include <data/Interface.h>
#include <data/Appstate.h>

enum  { ESC=27, ASC=126, F1=239, F2=241, F3=243, F4=245, F5=53, F6=55 };

class ViewInterface_class :
	osc_struct,
	interface_struct,
	virtual Frequency_class,
	Spectrum_class,
	state_struct
{
	string 				className 	= "";
	Interface_class*	Sds_p 		= nullptr;
	interface_t* 		sds 		= nullptr;

public:
	Appstate_class		Appstate	;

	ViewInterface_class( char appid, Interface_class* Sds, Register_class* reg )
	:
		Frequency_class(),
		Spectrum_class(),
		state_struct(),
		Appstate( appid, nullptr, Sds->addr, reg )
	{
		className = Logfacility_class::className;
		Sds_p			= Sds;
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





#endif /* VIEWINTERFACE_H_ */
