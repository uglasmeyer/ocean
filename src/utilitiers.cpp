/*
 * utilitiers.cpp
 *
 *  Created on: Oct 17, 2025
 *      Author: sirius
 */

#include <utility>
#include <Utilities.h>

void show_source_location( const experimental::source_location location )
{
			cout << "file: "
              << location.file_name() << '('
              << location.line() << ':'
              << location.column() << ") `"
              << location.function_name() << "`: \n";
}
