@echo off

set CDWHERE=

:: execute cdwhere, store output in %CDWHERE%
:: cdwhere will write any errors to stderr
for /f %%a in ('%~dp0cdwhere.exe') do SET CDWHERE=%%a

if "%CDWHERE%"=="" (
  rem got error
) else (
  cd /d %CDWHERE%
)

:: remove %CDWHERE% 
set CDWHERE=