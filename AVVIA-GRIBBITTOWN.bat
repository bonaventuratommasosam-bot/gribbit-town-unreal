@echo off
REM Avvia Gribbit Town (progetto canonico = D:\GribbitTown) nell'Unreal Editor 5.5
REM -d3d11 evita il warning "out of date driver" della Intel UHD 620
set UE="C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJ="D:\GribbitTown\GribbitTown.uproject"
start "" %UE% %PROJ% -d3d11
