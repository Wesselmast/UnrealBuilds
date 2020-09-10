@echo off
	
setlocal enabledelayedexpansion

set stripped=%*
set stripped=!stripped:"=!

echo !stripped!

for %%s in (!stripped!) do (
	set fullstr=!fullstr! %%s
)

set fullstr=!fullstr:~1!

for /f "tokens=1,2,3 delims=#" %%a in ("%fullstr%") do (
	set first=%%a
	set secnd=%%b
	set third=%%c
)

echo %first%\Engine\Build\BatchFiles

if not exist "%first%\Engine\Build\BatchFiles" (
	powershell write-host -fore Red WBUILD ERROR: something went wrong!
	GOTO :eof
)
pushd %first%\Engine\Build\BatchFiles\
call RunUAT BuildCookRun -project=%secnd%  -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -maps=AllMaps -compile -stage -pak -archive -archivedirectory=%third%
popd