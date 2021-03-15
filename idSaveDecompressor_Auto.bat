@echo off
setlocal enableextensions
cd /d "%~dp0"

:StartLoop
if  "%~1"== "" goto EndLoop
findstr /B /C:"IDSAVEDECOMPRESSOR" "%~1" >nul 2>&1
if errorlevel 1 (
	idSaveDecompressor.exe -d "%~1" "%~1" echo on
) else (
	idSaveDecompressor.exe -c  "%~1" "%~1" echo on
)
shift
goto StartLoop
:EndLoop
PAUSE