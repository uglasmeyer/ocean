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
 * Build.h
 *
 *  Created on: Dec 6, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef BUILD_H_
#define BUILD_H_

#include <Logfacility.h>
#include <data/Config.h>
#include <ReleaseNotes.h>
#include <System.h>

/**************************************************
 * Build_class
 *************************************************/
class Build_class :
	virtual public Logfacility_class
{
	string className = "";
	string srcdir	= "";
	file_structure* Bin 	= nullptr;

public:
	Build_class( string sdir, file_structure* bin ) :
		Logfacility_class("Build_class")
	{
		className = Logfacility_class::className;
		srcdir = sdir;
		Bin = bin;
	};
	virtual ~Build_class()
	{

		DESTRUCTOR( className );
	};

	void remote( string host )
	{
		string archlibdir = Bin->libdir + "aarch64/";
		string archbindir = Bin->bindir + "aarch64/";

		string ssh = "ssh " + host;
		string chdir = " 'cd " + srcdir;
		string cmd1 = ssh 	+ " '"
							+ " cd " + srcdir + " &&"
//							+ " git pull --no-edit ssh://saturn/home/sirius/git/Ocean/.git main &&"
							+ " git fetch saturn && git reset --hard saturn/main && "
							+ " cd Cmake &&"
							+ " cmake CMakeLists.txt &&"
							+ " make clean &&"
							+ " make -j4 &&"
							+ " cd " +  archlibdir + " && "
							+ " scp * saturn:" + archlibdir + " &&"
							+ " cd " +  archbindir + " && "
							+ " scp * saturn:" + archbindir + "'";

		System_execute( cmd1 );
		cout << cmd1 << endl;
	}
	void local()
	{
		string cmd1 = " cd " + srcdir + " &&"
						+ " cd Cmake &&"
						+ " cmake CMakeLists.txt &&"
						+ " make clean &&"
						+ " make -j12 ";
		System_execute( cmd1 );
		cout << cmd1 << endl;
	}

private:
};

#include <Logfacility.h>

/**************************************************
 * Deploy_class
 *************************************************/
class Deploy_class :
	virtual public Logfacility_class
{
	string className = "";
	string srcdir	= "";

public:
	Deploy_class( string sdir ) :
		Logfacility_class("Deploy_class")
	{
		className = Logfacility_class::className;
		srcdir = sdir;
	};
	virtual ~Deploy_class()
	{
		DESTRUCTOR( className );
	};

	void Update_Gitdir()
	{
		string cmd1 = "cd ~ && mv "
						      + rn_tgz
						+ " " + rn_sha256
						+ " " + rn_cksum
						+ " " + srcdir;
		System_execute( cmd1 );
		cout << cmd1 << endl;
	}

private:
};


#endif /* BUILD_H_ */
