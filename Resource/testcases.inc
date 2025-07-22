:syntax

call :syntaxtest1

pause key
call :maintest
exit

:syntaxtest1
call :sliding
instrument load piano
text load instrument file piano.kbd

notes set "(CEG)- A'BCD (CE)- A'BC(EG) C.(ACE)-GDA" 8
notes set octave 2
notes per_second 5
notes set prefix 1 2

notes play on 20
rec loop _NOTES 100 1
pause 20
notes set octave 3
notes per_second 4
notes set prefix 1 3
pause auto

text syntax check program flow
include lib
return

:maintest
text entering main test
pause key


StA1 = 1
add 	drums  drums  StA1
play StA1 amp 100

text add/ Play notes using an instrument
pause key

instrument load drums		# load file etc/Instrument/drums.kdb
notes load drums	# load file etc/Notes/drums.kbd
notes play on 100 		# switch to notes mode
rec store StA1			# store to memory 1
pause auto				# wait for sentence duration seconds
rec stop StA1			# stop record on memory 1
notes play off			# switch off note playing
play StA1 amp 100		# switch on memory 1 playing

text This is the same as saying (foreground mode):
pause key

StA2 = 0
INS = 4
rec amp INS 100
osc main unmute
set main freq 220
set vco amp 50
adsr beat 4
adsr attack 10
adsr decay 90
adsr softfreq 0
rec store StA2
pause 5
rec amp StA2 100

text some osc and adsr commands
pause key

RECFILE = 77
record file 10
record stop RECFILE
rec mute StA2
rec mute StA1 
rec amp 7 100
record play RECFILE 5

text record file name is synthesizer77.wav and synthesizer.mp3
pause key

exit

:sliding

osc main reset 
set fmo freq A3
set fmo amp 20
adsr beat 0
play _INSTRUMENT amp 75 5
osc main set sinus A4 80 5
osc main loop freq A5
pause 2
set main freq A2
pause 2
set main freq A3
osc main amp loop 0
pause 10
play _INSTRUMENT amp 75 5
instrument load Piano
set fmo freq 10
notes load drums
rec mute _INSTRUMENT
play 6 amp 75

pause key
exit
