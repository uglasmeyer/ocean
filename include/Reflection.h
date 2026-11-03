/**************************************************************************
MIT License

Copyright (c) 2026 Ulrich Glasmeyer

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
 * Reflection.h
 *
 *  Created on: Nov 2, 2026
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef REFLECTION_H_
#define REFLECTION_H_


#include <bit>
#include <contracts>
#include <iostream>
#include <utility>
#include <meta>
#include <iostream>
#include <type_traits>
#include <vector>

using std::vector;
using std::string_view;

typedef vector< string_view >	stringview_vec_t;

#if __glibcxx_reflection < 202506L
#error "Reflection not supported"
#endif

#if __cplusplus < 202303L
#error "Reflection requires CXX26 at least __cplusplus"
#endif
#include <meta>


//using namespace  std::meta;
using std::meta::info; // @suppress("Symbol is not resolved")
using std::meta::access_context;
using std::meta::is_object_type;
using std::meta::display_string_of;
using std::meta::is_enum_type;
using std::meta::identifier_of;
using std::meta::is_scoped_enum_type;


template< typename Obj_t >
	requires( is_object_type( ^^Obj_t ) && not is_enum_type( ^^Obj_t ))
constexpr stringview_vec_t	show_members()
{
    stringview_vec_t		sv_vec{};
    constexpr auto 			ctx		{ access_context::unchecked() };
    static constexpr auto 	members	{ std::define_static_array( members_of( ^^Obj_t, ctx ) ) };
	template
	for( constexpr auto member : members )
	{
    	sv_vec.push_back( display_string_of( member ) );
	}
	return sv_vec;
};

template <typename Enum_t>
	requires( is_enum_type( ^^Enum_t ) )
constexpr stringview_vec_t	show_members()
{
	stringview_vec_t		sv_vec	{};
    constexpr static auto	enumerators	= std::define_static_array( enumerators_of( ^^Enum_t ) );
    template for( constexpr info enumerator	: enumerators )
    {
        // splice the enumerator reflection back into a usable runtime value
    	// constexpr Enum_t 	value	= [: enumerator :];
        constexpr auto	name	= identifier_of( enumerator );
       	sv_vec.push_back		( name  );
       	//        	std::cout << "  " << name << " = " << std::to_underlying(value) << '\n';
    };
    return sv_vec;

}




#endif /* REFLECTION_H_ */
