@echo off
REM Crea la mappa vuota GribbitTown dentro l'Unreal Editor (modalita headless).
REM L'editor compila il modulo C++, crea l'empty map, assegna il GameMode e salva.
set UE="C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJ="C:\Users\pc\Unreal Projects\GribbitTown\GribbitTown.uproject"
set SCRIPT="C:\Users\pc\Unreal Projects\GribbitTown\Content\Python\CreateGribbitMap.py"

%UE% %PROJ% -ExecutePythonScript=%SCRIPT% -unattended -nopause -nullrhi
