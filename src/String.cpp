/*
 * String.cpp
 *
 *  Created on: Mar 3, 2024
 *      Author: sirius
 */

#include "String.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <assert.h>






vector_str_t String::to_array( char ch )
{

	stringstream ss( Str );
    string substr;

    arr.clear();
	while( ss.good() )
	{
	    getline( ss, substr, ch );
	    arr.push_back(substr);
	}
	if ( arr.size() == 0 )
		arr[0] = Str;
	return arr;

}

vector_str_t String::to_unique_array( char ch )
{
	Str.append(1,ch);
	stringstream ss( Str );
    string substr;

    arr.clear();
	while( ss.good() )
	{
	    getline( ss, substr, ch );
	    if ( substr.length() > 0 )
	    	arr.push_back(substr);
	}
	if ( ( arr.size() == 0 ) )
    	arr.push_back( "" );
	return arr;
	;
}

int count_char( char ch, string s )
{
	int i = 0;
	for( char c : s ) if ( c == ch ) i++;
	return i;
}

vector_str_t String::to_bracket_array( char ch ) // a "b c" d -> a,bc,d
{

	replace_char('\t', ' ');
	to_unique_array(' ');
	int count = count_char( ch, Str );
	if ( count == 0 ) return arr;
	if (( count % 2 ) != 0 )
	{
		string err = "missing ";
		err.push_back(ch);
		Comment( ERROR, err );
		if ( TEST )
		{
			arr[0] = err;
			return arr;
		}
		exit(1);
	}
	size_t pos 		= 0;
	vector<string> 	result{};
	string sub 		{""};

	while( pos != arr.size() )
	{
		string arrpos = arr[ pos ];
		if ( arrpos[0] == ch )
		{
			sub = arrpos.substr(1,string::npos);
			while( ( arrpos.back() != ch ) and ( pos != arr.size()-1 ) )
			{
				pos++;
				arrpos	= arr[pos];
				sub.append(arrpos);
			}
			sub = sub.substr(0, sub.length()-1);
		}
		else
			sub = arrpos;

		result.push_back(sub);
		pos++;
	}
	return result;
}

void String::replace_comment(  )
{
	int pos = Str.find("#");
	Str = Str.substr( 0, pos );

}

void String::replace_char( char ch_old, char ch_new )
{
	std::for_each( Str.begin(), Str.end(), [ch_old, ch_new] ( char& ch )
			{ if ( ch == ch_old ) ch = ch_new;} );
}

void String::normalize()
{
	replace_comment();
	remove( Str.begin(), Str.end(), ' ' );
	remove( Str.begin(), Str.end(), '\t' );

}
bool String::is_number(  )
{
	try
	{
		stoi( this->Str);
	}
	catch (const std::invalid_argument& exception )
	{
		cout << exception.what() << endl;
		return false;
	}
	return true;
}

int String::secure_stoi( string str)
{
	this->Str = str;
	if ( is_number(  ) )
	{
		return stoi( str );
	}
	else
	{
		Comment( ERROR, "cannot convert string: >"+str+"< to integer " );
		exit(1);
	}
}


void String::to_lower()
{
	std::for_each( Str.begin(), Str.end(), [] ( char& ch ) { ch = tolower(ch) ; } );
}

string String::to_hex( long value )
{
	stringstream ss;
	ss << "0x" << uppercase << hex << value ;
	return ss.str();
}

void String::test()
{
	Set_Loglevel( TEST, true);
	Comment( TEST, "Test String start" );

	string str 		= "a=abc=def";
	String 			S{""};
	S				= str;
	vector_str_t arr = S.to_array('=');

	assert( arr[0].compare("a")					== 0 );
	assert( arr[1].compare("abc") 				== 0 );
	assert( arr[2].compare("def") 				== 0 );

	arr 			= S.to_array(':');
	assert( arr[0].compare(str) 				== 0 );
	arr 			= S.to_unique_array(':');
	assert( arr[0].compare(str)					== 0 );

	assert( to_hex(0xFFFF).compare( "0xFFFF" ) 	== 0 );

	S 	= " test \"two	 words\" \"three words 123 \" end";
	arr = S.to_bracket_array( '\"');
//	for ( string s : arr ) cout << s << endl;
	assert( arr[0].compare("test") 					== 0 );
	assert( arr[1].compare("twowords") 				== 0 );
	assert( arr[2].compare("threewords123") 			== 0 );
	assert( arr[3].compare("end") 					== 0 );

	S 	= " test a \"two words\" \"three words bc  d";
	arr = S.to_bracket_array( '\"');
	assert( arr[0].compare("missing \"") == 0 );
//	for ( string s : arr ) cout << s << endl;

	S 				= "a:b:c:d";
	arr 			= S.to_array(':');
	assert( arr.size() 							== 4 );

	S				= "test0  #	  comment # test";
	S.normalize();
	cout << "\'" << S.Str << "\'" << endl;
	assert( S.Str.compare("test0  ")				== 0 );

	S				= "a  b  this-2 d             this ";
	arr 			= S.to_unique_array(' ');
	assert( arr[4].compare("this")				== 0 );
	arr 			= S.to_array(' ');
	cout << arr[4] << endl;
	assert( arr[4].compare("this-2")			== 0 );

	S				= "a  b  c  d  c ";
	S.replace_char(' ', 'x');
	assert( S.Str.compare("axxbxxcxxdxxcx") == 0 );

	S 				= "start 	Synthesizer 10000 3";
	S.replace_char('\t' , ' ');
	arr = S.to_unique_array(' ');
	assert( arr[1].compare("Synthesizer") == 0 );


	String St( "this is a test" );
	assert( St.Str.compare("this is a test") 	== 0 );

	String A{"Equal"};
	String B{"Equal"};
	String C{"not equal"};
	cout << A.Str << " " << B.Str << endl;
	assert( A == B );

	A = "UP_PER";
	A.to_lower();
	assert( A.Str.compare("up_per") == 0 );

	Comment( TEST, "Test String OK" );
//	assert(false);

}


