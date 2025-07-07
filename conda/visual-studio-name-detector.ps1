# PowerShell script to detect the latest Visual Studio and return a string like:
# "Visual Studio <display version major> <year>"
Invoke-Expression "vswhere -latest -products * -format json | ConvertFrom-Json | Select-Object -First 1"
exit 1
$vswhere = "$Env:ProgramFiles(x86)\Microsoft Visual Studio\Installer\vswhere.exe"
if (!(Test-Path $vswhere)) {
    Write-Error "vswhere.exe not found."
    exit 1
}

$instance = & vswhere -latest -products * -format json | ConvertFrom-Json | Select-Object -First 1
if (-not $instance) {
    Write-Error "No Visual Studio installation found."
    exit 1
}

$displayVersion = $instance.catalog_productDisplayVersion
$year = $instance.catalog_productLineVersion

if (-not $displayVersion -or -not $year) {
    Write-Error "Could not determine Visual Studio version/year."
    exit 1
}

$major = $displayVersion.Split('.')[0]

Write-Output "Visual Studio $major $year"
