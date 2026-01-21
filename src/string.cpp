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
 * String.cpp
 *
 *  Created on: Mar 3, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#include <String.h>
#include <System.h>



bool fcomp( const float& a, const float& b, float precision )
{
	return ( abs(abs(a) - abs(b)) <= abs(precision) ) ? true : false;
}

bool strEqual( const string& a, const string& b )
{
	return ( a.compare( b ) == 0 );
}


string notnull( char* cstr )
{
	const char* empty = "";
	const char* pstr = cstr;
	if( ! pstr )
		pstr = empty;
	return string( pstr );

}

int char2int( const char& ch )
{
	return (int)( ch - 48 );
}
char int2char( const int& i )
{
	return (char)( i + 48 );
}

set<int> range_set( const int& min, const int& max )
{
	set<int> intset {};
	if ( min > max ) return intset;
	for ( int n = min; n < max + 1; n++ )
	{
		intset.insert( n );
	}
	return intset;
}




vector_str_t String::to_array( char ch )
{

	stringstream ss( Str );
    string substr;

    Vec.clear();
	while( ss.good() )
	{
	    getline( ss, substr, ch ); // provides null-terminate substr.data()
	    Vec.push_back(substr);
	}
	if ( Vec.size() == 0 )
		Vec[0] = Str;
	return Vec;

}

set<char> String::to_set()
{
	for ( char ch : Str )
	{
		Set.insert(ch);
	}
	return Set;
}
vector_str_t String::to_unique_array( char ch )
{
	Str.append(1,ch);
	stringstream ss( Str );
    string substr;

    Vec.clear();
	while( ss.good() )
	{
	    getline( ss, substr, ch );
	    if ( substr.length() > 0 )
	    	Vec.push_back(substr);
	}
	if ( ( Vec.size() == 0 ) )
    	Vec.push_back( "" );
	return Vec;
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
	if ( count == 0 ) return Vec;
	if (( count % 2 ) != 0 )
	{
		string err = "missing ";
		err.push_back(ch);

		if ( LogMask.test( TEST ) )
		{
			Vec[0] = err;
			return Vec;
		}
		else
		{
			{ Exception( err ); }
		}
	}
	size_t pos 		= 0;
	vector<string> 	result{};
	string sub 		{""};

	while( pos != Vec.size() )
	{
		string arrpos = Vec[ pos ];
		if ( arrpos[0] == ch )
		{
			sub = arrpos.substr(1, STRINGNOTFOUND );
			while( ( arrpos.back() != ch ) and ( pos != Vec.size()-1 ) )
			{
				pos++;
				arrpos	= Vec[pos];
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
	std::ranges::for_each( Str, [ch_old, ch_new] ( char& ch )
			{ if ( ch == ch_old ) ch = ch_new;} );
}

void String::normalize()
{
	auto erase_char = [ this ]( char del )
		{
			string ret {""};
			for( char ch : Str )
				if ( del != ch ) ret.push_back( ch );
			return ret;
		};

	replace_comment();
	Str = erase_char( ' ' );
	Str = erase_char( '\t');

}

bool String::is_number(  )
{
	try
	{
		stoi( this->Str);
		return true;
	}
	catch (const std::invalid_argument& exception )
	{
		try
		{
			stof( this->Str );
			return true;
		}
		catch ( const std::invalid_argument& exception )
		{
			Comment(ERROR, exception.what());
			return false;
		}
	}
	return false;
}

float String::secure_stof( string str)
{
	if ( str.length() == 0 )
		return 0;
	this->Str = str;
	if ( is_number(  ) )
	{
		return stof( str );
	}
	else
	{
		Exception( "cannot convert string: >"+str+"< to integer " );
	}
	return -1;
}

int String::secure_stoi( string str )
{
	if ( str.length() == 0 )
		return 0;
	this->Str = str;
	if ( is_number(  ) )
	{
		return stoi( str.data() );
	}
	else
	{
		Exception( "cannot convert string: >"+str+"< to integer " );
	}
	return -1;
}

void String::to_lower( string& str )
{
	std::for_each( str.begin(), str.end(), [] ( char& ch ) { ch = tolower(ch) ; } );
}
string String::to_lower()
{
	std::for_each( Str.begin(), Str.end(), [] ( char& ch ) { ch = tolower(ch) ; } );
	return Str;
}

string String::to_hex( long value )
{
	stringstream ss;
	ss << "0x" << uppercase << hex << value ;
	return ss.str();
}

int String::to_int( const string& s )
{
	int i;
	Str = s;
	to_lower();
	size_t pos = s.find( "0x");
	if( pos == STRINGNOTFOUND )
	{
		stringstream ss( s );
		ss >> dec >> i;
	}
	else
	{
		string sub = s.substr( pos+2, s.length()-1 );
		stringstream ss( sub );
		ss >> hex >> i;
	}
	return i;
}

void String::Show( const string& comment )
{
	cout << comment << ": " << Str << endl;
}


void String::TestString()
{
	TEST_START( "String" );

	map<int, string> testmap = // @suppress("Invalid arguments")
	{
			{ 0, string("zero") },
			{ 1, string("one") }
	};
	ASSERTION( strEqual( testmap[1], "one" ), "testmap", testmap[1], "one" );

	map<string, int> testimap = imap( testmap );
	ASSERTION( testimap["zero"] == 0 , "testimap", testimap["zero"], 0L );

	String A{""};
	String B{""};
	A = "1234";
	B = "1234";
	assert( A == B );
	int i = A.to_int( "   0x1234  " );
	assert( i == 0x1234 );
	i = A.to_int( "  1234 " );
	assert( i == 1234 );
	i = A.to_int( "abcd");
	assert( i == 0 );

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
	for ( string s : arr ) cout << s << endl;
	assert( arr[0].compare("test") 					== 0 );
	assert( arr[1].compare("twowords") 				== 0 );
	assert( arr[2].compare("threewords123") 		== 0 );
	assert( arr[3].compare("end") 					== 0 );

	Str 	= " test a \"two words\" \"three words bc  d";
	arr = to_bracket_array( '\"');
	cout << arr[0] << endl;
	assert( arr[0].compare("missing \"") == 0 );
//	for ( string s : arr ) cout << s << endl;

	S 				= "a:b:c:d";
	arr 			= S.to_array(':');
	assert( arr.size() 							== 4 );

	S				= "test0  	#	  comment # test";
	S.normalize();
	cout << ">" << S.Str << "<" << endl;
	assert( S.Str.compare("test0")				== 0 );

	S				= "a  b  this-2 d             this ";
	arr 			= S.to_unique_array(' ');
	assert( arr[4].compare("this")				== 0 );
	arr 			= S.to_array(' ');
	cout << arr[4] << endl;
	assert( arr[4].compare("this-2")			== 0 );

	S				= "a  b  c  d  c ";
	S.replace_char(' ', 'x');
	assert( S == String( "axxbxxcxxdxxcx") );

	S 				= "start 	Synthesizer 10000 3";
	S.replace_char('\t' , ' ');
	arr = S.to_unique_array(' ');
	assert( String( arr[1] ) == String("Synthesizer") );

	S 				= "1,2,3,4,5";
	arr = S.to_array(',');
	ASSERTION( arr.size() == 5, "arr size", arr.size(), 5 );

	String St( "this is a test" );
	assert( St == String("this is a test") 	);

	A = "Equal";
	B = "Equal" ;
	String C{"not equal"};
	cout << A.Str << " " << B.Str << endl;
	assert( A == B );
	assert( not ( C == B ));

	A = "UP_PER";
	A.to_lower();
	B = "up_per";
	assert( A == B );

	set<int> s {};
	vector<int> v = { 1,3,5,7,9 };
	s = vector2set(v);
	assert( s.contains(5) ) ;
	assert( not s.contains(6) ) ;

	set<char> charset = { 'a','n','b','v' };
	assert( charset.contains( 'b' ));
	set<string> Ss {};
	vector<string> V = {"ab", "cd", "ef", "gh" };
	Ss = vector2set( V );
	assert( Ss.contains("cd"));
	assert( not Ss.contains("de"));

	list<int> l { 1,2,3 };
	S = show_items( l );
	S.Show( "Items" );

	list<string> L2 { "a", "b", "c"};
	show_type( L2 );

	String Z { "abcdef" };
	assert( Z.Set.contains('f'));

	set<int> numbers = range_set(0,10);
	ASSERTION( numbers.contains( 5 )		, "range 0..10", 5, "true" );
	ASSERTION( (not numbers.contains( -3 ))	, "range 0..10", -3, "false" );

	ASSERTION(  secure_stoi( "-10" ),"secure_stof", secure_stoi( "-10" ), -10 );
	bool b = (abs(secure_stof( "-10.05" )) - 10.05) < 1E-2;
	ASSERTION(  ( b ),"secure_stof", secure_stof( "-10.05" ), -10.05 );

	Assert_equal( char2int('0'), 0 );
	Assert_equal( char2int('2'), 2 );

	TEST_END( "String" );

}


