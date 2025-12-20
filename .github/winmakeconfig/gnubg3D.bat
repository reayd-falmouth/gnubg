@echo off
@echo . 
@echo . Creating  a  new  Win32  build  for  GNU  Backgammon
@echo .           GTK+ GUI (3D and Panel Version)
@echo .
::
:: Prevent adding MingW dirs to path again and again
if NOT "%MINGWDIR%"=="" goto IFEXIST
::
:: Change MINGWDIR=C:\MinGW if you installed MinGW elsewhere
::  ============= to change if needed ===================
:: SET MINGWDIR=C:\MinGW
SET MINGWDIR=C:\msys64\mingw64
::  ============= to change if needed ===================
::
:: Path %PATH% is replaced to prevent interference
:: If you want to keep your existing DOS %path%, then use next line
:: SET PATH=%MINGWDIR%\bin;%MINGWDIR%\lib;%PATH%
SET PATH=%MINGWDIR%\bin;%MINGWDIR%\lib;
::
::
:IFEXIST
if exist %MINGWDIR%\bin\mingw32-make.exe goto MINGW2
@echo .
@echo .             !!!!!  mingw32-make.exe was not found !!!!!
@echo .
@echo . ** MinGW 2 is not installed properly
@echo .          check if %MINGWDIR%\bin is on your PC
@echo . ** MinGW 2 is not installed in %MINGWDIR%
@echo .          EDIT this bat file (line SET MINGWDIR=%MINGWDIR%)
@echo .
@echo .           Procedure terminated ABNORMALLY
@echo .
goto ENDED
::
::
:MINGW2
SET MAKE=mingw32-make.exe
@echo .
@echo . ....starting the make process
@echo .
:: Next line to prevent changes in config.gui3D are not copied to config.h
DEL config.h
:: Next lines to be sure you get a clean build
DEL board3D\*.o
DEL board3D\libboard.a
DEL *.o
::
:: Next line sets the BASEDIR variable for Makefile s
SET BASEDIR=%MINGWDIR%
::
:: Next line sets the GNUDIR variable for Makefile s
:: GNUDIR is the directory with the Source files
:: Adapt it if you installed the source files elsewhere
::  ============= to change if needed ===================
SET GNUDIR=C:\GNUBG
::  ============= to change if needed ===================
::
IF EXIST %GNUDIR%\GNUBG.C GOTO GNUDIROK
@echo .
@echo .             !!!!!  Source files not found !!!!!
@echo .
@echo . ** gnubg source files are not installed properly
@echo .          check if %GNUDIR%\gnubg.c is on your PC
@echo . ** source files are not installed in %GNUDIR%
@echo .          EDIT this bat file (line SET GNUDIR=%GNUDIR%)
@echo .
@echo .           Procedure terminated ABNORMALLY
@echo .
GOTO ENDED
::
:GNUDIROK
::
::
%MAKE% -f Makefile3D
@echo .
@echo . Procedure ended
@echo . Check if gnubg.exe is built
:ENDED