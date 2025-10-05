/*
 * variantion.h
 *
 *  Created on: Apr 30, 2024
 *      Author: sirius
 */

#ifndef VARIATION_H_
#define VARIATION_H_

#include <Mixer.h>
#include <notes/Notes.h>
#include <Ocean.h>


class Charset_class //: virtual public string
{
	// a charset A is a unique string of chars
	string		 SetStr	{};
	vector<char> uSet 	{}; // unique set of SetStr chars
public:
	vector<char> Set	{};

	Charset_class( string set )
	{
		SetStr = set;	// the origin of the string
		std::copy( set.begin(),set.end(), back_inserter( Set ));
		std::sort(Set.begin(),Set.end());
		std::unique_copy(set.begin(), set.end(), back_inserter(uSet));
	};

	~Charset_class(){};

	// https://en.cppreference.com/w/cpp/language/operators
    friend Charset_class operator+(const Charset_class& lhs,
                       	   	   	   const Charset_class& rhs)
    {
    	Charset_class C{""};
    	std::set_union(	lhs.Set.begin(), lhs.Set.end(),
    					rhs.Set.begin(), rhs.Set.end(), back_inserter(C.Set));
    	return C ; // return the result by value (uses move constructor)
    }

    friend Charset_class operator-(const Charset_class& lhs,
                       	   	   	   const Charset_class& rhs)
    {
    	Charset_class C{""};
    	std::set_difference(lhs.Set.begin(), lhs.Set.end(),
							rhs.Set.begin(), rhs.Set.end(), back_inserter( C.Set ));
        return C ;
    }

    friend Charset_class operator/(const Charset_class& lhs,
                       	   	   	   const Charset_class& rhs)
    {
    	Charset_class C{""};
    	std::set_intersection( 	lhs.Set.begin(), lhs.Set.end(),
								rhs.Set.begin(), rhs.Set.end(),	back_inserter( C.Set ));
        return C ;
    }

    Charset_class& operator+=(const char& ch )
    {

    	Set.push_back( ch );
		std::sort(Set.begin(), Set.end());

    	return *this; // return the result by reference
    }

    friend bool operator==(const Charset_class& A, const Charset_class& B )
	{
    	Charset_class E = A - B;
    	return ( E.Set.size() == 0 );
	}

    bool contains ( char ch )
    {
    	vector<char> c{ch};
    	return std::includes( Set.begin(), Set.end(), c.begin(), c.end() );
    }



    string Str()
    {
    	string str;
    	std::for_each( Set.begin(), Set.end(), [&str]( auto element ){	str.push_back(element) ;});
    	assert( str.data() != nullptr );
    	return str;
    }

    void show()
    {
    	std::for_each( Set.begin(), Set.end(), []( auto element ){ cout << element ;});
    	cout << endl;
    }

    void test()
    {
    	Charset_class A( "abcdacbf");
    	Charset_class B( "12341bcg");
    	const Charset_class E{""};

    	Charset_class C = A - A;
    	C.show();
    	assert ( C.Set.size() == 0 );
    	assert ( A.contains( 'b' ));
    	assert ( not A.contains( 'g' ));

    	C = A + B ;
    	Charset_class D{"abcdf1234g"};
    	Charset_class F{""};
    	F = D - C;
    	C.show();
    	assert( E == F );

    	C.show();
    	C += 'a';
    	C.show();
    	D.show();
    	F = D - C;
    	assert( E == F );

    	C = A - B;
    	C = C - A;
    	C.show();
    	B.show();
    	assert( C == E );

    	C = A / B;
    	C.show();

    }

};

class Variation_class :
		virtual public Logfacility_class,
		virtual public Note_class
{
	string className = "";
public:

	Variation_class() ;
	~Variation_class(){};

	typedef vector<note_t>
				noteword_t;
	typedef vector<noteword_t>
				notesentence_t;

	void 		Set_note_chars( uint );
	void 		Set_note_chars( uint, uint );
	void 		Define_fix( string  );
	void 		Define_variable( string  );
	void 		Define_rhythm  ( string );
	string 		Gen_noteline( string pattern, string filename );
	void 		Test();

private:
	noteline_prefix_struct
				Nlp_variation	= noteline_prefix_default;

	string Constant_chars{""};
	string Rhythm_chars{""};
	random_device rd;

	noteword_t 		Random_Notes{};
	notesentence_t 	Sentence{};

	noteword_t 	list2vector( notelist_t );
	string 		insert_random(  );
	string 		input_filter( string , set<char> );
	void 		define_random_note_vector( string str );
	noteword_t 	gen_random_note_word();
	void 		randomize_notes_octave( string );
	noteword_t 	str2words( string note_str );
	void 		swap_word( int i, int j );
	void		reverse_word( int i );
	void 		set_octave( int, noteword_t& );
	string 		scan_sentence( char );

	template<typename T>
	auto gen_random_note( T random_set)
	{
		mt19937 engine(rd());
		std::shuffle( random_set.begin(), random_set.end(),  engine );
		return random_set[0];
	}


};



#endif /* VARIATION_H_ */
