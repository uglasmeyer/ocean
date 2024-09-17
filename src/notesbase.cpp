/*
 * Notenase.cpp
 *
 *  Created on: Jul 3, 2024
 *      Author: sirius
 */

#include <Common.h>
#include <Notesbase.h>

float Note_base::Calc_frequency( const uint8_t& key )
{
	if ( key > root2.vec.size()-1 )
	{
		cout.flush() << "limit: " << (int)root2.vec.size()-1 << endl;
		cout.flush() << "key  : " << (int)key << endl;
		Exception( "Octave key exceeds limit " );
	}
	return root2.vec[ key ] * oct_base_freq;
}

void Note_base::show_noteline_prefix( noteline_prefix_t nlp )
{
	stringstream strs{ "\n" };
	string nlp_str = convention_names[nlp.convention ];
	strs << "Note prefix:" << endl;
	strs << setw(20) << left << "Octave: " 			<< (int) nlp.Octave << endl;
	strs << setw(20) << left << "Convention: " 		<< (int) nlp.convention << ":" << nlp_str << endl;
	strs << setw(20) << left << "Notes per sec: " 	<< (int) nlp.nps << endl;
	strs << setw(20) << left << "Flats: " 			<< (int) nlp.flat << endl;
	strs << setw(20) << left << "Sharps: " 			<< (int) nlp.sharp << endl;
	Comment( INFO, strs.str() );
}

string Note_base::noteline_prefix_to_string( noteline_prefix_t nlp )
{
	stringstream strs{""};
	strs 	<< (int) nlp.Octave 		<<","
			<< (int) nlp.convention		<<","
			<< (int) nlp.nps 			<<","
			<< (int) nlp.flat			<<","
			<< (int) nlp.sharp ;
	return strs.str();
}

Note_base::noteline_prefix_t Note_base::string_to_noteline_prefix( string str )
{
	auto check_nps = [ this ]( int i )
		{
		for( int n : Notes_per_Sec )
			if ( n==i) return true;
		return false;
		};
	auto range_error = [ this ]( auto val, vector<size_t> range )
		{
			Comment ( ERROR, "Out of Range [" + to_string( range[0] ) + "," +
												to_string( range[1] ) + "}" );
			if ( Log[ TEST ] ) return;
			Exception( "Cannot assign noteline_prefix " +
					 to_string( val ) +
					" to noteline_structure" );//raise( SIGINT );
		};
	String S = str;
	noteline_prefix_t nlp;
	vector_str_t arr = S.to_unique_array(',');
//	show_vector( arr );
	if ( arr.size() < 5 )
	{
		if ( Log[ TEST ] ) return noteline_prefix_default;
		Exception( "Cannot assign noteline_prefix of length < 5" );//raise( SIGINT );
	}

	nlp.Octave = S.secure_stoi( arr[0] );
	if ( ( nlp.Octave < min_octave ) or ( nlp.Octave > max_octave ) )
		range_error( nlp.Octave, {min_octave, max_octave } );

	nlp.convention = S.secure_stoi(arr[1]);
	if ( ( nlp.convention < 0 ) or ( nlp.convention >= convention_names.size() ))
		range_error( nlp.convention, {0, convention_names.size() } );

	nlp.nps = S.secure_stoi( arr[2]);
	if ( not check_nps( nlp.nps) )
	{
		if ( Log[ TEST ] ) return noteline_prefix_default;
		Exception( "Cannot assign noteline_prefix " + str + " to noteline_structure" );//raise( SIGINT );
	}

	nlp.flat = S.secure_stoi(arr[3]);
	if ( ( nlp.flat < 0 ) or ( nlp.flat > 7 ))
		range_error( nlp.flat, {0, 7} );

	nlp.sharp = S.secure_stoi(arr[4]);
	if ( ( nlp.sharp < 0 ) or ( nlp.sharp > 7 ))
		range_error( nlp.sharp, {0, 7} );

	return nlp;

}

void Note_base::test()
{
	Set_Loglevel( TEST, true);
	Comment( TEST, "Notes_base test start");

	show_noteline_prefix( noteline_prefix_default );
	Comment( TEST, noteline_prefix_to_string( noteline_prefix_default ));
	string nlp_str = "2,1,4,0,0,";
	noteline_prefix_t nlp = string_to_noteline_prefix(nlp_str);
	show_noteline_prefix(nlp);

	nlp_str = "1 ";
	nlp = string_to_noteline_prefix(nlp_str);
	nlp_str = "8, 8, 8, 7, 8";
	nlp = string_to_noteline_prefix(nlp_str);
	Comment( TEST, "Notes_base test done");
}



