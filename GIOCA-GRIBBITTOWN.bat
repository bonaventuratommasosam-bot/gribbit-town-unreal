@echo off
REM ============================================================
REM  GIOCA GRIBBIT TOWN
REM  Avvia l'Editor Unreal e carica la mappa di gioco.
REM  Poi premi Play (in alto a destra) per giocare.
REM ============================================================
set UE="C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJECT="C:\Users\pc\Documents\design\GribbitTown.uproject"
set MAP=/Game/Maps/GribbitTown_Main

%UE% %PROJECT% %MAP%
