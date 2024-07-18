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
#define MAINFREQUENCYKEY	221
#define VCOFREQUENCYKEY		225
#define FMOFREQUENCYKEY		227
#define FMOLFO_KEY			222
#define VCOLFO_KEY			223

// OSC volume
#define MUTEMAINAMP_KEY 	'a'
#define UNMUTEMAINAMP_KEY	134
#define VCOAMPKEY 			'1'
#define FMOAMPKEY			'2'
#define MASTER_AMP_KEY			'3'
#define MAIN_AMPLOOP_KEY	138 // parameter, LOOP_end, LOOP_step

// OSC connect
#define CONNECTVCOFMOKEY	'V'
#define CONNECTFMOVCOKEY	'F'

// OSC Waveform
#define SETWAVEFORMFMOKEY	'u'
#define SETWAVEFORMVCOKEY	'w'
#define SETWAVEFORMMAINKEY  'W'

// Reset OSC
#define RESETFMOKEY			'f'
#define RESETVCOKEY			'v'
#define RESETMAINKEY		'o'

// ADSR
#define ADSRDURATIONKEY 	'b'
#define ADSRDECAYKEY		'c'
#define ADSRSUSTAINKEY		'e'
#define ADSRHALLKEY 		'C'
#define PMWDIALKEY			'd'
#define SOFTFREQUENCYKEY	103

// DataArray
#define STORESOUNDKEY		'L'
#define STOPRECORD_KEY		131
#define SETMBAMPPLAYKEY		109
#define MUTEMBKEY			'M'
#define MUTEREC_KEY			135
#define TOGGLEMBPLAYKEY		'n'
#define CLEAR_KEY			136
#define EXTERNAL_AMPLOOP_KEY 139

// Notes
//#define NOTESONOFFKEY		128
#define NEWNOTESLINEKEY		'P'
#define NEWNOTESLINEFLAG	'Z'
#define UPDATENOTELINEKEY 	'p'
#define UPDATENOTESKEY		'k'
#define NOTESONKEY 			129
#define NOTESOFFKEY			130
#define PLAYNOTESREC_ON_KEY 132
#define PLAYNOTESRECOFF_KEY 133
#define SETNOTESPERSEC_KEY	140
#define SETBASEOCTAVE_KEY	141

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
#define	UPDATESPECTRUM_KEY  137

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
