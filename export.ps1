# Get the directory path of the current script
$SCRIPT_DIR = Split-Path -Parent $MyInvocation.MyCommand.Definition

# Check if the XF_ROOT environment variable exists
if (-Not (Test-Path Env:XF_ROOT)) {
    $Env:XF_ROOT = $SCRIPT_DIR
} else {
    if (-Not (Test-Path -Path $Env:XF_ROOT)) {
        Write-Output "XF_ROOT path is invalid"
        exit 1
    }
}

# Initialize array index
$index = 0
$output = @()

# Call the Python script and capture its output
$gen_kconfig_output = & python "$Env:XF_ROOT\tools\export_script\gen_kconfig.py"
$gen_kconfig_output_lines = $gen_kconfig_output -split "`n"

foreach ($line in $gen_kconfig_output_lines) {
    $output += $line.Trim()
    $index++
}

$output_list = $output -join " "

if ($args.Count -eq 0) {
    Write-Output "You need to specify a target: $output_list"
    exit 1
}

if ($args.Count -gt 1) {
    Write-Output "Too many parameters, only one parameter is supported"
    exit 1
}

$valid_port = $false
foreach ($item in $output) {
    if ($args[0] -eq $item) {
        $valid_port = $true
        break
    }
}

if (-Not $valid_port) {
    Write-Output "You need to choose one of the following targets: $output_list"
    exit 1
}

$Env:XF_TARGET = $args[0]

# Call the Python script and capture its output
$Env:XF_TARGET_PATH = & python "$Env:XF_ROOT\tools\export_script\get_path.py" $args[0]
$Env:XF_TARGET_PATH = $Env:XF_TARGET_PATH.Trim()

if ([string]::IsNullOrEmpty($Env:XF_TARGET_PATH)) {
    Write-Output "XF_TARGET_PATH is empty"
    exit 1
}

$Env:XF_VERSION = "v0.2.1"

# Execute the check_virtualenv.py script and capture the return value
& python "$Env:XF_ROOT\tools\export_script\check_virtualenv.py"
$VENV_RESULT = $LASTEXITCODE

if ($VENV_RESULT -eq 1 -or $VENV_RESULT -eq 2) {
    & "$Env:USERPROFILE\.xfusion\$Env:XF_VERSION\Scripts\activate.ps1"
}

# Upgrade pip and install necessary packages
& python -m pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple
& pip install xf_build==0.3.6 -i https://pypi.tuna.tsinghua.edu.cn/simple
& pip install windows-curses -i https://pypi.tuna.tsinghua.edu.cn/simple

Write-Output "XF_ROOT:           $Env:XF_ROOT"
Write-Output "XF_TARGET:         $Env:XF_TARGET"
Write-Output "XF_VERSION:        $Env:XF_VERSION"
Write-Output "XF_TARGET_PATH:    $Env:XF_TARGET_PATH"
