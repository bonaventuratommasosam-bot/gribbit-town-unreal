# Completa lo spostamento di Unreal Engine 5.5 da C: a D:.
# Da eseguire in un PowerShell APERTO COME AMMINISTRATORE.
#
# Non cancella niente se la copia su D: non corrisponde all'originale.

$ErrorActionPreference = 'Stop'
$src = 'C:\Program Files\Epic Games\UE_5.5'
$dst = 'D:\EpicGames\UE_5.5'

Write-Host "=== Verifica ==="

if (-not (Test-Path $src)) { Write-Host "Origine assente: forse e' gia' stato spostato."; exit 1 }
if (-not (Test-Path $dst)) { Write-Host "ERRORE: la copia su D: non esiste. Annullo."; exit 1 }
if (-not (Test-Path (Join-Path $dst 'Engine\Binaries\Win64\UnrealEditor.exe'))) {
    Write-Host "ERRORE: UnrealEditor.exe non trovato nella copia. Annullo."
    exit 1
}

$fs = Get-ChildItem $src -Recurse -File -Force -ErrorAction SilentlyContinue | Measure-Object Length -Sum
$fd = Get-ChildItem $dst -Recurse -File -Force -ErrorAction SilentlyContinue | Measure-Object Length -Sum

Write-Host ("origine C:  {0,7:N0} file  {1,6:N2} GB" -f $fs.Count, ($fs.Sum / 1GB))
Write-Host ("copia   D:  {0,7:N0} file  {1,6:N2} GB" -f $fd.Count, ($fd.Sum / 1GB))

$mancanti = $fs.Count - $fd.Count
$deltaGB = ($fs.Sum - $fd.Sum) / 1GB

if ($mancanti -gt 50 -or $deltaGB -gt 0.05) {
    Write-Host ""
    Write-Host ("STOP: alla copia mancano {0:N0} file e {1:N2} GB." -f $mancanti, $deltaGB)
    Write-Host "Non cancello niente. Fai ricompletare la copia e rilancia questo script."
    exit 1
}

Write-Host ""
Write-Host "Copia integra. Procedo."
Write-Host ""
Write-Host "=== Cancellazione dell'originale su C: ==="
Remove-Item -LiteralPath $src -Recurse -Force
Write-Host "fatto."

Write-Host ""
Write-Host "=== Giunzione al posto vecchio ==="
cmd /c mklink /J "$src" "$dst" | Out-Null
if (Test-Path (Join-Path $src 'Engine\Binaries\Win64\UnrealEditor.exe')) {
    Write-Host "giunzione creata e funzionante: $src -> $dst"
} else {
    Write-Host "ATTENZIONE: giunzione non verificata. Controlla a mano."
}

Write-Host ""
Write-Host ("C: liberi {0:N1} GB   D: liberi {1:N1} GB" -f ((Get-PSDrive C).Free / 1GB), ((Get-PSDrive D).Free / 1GB))
Write-Host ""
Write-Host "Finito. Ora puoi riaprire Unreal."
