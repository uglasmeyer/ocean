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

/*
template < typename I, typename W>
void ASSERTION (	bool expr,
				const string message,
				I input,
				W expected,
				source_location location = source_location::current() // @suppress("Function cannot be resolved") // @suppress("Type cannot be resolved")
			)
{
	if ( expr ) return;

	cout 	<< "file: "
			<< location.file_name() << '(' // @suppress("Method cannot be resolved")
			<< location.line() << ':' // @suppress("Method cannot be resolved")
			<< location.column() << ") `" // @suppress("Method cannot be resolved")
			<< location.function_name()  // @suppress("Method cannot be resolved")
			<< message 							<< '\n'
			<< "input    value: " << input 		<< '\n'
			<< "expected value: " << expected 	<< endl;

	raise( SIGTERM );
};// @suppress("Function cannot be instantiated")
*/
#define ASSERTION(	 expr , message, input, expected )\
	if ( not (expr) ) \
	{\
	printf( "file: ( %s ) line: ( %d ) in function: ( %s )\n", __FILE__, __LINE__, __func__ );\
	cout 	<< message 							<< '\n'\
			<< "input    value: " << (input) 		<< '\n'\
			<< "expected value: " << (expected) 	<< endl;\
	raise( SIGINT ); \
	};


extern void exit_proc( int );

class Exit_class :
	public virtual Logfacility_class
{
	const vector<uint> 	signal_vec = {  SIGINT,   SIGHUP,   SIGABRT,   SIGTERM };
	const vector<string>signal_str = { "SIGINT", "SIGHUP", "SIGABRT", "SIGTERM"};
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
