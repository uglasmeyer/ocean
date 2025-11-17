//@lic

/*  
    Ocean Sound Lab Application Suite (short Ocean-SL)
	Ocean-SL is a c++Project that provides a set of Sound Managing Applications,
	such as Synthesizer, Audioserver, Composer with Graphical UserInterface for
	Linux based Operating Systems, that allows to generate, play and record sound.

    Copyright (C) 2025  Ulrich.Glasmeyer@web.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

@lic*/


/*
 * Exit.h
 *
 *  Created on: Oct 29, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef EXIT_H_
#define EXIT_H_

#include <Logfacility.h>


#define NOEXEPT true


extern void exit_proc( int );

class Exit_class :
	public virtual Logfacility_class
{

	typedef struct signalmap_struct
	{
		int 	signal	= -1;
		string 	name	=  "";
	} signalmap_t;

	const vector<signalmap_t> signalmap =
	{
			{ SIGINT, "SIGINT" },
			{ SIGILL, "SIGILL" }, // used to interrupt the initialization phase of a process
			{ SIGHUP, "SIGHUP" },
//			{ SIGABRT,"SIGABRT" },
			{ SIGTERM,"SIGTERM"}
	};

	string className = "";
public:

	Exit_class( ) :
		Logfacility_class("Exit_class")
	{
		this->className = Logfacility_class::className;
		catch_signals();
	};
	virtual ~Exit_class()
	{
		DESTRUCTOR( className );
	};

private:
	void catch_signals( )
	{
		for ( signalmap_t sig : signalmap )
		{
			Comment( DEBUG, "Catching signal: " + sig.name );
			signal( sig.signal	, &exit_proc );
		}
	}

};


#endif /* EXIT_H_ */
