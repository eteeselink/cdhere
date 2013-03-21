@echo off

:: execute hereinfo, store output in %HEREINFO%
:: hereinfo will write any errors to stderr
FOR /F "tokens=*" %%i in ('%~dp0hereinfo.exe') do SET HEREINFO=%%i

if "%HEREINFO%"=="" (
  rem got error
) else (
  echo going to %HEREINFO%
  cd /d %HEREINFO%
)

:: remove %HEREINFO% 
set HEREINFO=