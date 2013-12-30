@echo off

set CDWHERE=
set PROGRAM=cdwhere

if !%1!==!/d! (
  set PROGRAM=cdto
  set OPTS=%2
)

:: execute cdwhere, store output in %CDWHERE%
:: cdwhere will write any errors to stderr
for /f "delims=" %%a in ('%~dps0%PROGRAM%.exe %OPTS%') do SET CDWHERE=%%a

if "%CDWHERE%"=="" (
  rem got error
) else (
  cd /d %CDWHERE%
)

:: remove %CDWHERE% 
set CDWHERE=
