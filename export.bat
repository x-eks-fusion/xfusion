@echo off

rem Get the directory path of the current script
set "SCRIPT_DIR=%~dp0"

rem Check if the XF_ROOT environment variable exists
if not defined XF_ROOT (
    set "XF_ROOT=%SCRIPT_DIR%"
) else (
    if not exist "%XF_ROOT%" (
        echo XF_ROOT path is invalid
        exit /b
    )
)

rem Initialize array index
set index=0


rem Enable delayed variable expansion
setlocal enabledelayedexpansion

rem Call the Python script and capture its output
for /f "delims=" %%i in ('python "%XF_ROOT%tools\export_script\gen_kconfig.py"') do (
    set "output[!index!]=%%i"
    set /a index+=1
)

set "output_list="
for /l %%i in (0,1,!index!-1) do (
    set "output_list=!output_list! !output[%%i]!"
)
set "output_list=!output_list:~1!"

if "%~1"=="" (
    echo You need to specify a target: %output_list%
    exit /b
)

if "%~2" NEQ "" (
    echo Too many parameters, only one parameter is supported
    exit /b
)

set "valid_port=false"
for /l %%i in (0,1,!index!-1) do (
    if "%~1"=="!output[%%i]!" (
        set "valid_port=true"
    )
)

if "%valid_port%"=="false" (
    echo You need to choose one of the following targets: %output_list%
    exit /b
)

endlocal

set "XF_TARGET=%~1"

for /f "delims=" %%i in ('python "%XF_ROOT%tools\export_script\get_path.py" %~1') do (
    set "XF_TARGET_PATH=%%i"
)

if "%XF_TARGET_PATH%"=="" (
    echo XF_TARGET_PATH is empty
    exit /b
)

set "XF_VERSION=v0.2.1"

python "%XF_ROOT%tools\export_script\check_virtualenv.py"
set "VENV_RESULT=%ERRORLEVEL%"

echo %USERPROFILE%\.xfusion\%XF_VERSION%\bin\activate.bat
if "%VENV_RESULT%"=="1" (
    call "%USERPROFILE%\.xfusion\%XF_VERSION%\Scripts\activate.bat"
) else if "%VENV_RESULT%"=="2" (
    call "%USERPROFILE%\.xfusion\%XF_VERSION%\Scripts\activate.bat"
)

python.exe -m pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install xf_build==0.3.8 -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install windows-curses -i https://pypi.tuna.tsinghua.edu.cn/simple

echo XF_ROOT:           %XF_ROOT%
echo XF_TARGET:         %XF_TARGET%
echo XF_VERSION:        %XF_VERSION%
echo XF_TARGET_PATH:    %XF_TARGET_PATH%
