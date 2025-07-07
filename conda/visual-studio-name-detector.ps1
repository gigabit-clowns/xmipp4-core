$displayVersion = & vswhere `
    -latest `
    -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property catalog_productDisplayVersion

$releaseYear = & vswhere `
    -latest `
    -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property catalog_productLineVersion

$majorVersion = $displayVersion.Split('.')[0]
Write-Output "Visual Studio $majorVersion $releaseYear"
