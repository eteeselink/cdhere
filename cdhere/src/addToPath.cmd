@echo off
powershell -command "[System.Environment]::SetEnvironmentVariable(\"PATH\", $Env:Path + \";%~dsp0\\", \"Machine\")"
if not errorlevel 1 goto :ok
echo Got errors! Are you sure you ran this as administrator?
goto :end

:ok
echo Added %~dsp0 to system path!

:end
pause