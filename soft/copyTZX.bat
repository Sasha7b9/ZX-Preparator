@echo off
rem Копирует рекурсивно файлы *.tzx из SRC в DST
setlocal

rem === Настройки ===
set "SRC=c:\ZX\zx_spectrum_tosec_set_september_2023\Games\"
set "DST=c:\ZX\zx_spectrum_tosec_set_september_2023\TZX\"

if not exist "%DST%" mkdir "%DST%"

for /r "%SRC%" %%F in (*.tzx) do (
    copy /Y "%%F" "%DST%\" >nul
)

echo Готово.
pause
