@echo off

REM Change -noslow back to -slow for full work

REM - Trick to save output of program to DRIVE var - tool to be built
FOR /F "tokens=1 delims=/ " %%i in ('u3_stuff.exe') do SET DRIVE=%%i

REM - Really no case info - so, just something fun for now
set case="Fluffy Bunny Attacks Again"

REM - The final location for the reports - Drive + this.
set dst=%drive%:\report\$magic$

REM - Lets get down to business
echo Starting wft - saving output to %dst%
pause

REM - If on %drive% wft.cfg exists - use it
IF EXIST %drive%:\wft.cfg set cfg=-cfg %drive%:\wft.cfg

wft\wft.exe -case %case% -noslow -dst %dst% -nointeractive %cfg%

pause

