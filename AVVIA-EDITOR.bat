@echo off
REM Avvia Gribbit Town nell'Unreal Editor 5.5 (modalita editor, NON -game)
REM Carica direttamente GribbitTown_Main. Usa -d3d11 per la Intel UHD 620.
set UE="C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJ="D:\GribbitTown\GribbitTown.uproject"
set MAP=GribbitCity
start "" %UE% %PROJ% %MAP% -d3d11
