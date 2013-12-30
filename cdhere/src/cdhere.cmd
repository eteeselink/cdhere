@echo off

set CDWHERE=
set CDHERE_OPTS=

if !%1!==!/d! (
  set CDHERE_OPTS=/d %2
)

:: execute cdwhere, store output in %CDWHERE%
:: cdwhere will write any errors to stderr
for /f "delims=" %%a in ('%~dps0cdwhere.exe %CDHERE_OPTS%') do SET CDWHERE=%%a

if "%CDWHERE%"=="" (
  rem got error
) else (
  cd /d %CDWHERE%
)

:: remove env vars
set CDWHERE=
set CDHERE_OPTS=