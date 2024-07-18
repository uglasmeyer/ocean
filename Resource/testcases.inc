:syntax

call :syntaxtest1

pause key
call :maintest
exit

:syntaxtest1
osc main mute
text syntax check program flow
include lib
return

:maintest
text entering main test
pause key


StA1 = 1
add 	drums  drums3  StA1
play StA1 amp 100

text add/ Play notes using an instrument
pause key

instrument load drums	# load files drums2.kdb and drums2.spec
notes load drums3		# load file drums3.kbd
notes play on 100 		# switch to notes mode
rec store StA1			# store to memory 1
pause auto				# wait for sentence duration seconds
rec stop StA1			# stop record on memory 1
notes play off			# switch off note playing
play StA1 amp 100		# switch on memory 1 playing

text This is the same as saying (foreground mode):
pause key

StA2 = 0
osc main unmute
set main freq 220
set vco amp 50
adsr beat 4
adsr attack 10
adsr decay 90
adsr softfreq 0
rec store StA2
pause 5
osc main mute
rec amp StA2 100

text some osc and adsr commands
pause key

RECFILE = 77
record file 10
record stop RECFILE
rec amp StA2 0
rec amp StA1 1
rec amp 7 100
record play RECFILE 5

text record file synthesizer77.wav
pause key

instrument load piano
text load instrument file piano.kbd and associated spectrum file piano.spec
pause key

osc main mute
notes set octave 3
notes per_second 5
notes set "|2(CEG)----- |2A'BCD (CE)- A'BC(EG) C.(ACE)-B.A." 8
notes play on 80
pause key

osc main unmute

