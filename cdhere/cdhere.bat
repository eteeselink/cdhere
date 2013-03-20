@echo off

:: execute hereinfo, store output in %HEREINFO%
FOR /F "tokens=*" %%i in ('%~dp0hereinfo.exe') do SET HEREINFO=%%i

cd /d %HEREINFO%

:: remove %HEREINFO% 
set HEREINFO=