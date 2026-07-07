@echo off
REM Avvia Gribbit Town nell'Unreal Editor 5.5
set UE="C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJ="C:\Users\pc\Unreal Projects\GribbitTown\GribbitTown.uproject"
start "" %UE% %PROJ%
