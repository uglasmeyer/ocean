/*
 * Exit.h
 *
 *  Created on: Oct 29, 2024
 *      Author: sirius
 */

#ifndef EXIT_H_
#define EXIT_H_

#include <Ocean.h>
#include <Logfacility.h>

extern void exit_proc( int );

class Exit_class :
	public virtual Logfacility_class
{
	const vector<uint> 	signal_vec = {  SIGINT,   SIGHUP,   SIGABRT };
	const vector<string>signal_str = { "SIGINT", "SIGHUP", "SIGABRT" };
	string className = "";
public:

	Exit_class( ) :
		Logfacility_class("Exit_class")
	{
		this->className = Logfacility_class::module;
		catch_signals();
	};
	virtual ~Exit_class()
	{};

private:
	void catch_signals( )
	{
		uint idx = 0;
		for ( uint sig : signal_vec )
		{
			string sig_str = signal_str[ idx ];
			Comment(INFO, "Catching signal: " + sig_str );
			signal( sig	, &exit_proc );
			idx++;
		}
	}

};


#endif /* EXIT_H_ */
