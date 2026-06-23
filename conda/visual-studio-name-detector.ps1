if (-not (Get-Command vswhere -ErrorAction SilentlyContinue)) {
    Write-Error "vswhere not found. Please ensure it is installed and available in your PATH."
    exit 1
}

$displayVersion = & vswhere `
    -latest `
    -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property catalog_productDisplayVersion

$majorVersion = [int]$displayVersion.Split('.')[0]

# Map major version to release year. catalog_productLineVersion is unreliable
# for VS 2026+ (returns the major number instead of the year).
$yearMap = @{15 = 2017; 16 = 2019; 17 = 2022; 18 = 2026}
$releaseYear = $yearMap[$majorVersion]
if (-not $releaseYear) {
    Write-Error "Unsupported Visual Studio major version: $majorVersion"
    exit 1
}

Write-Output "Visual Studio $majorVersion $releaseYear"
