# ocean_sound_lab v4.2.1-41

Summary
- Prebuilt binary and runtime distribution for release 4.2.1-41 (tarball).

what's included
- ocean_sound_lab_4.2.1-41.tar.gz - prebuilt binaries for linux x86_64, aarch64, Ubuntu libc.so.6
and supporting files.

- Ocean Sound Lab provides a programmable set of Sound Managing Applications, such as Synthesizer, 
Audioserver, Composer with Graphical UserInterface for Linux based Operating Systems, 
that allows to generate, play and record sound. 
It includes interfaces for musicxml-files, supports the sound drivers: native ALSA and Pulseaudio 
and consists of a direct integration of musescore3 and the music file converting tools 
ffmpeg and id3tool. 
This distribution includes the 3rd party runtime libraries for Qt6, RtAudio and tiny2xml 
and is tested and compiled on ubuntu 2025-10 for x86_64 and aarch64 on rasperry PI and NUC.


Installation
1. Download the tarball from the Release assets.
2. Extract:
   tar -xzf ocean_sound_lab_4.2.1-41.tar.gz
3. Follow the install.txt inside the extracted folder for runtime/setup instructions.

Verification
		- verify cksum (as provided by packager):
		3515971935 116930179 ocean_sound_lab_4.2.1-41.tar.gz
- verify SHA-256 locally: 
f945c1665bf5ef40fb4ebb8b8b63cdbfa5f3f7d5293c6f0156b70a0846ff4ce7  ocean_sound_lab_4.2.1-41.tar.gz
- Verify: shasum -a 256 -c ocean_sound_lab_4.2.1-41.tar.gz.sha256

License & source
- This binary distribution is provided under the terms in the repository LICENSE file.
- Full source corresponding to this build is available at:  
https://github.com/uglasmeyer/ocean/releases/tag/v4.2.1-41

Notes
- N/A. If this release contains breaking changes, document migration steps here.
- Consumer should refer to further information in the Ocean-SL_Usermanual.pdf, but should consider 
this as beeing under construction with respect to a
- comprehensive desription of GUI features
- and workflows
- documentation of the keyboard usage and features
- libOcean.so SDS structure


Changelog
- combined distribution of x86 and arch binaries
- GUI Cut desk, allowing to cut recorded wav files


