# Ocean Sound lab Ocean-SL

Ocean Sound Lab is a C++Project that provides a set of Sound Managing Applications, such as 
Synthesizer, Audioserver, Composer with Graphical UserInterface for Linux based Operating Systems, 
that allows to generate, play and record sound. It includes interfaces for musicxml-files and 
supports the sound drivers: -> native ALSA and -> Pulseaudio and consists of a direct interface to 
musescore3 and sound converting tools like ffmpeg and id3tool. The binary distribution of Ocean-SL
is limited to the x86_64 architecture (Nov.2025). This distribution includes the 3rd party 
libraries for Qt6, RtAudio and tiny2xml. It is tested and compiled on ubuntu 2025-10 with x86_64.
 

By Ulrich Glasmeyer, during the time 2023 - 2025

This distribution of Ocean-SL contains the following
- bin	: Audioserver, Synthesizer, Keyboard, Qt6-Userinterface OceanGUI, SDSview, Rtsp
- lib	: Qt6-runtime, tiny2xml and RtAudio runtime, libocean.so
- etc	: configuration file and sample program, notes and sound files
- doc	: user documentation
- var	: sample wav files

Ocean-SoundLab

The Ocean in the name of the software stands for waves. Theses are stimulated and summed together 
by the wind on the ocean. Take this picture and apply it to sound waves. These are put together by this software, the sound lab. Combining harmonic OSC, VCO and FMO base frequencies with harmonic overtones of their base frequency provides a phantastic sound universe on one hand. 
Select a specific point out of this univers and you get an instrument. With this instrument you
can play music, e.g. with notes from a musicxml-file or with your keyboard on the desk. You can 
add rythm to the sound and combine up to four synthesizer processes. All orchestrated by one 
graphical userinterface. Record these sounds and convert it into personalzed mp3-music files.
Thats it.

I am not a musician but a physician. Now its up to you.

The software was designed for 
- scalability and c++performance, 
- easy to use (once you are familiar with its features), 
- reliable process control
- reproducable results
and 
- all synthesizer controls are programable

Source code is available at https://github.com/uglasmeyer/ocean
To compile the project take a snapshot of the latest version.

There are some prerequisites as follows:
- gcc version with support for cxx standard 23, eg: (gcc (Ubuntu 15.2.0-4ubuntu4) 15.2.0)
- Qt6 development environment core, widgets, gui and qmake
- tinyxml2 development environment

If these prerequisites are fulfilled on the Linux machine
you can use the cmake system as follows:
- change to the Cmake directory of the source code distribution
- check the file CMakeLists.txt. 
By default it will create a binary distribution in \$HOME/OceanDev
Check also the correctness of include dirctory ${Qt6}.

- Enter the command: cmake CMakeLists.txt 
if this was successful,
- Enter the command: make


Author: Ulrich.Glasmeyer@web.de