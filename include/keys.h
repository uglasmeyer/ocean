/*
 * keys.h
 *
 *  Created on: Apr 1, 2024
 *      Author: sirius
 */

/*
https://en.wikipedia.org/wiki/ASCII#Character_set
 */


#ifndef INCLUDE_KEYS_H_
#define INCLUDE_KEYS_H_


// OSC frequency
#define MAINFREQUENCYKEY	127
#define VCOFREQUENCYKEY		126
#define FMOFREQUENCYKEY		125
#define FMOLFO_KEY			124
#define VCOLFO_KEY			123

// OSC volume
#define MUTEMAINAMP_KEY 	'a'
#define UNMUTEMAINAMP_KEY	122
#define VCOAMPKEY 			49
#define FMOAMPKEY			50
#define MASTER_AMP_KEY		51
#define MAIN_AMPLOOP_KEY	121 // parameter, LOOP_end, LOOP_step

// OSC connect
#define CONNECTVCOFMOKEY	'V'
#define CONNECTFMOVCOKEY	'F'

// OSC Waveform
#define SETWAVEFORMFMOKEY	'u'
#define SETWAVEFORMVCOKEY	'w'
#define SETWAVEFORMMAINKEY  'W'

// Reset OSC
#define RESETFMOKEY			'f'
#define RESETVCOKEY			15
#define RESETMAINKEY		12

// ADSR
#define ADSRDURATIONKEY 	'b'
#define ADSRDECAYKEY		'c'
#define ADSRSUSTAINKEY		'e'
#define ADSRHALLKEY 		'C'
#define PMWDIALKEY			'd'
#define SOFTFREQUENCYKEY	103

// DataArray
#define STORESOUNDKEY		'L'
#define STOPRECORD_KEY		120
#define SETMBAMPPLAYKEY		109
#define MUTEMBKEY			'M'
#define MUTEREC_KEY			19
#define TOGGLEMBPLAYKEY		21
#define CLEAR_KEY			118
#define EXTERNAL_AMPLOOP_KEY 17

// Notes
#define NEWNOTESLINEKEY		'P'
#define NEWNOTESLINEFLAG	'Z'
#define UPDATENOTELINEKEY 	14
#define UPDATENOTESKEY		'k'
#define NOTESONKEY 			116
#define NOTESOFFKEY			20
#define PLAYNOTESREC_ON_KEY 114
#define PLAYNOTESRECOFF_KEY 22
#define SETNOTESPERSEC_KEY	13
#define SETBASEOCTAVE_KEY	11

// Wavedisplay
#define WAVEDISPLAYTYPEKEY	'D'
#define SETWAVEDISPLAYKEY	'q'

// External
#define RECORDWAVFILEKEY	'R'
#define RECORDWAVFILEFLAG	'X'
#define SETEXTERNALWAVEFILE	'l'

// Instrument
#define SAVEINSTRUMENTKEY 	's'
#define NEWINSTRUMENTKEY 	'S'
#define NEWINSTRUMENTFLAG 	'Y'
#define SETINSTRUMENTKEY	'K'

// Spectrum
#define	UPDATESPECTRUM_KEY  110
#define GETSPECTRUM_KEY		52

#define USERINTERFACE_FLAG   1
#define EXITKEY				'#'
#define NULLKEY 			0

#include <vector>

const std::vector<char> init_keys = {

		SETINSTRUMENTKEY,
		ADSRDURATIONKEY,
		ADSRDECAYKEY,
		ADSRSUSTAINKEY,
		ADSRHALLKEY,
		PMWDIALKEY,
		SOFTFREQUENCYKEY,
		WAVEDISPLAYTYPEKEY,
		SETWAVEDISPLAYKEY,
		UPDATENOTESKEY
};


#endif /* INCLUDE_KEYS_H__ */
