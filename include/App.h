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
 * App.h
 *
 *  Created on: Aug 27, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef APP_H_
#define APP_H_

#include <data/Semaphore.h>
#include <data/Statistic.h>
#include <data/DataWorld.h>
#include <Kbd.h>



// https://en.cppreference.com/w/cpp/language/parameter_pack

class Application_class :
	virtual Logfacility_class,
	public virtual Interface_base,
	public sdsstate_struct,
	Statistic_class
{
    const set<APPID>	logowner 			=  { GUI_ID, COMPID, RTSPID };
	string 				This_Application 	;
	string 				ProgramName 		;

public:

	APPID 				AppId				;
	process_t 			properties 			{};
	Time_class			Timer				{};
	Kbd_base			Kbd					{};
	Appstate_class*		Appstate			;
	Config_class*		Cfg 				;

    					Application_class	( Dataworld_class* );
	virtual 			~Application_class	();

	void 				Start				( int, char* [] );
	void 				Ready				();
	kbdInt_t			KeyboardKey			( bool debug);


private:
	bool 				redirect_stderr 	= false;
	void 				app_properties		();
	void 				deRegister			();
	void 				versionTxt			();

};



#endif /* APP_H_ */
