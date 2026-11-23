@echo off
REM ============================================================
REM  GIOCA GRIBBIT TOWN
REM  Avvia l'Editor Unreal e carica la mappa di gioco.
REM  Poi premi Play (in alto a destra) per giocare.
REM ============================================================
set UE="C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJECT="D:\GribbitTown\GribbitTown.uproject"
set MAP=/Game/Maps/GribbitCity

%UE% %PROJECT% %MAP% -game -windowed -ResX=1280 -ResY=720 -d3d11
