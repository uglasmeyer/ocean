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
 * Menu.h
 *
 *  Created on: Oct 26, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef SDSMENU_H_
#define SDSMENU_H_

#include <Viewinterface.h>
#include <Kbd.h>

class Menue_class :
	virtual public Logfacility_class
{
	string 				className 	= "";
	ViewInterface_class ViewSds ;
	Dataworld_class*	DaTA;
	interface_t*		sds;
	interface_t*		sds_master;
	Id_t 				sdsid;
	KEYCODE 			page 		= KEYCODE::F1;

	typedef function< kbdInt_t( kbdInt_t)> switch_fnc_t ;
	switch_fnc_t 		Switch;

public:
	Menue_class( Dataworld_class* data, switch_fnc_t _switch, string footer = "" ) ;
	virtual ~Menue_class();
	void Set_sdsid( int delta );
	void Loop();

private:

	kbdInt_t Key_event( );
};



#endif /* SDSMENU_H_ */
