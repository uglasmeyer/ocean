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
 * String.h
 *
 *  Created on: Mar 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef STRING_H_
#define STRING_H_


#include <Ocean.h>
#include <Logfacility.h>


typedef vector<string>	vector_str_t;
const size_t STRINGNOTFOUND = string::npos;

extern int 		char2int( const char& ch );
extern char 	int2char( const int& i );
extern set<int> range_set( const int& min, const int& max );
extern bool 	strEqual( const string& , const string&  );
extern string 	notnull( char* cstr );

inline string bool_str( bool b, string t="yes", string f="no" )
	{ return ( b ) ? t : f; };

template<typename Key, typename Value>
auto imap( map<Key,Value> m )
{
	map<Value, Key> M {};
	for( auto [k,v] : m )
	{
		M[v] = k;
	}
	return M;
}
template< typename C > vector<C> set2vector( set<C> s )
{
	vector<C> v {};
	for( auto i : s )
	{
		v.push_back( i );
	}
	return v;
}

template< class C > set< C > vector2set( vector< C > v )
{
	set< C > s {};
	for( C i : v )
	{
		s.insert( i );
	}
	return s;
}

template< class C > list< C > vector2list( vector< C > v )
{
	list< C > l {};
	for( C i : v )
	{
		l.insert( i );
	}
	return l;
}
template< class C > vector< C > list2vector( list< C > l )
{
	vector< C > v {};
	for( C c : l )
	{
		v.push_back( c );
	}
	return v;

}

template<class C> string show_str_items( C all_items )
{
	stringstream strs{ "" };
    for ( auto item : all_items )
    {
   		strs << item << " ";
    }
    return strs.str();
}

template<class C> string show_items( C all_items )
{
	stringstream strs{ "" };
    for ( auto item : all_items )
    {
		int number = (int)item;
   		strs << dec << number << " ";
    }
    return strs.str();
}

template<typename T> string show_type( T all_items )
{
	stringstream strs{ "" };
    for ( auto item : all_items )
    {
    	strs << item << " ";
    }
    return strs.str();
}

template < typename T>
bool is_string( T v )
{
	return (typeid( v )  == typeid(string())) ? true : false ;
}

class String :
		virtual public Logfacility_class
{
	string 			className 	= "";
public:
	string 			Str			{};
	vector<string> 	Vec			{};
	set<char>		Set			{};

	String( const string str="" ) :
		Logfacility_class( "String_class" )
	{
		className = Logfacility_class::className;
		this->Str = str;
        this->Set = to_set( );
        std::ranges::for_each( str, [ this ]( char ch){ Vec.push_back( string{ch} ) ;});
	}
	~String(){};

	void operator=(const string& str)
    {
        this->Str=str;
        this->Set = to_set( );
    }
	void operator=(const char* p )
	{
		this->Str.assign( p );
		this->Set = to_set( );
	}

    friend bool operator==(const String& A, const String& B )
	{
    	return ( A.Str.compare( B.Str ) == 0 );
	}

    set<char>		to_set();
	vector_str_t 	to_array( char );
	vector_str_t 	to_unique_array( char );
	vector_str_t 	to_bracket_array( char );

	void 			replace_comment(  	);
	void 			replace_char( char , char  );
	void 			normalize(			);
	bool			is_number( 			);
	int 			secure_stoi( string	str );
	float 			secure_stof( string str );
	string 			to_hex( long )	;
	void 			TestString();
	string 			to_lower( );
	void 			to_lower( string& str );
	int 			to_int( const string& s );
	void			Show( const string& comment );

};

//----------------------------------------------------------------------


class Value
{
public:
	int 	val;
	char 	ch;
	string 	str;
	string	boolstr;
	Value( const int& val )
	{
        this->val		= (int) val;
        this->ch 	= (char) val;
        this->str	= to_string( val );
        this->boolstr		= ( val == 0 ) ? "OFF" : "ON";
	};
	~Value(){};

    void operator=(const int& val )
    {
        this->val		= (int) val;
        this->ch 	= (char) val;
        this->str	= to_string( val );
        this->boolstr		= ( val == 0 ) ? "OFF" : "ON";
    }


};

#endif /* STRING_H_ */
