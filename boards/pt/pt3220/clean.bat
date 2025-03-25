@echo off
SET KPATH=%~dp0
echo __________________________________________________
echo ^>^>^> %~0
echo %KPATH%
del /Q /S "%KPATH%\*.lst" 2>nul
del /Q /S "%KPATH%\*.map" 2>nul
del /Q /S "%KPATH%\*.uvgui.*" 2>nul
del /Q /S "%KPATH%\*.uvguix.*" 2>nul
del /Q /S "%KPATH%\JLinkLog.txt" 2>nul
del /Q /S "%KPATH%\*.scvd" 2>nul
del /Q /S "%KPATH%\*.i" 2>nul
rd /Q /S "%KPATH%\output" 2>nul

echo.
choice /T 1 /C ync /CS /D y /n 
::TIMEOUT /T 1
