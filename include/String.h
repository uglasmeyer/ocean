/*
 * String.h
 *
 *  Created on: Mar 3, 2024
 *      Author: sirius
 */

#ifndef STRING_H_
#define STRING_H_



#include <Ocean.h>
#include <Logfacility.h>

typedef vector<string>	vector_str_t;
const size_t STRINGNOTFOUND = string::npos;

extern bool cmpstr( const string& , const string&  );
extern string notnull( char* cstr );


class String : virtual public Logfacility_class
{
public:
	string 			Str{""};
	vector_str_t 	arr{""};

	String( const string& str ) : Logfacility_class("String")
	{
		this->Str = str;
	}
	~String(){};


	void operator=(const string& str)
    {
        this->Str=str;
    }

    friend bool operator==(const String& A, const String& B )
	{
    	return ( A.Str.compare( B.Str ) == 0 );
	}

	vector_str_t 	to_array( char );
	vector_str_t 	to_unique_array( char );
	vector_str_t 	to_bracket_array( char );

	void 			replace_comment(  	);
	void 			replace_char( char , char  );
	void 			normalize(			);
	bool			is_number( 			);
	int 			secure_stoi( string	);
	string 			to_hex( long )	;
	void 			test();
	void 			to_lower( );

};

//----------------------------------------------------------------------


class Value
{
public:
	int 	i;
	char 	ch;
	string 	str;
	string	boolstr;
	Value( const int& val )
	{
        this->i		= (int) val;
        this->ch 	= (char) val;
        this->str	= to_string( val );
        this->boolstr		= ( i == 0 ) ? "OFF" : "ON";
	};
	~Value(){};

    void operator=(const int& val )
    {
        this->i		= (int) val;
        this->ch 	= (char) val;
        this->str	= to_string( val );
        this->boolstr		= ( i == 0 ) ? "OFF" : "ON";
    }


};

#endif /* STRING_H_ */
