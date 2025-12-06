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
 * ReleaseNotes.h
 *
 *  Created on: Nov 16, 2025
 *      Author: Ulrich.Glasmeyer@web.de
 */

#ifndef RELEASENOTES_H_
#define RELEASENOTES_H_

#include <Version.h>

extern string getArch();

const string rn_filename = "RELEASE_NOTES.md";
const string rn_newline = "\n\n";
const string rn_appname = "ocean_sound_lab";
const string rn_architecture = getArch();
const string rn_archive = rn_appname + "_" + Version_No + ".tar";
const string rn_tgz = rn_archive + ".gz";
const string rn_sha256 = rn_tgz + ".sha256";
const string rn_cksum = rn_tgz + ".cksum";
const string rn_tag = "v" + Version_No;
const string rn_userdoc = "Ocean-SL_Usermanual";

const string rn_title = "# " + rn_appname + " v" + Version_No + rn_newline;
const string rn_summary =
		"Summary\n"
		"- Prebuilt binary and runtime distribution for release " +
		Version_No +
		" (tarball)." + rn_newline;

const string rn_include =
		"what's included\n- " +
		rn_tgz + " - prebuilt binaries for linux x86_64, aarch64, Ubuntu libc.so.6\n" +
 		"and supporting files." + rn_newline;

const string rn_changes =
R"(- Ocean Sound Lab provides a programmable set of Sound Managing Applications, such as Synthesizer, 
Audioserver, Composer with Graphical UserInterface for Linux based Operating Systems, 
that allows to generate, play and record sound. 
It includes interfaces for musicxml-files, supports the sound drivers: native ALSA and Pulseaudio 
and consists of a direct integration of musescore3 and the music file converting tools 
ffmpeg and id3tool. 
The binary distribution of Ocean-SL is limited to the x86_64 architecture (as of Nov.2025). 
This distribution includes the 3rd party runtime libraries for Qt6, RtAudio and tiny2xml 
and is tested and compiled on ubuntu 2025-10 for x86_64.
)" + rn_newline;

const string rn_installation =
R"(Installation
1. Download the tarball from the Release assets.
2. Extract:
   tar -xzf )" + rn_tgz + "\n" +
"3. Follow the install.txt inside the extracted folder for runtime/setup instructions." + rn_newline;

const string rn_license =
R"(License & source
- This binary distribution is provided under the terms in the repository LICENSE file.
- Full source corresponding to this build is available at:  
https://github.com/uglasmeyer/ocean/releases/tag/)" + rn_tag + rn_newline;

const string rn_notes =
R"(Notes
- N/A. If this release contains breaking changes, document migration steps here.
- Consumer should refer to further information in the )" + rn_userdoc +
R"(.pdf, but should consider 
this as beeing under construction with respect to a
- comprehensive desription of GUI features
- and workflows
- documentation of the keyboard usage and features
- libOcean.so SDS structure
)" + rn_newline;

const string rn_changelog =
R"(Changelog
- combined distribution of x86 and arch binaries
- GUI Cut desk, allowing to cut recorded wav files
)" + rn_newline;

#endif /* RELEASENOTES_H_ */
