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

#define ASSERTION(	 expr , message, input, expected )\
	if ( not (expr) ) \
	{\
	printf( "file: ( %s ) line: ( %d ) in function: ( %s )\n", __FILE__, __LINE__, __func__ );\
	cout 	<< message 							<< '\n'\
			<< "input    value: " << (input) 		<< '\n'\
			<< "expected value: " << (expected) 	<< endl;\
	raise( SIGINT ); \
	};

#define EXCEPTION( err_str )\
	{\
	cout << "An application exception occured " << endl;\
	cout << "See above the detail, or visit the Synthesizer.log file for more information" <<endl;\
	cout << "Press <Ctrl>d to enter the common exit procedure" << endl;\
	string s;\
	cin >> s;\
	raise( SIGINT );\
	};


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
			{ SIGHUP, "SIGHUP" },
			{ SIGABRT,"SIGABRT" },
			{ SIGTERM,"SIGTERM"}
	};

	string className = "";
public:


	Exit_class( ) :
		Logfacility_class("Exit_class")
	{
		this->className = Logfacility_class::module;
		catch_signals();
	};
	virtual ~Exit_class()
	{
		cout << "visited ~Exit_class" << endl;
	};

private:
	void catch_signals( )
	{
		for ( signalmap_t sig : signalmap )
		{
			Comment(INFO, "Catching signal: " + sig.name );
			signal( sig.signal	, &exit_proc );
		}
	}

};


#endif /* EXIT_H_ */
