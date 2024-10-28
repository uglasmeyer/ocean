
return
#-----------------------------------------

:const
_NOTES = 6
_EXTERNAL = 7
_INSTRUMENT = 4
_KEYBOARD = 5
return


:stopserver
stop AudioServer
stop Synthesizer

return
#-----------------------------------------

:startserver
#start 	AudioServer # shmkey is set to frames per second
#start 	Synthesizer 
start rtsp -C
return
#-----------------------------------------

:clearstorage
rec clear 0
rec clear 1
rec clear 2
rec clear 3
rec clear 4
rec clear 5
rec clear 6
rec clear 7
return

#-----------------------------------------

:standardintro
notes play off
osc main mute

osc main amp 20
rec amp 7 20

record file
record loop amp 100
osc main loop amp 100
return

#-----------------------------------------

:standardend
TRACKNR = 1
osc main loop amp 0
record loop amp 0
pause 7
record stop TRACKNR
pause 6 # wait until recording to file is done
osc main amp 75
#call :stopserver
return
