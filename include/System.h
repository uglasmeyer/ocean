/**************************************************************************
MIT License

Copyright (c) 2025,2026 Ulrich Glasmeyer

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

 *
 *  Created on: Jan 4, 2024
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <Ocean.h>
#include <String.h>
#include <Exit.h>

extern int	 			System_execute		( const string& cmd, bool noexect = false );
extern int 				System_execute_bg	( const string& _cmd, string log1 );
extern void 			System_Test			();
extern bool 			Is_running_process	( const pid_t& pid );
extern void 			Remove_file			( string file );
extern void 			Rename_file			( string old_name, string new_name );
extern void 			Touch_file			( string file );
extern size_t 			loadData			( string name, void* data, size_t bytes );
extern bool 			dumpData			( string name, void* data, size_t bytes);
extern bool 			Write_template_text	( string file, string text, bool overwrite );

/**************************************************
 * dircontent_struct
 * provide information and keep data concerning
 * a list of directories (searchPath)
 * for a specific filename extension (ext)
 *************************************************/
struct dircontent_struct
{
	// a list of directories to search for files with extension ext
	string_vec_t	searchPath 			{};
	// contains the file names
	string_vec_t 	filename_vec		{};
	// contains the full qualified path to filename
	string_vec_t	dir_entry_vec		{};
	string 			ext					;
	// failes with exceptio in case of true
	bool			select_strong		; // do not replace not found by default

					dircontent_struct	( string_vec_t dirs, string _ext );
	virtual			~dircontent_struct	() = default;
	void 			UpdatePath			( string_vec_t path );
	void		 	list_directories	();
	void 			updateContent		();
	string			Getfile				( string name );
	string 			Select				( string& name, const string& default_name );
	string			Select_strong		( string& name, const string& default_name  );

	void			Create				( string name );
	bool 			Is_empty			();
	bool 			Is_file				( string name );


};
typedef dircontent_struct dircontent_t;


#endif /* INCLUDE_COMMON_H_ */
