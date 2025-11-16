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
