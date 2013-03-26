@echo off

pushd %~dp0

if "%1"=="" (
  echo Usage: 
  echo.
  echo   publish VERSION
) else (
  :: make directory without complaining if it already exists
  mkdir Downloads 2> nul
  
  :: remove old releases; we're version controlled anyway anyway
  del /S /Q Downloads\*

  :: create new release
  pushd Release  
  "c:\Program Files\7-Zip\7z.exe" a -tzip ..\Downloads\cdhere-%1.zip *
  popd
)

popd