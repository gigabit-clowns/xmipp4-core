#$props = & vswhere `
#    -latest `
#    -products * `
#    -property catalog_productDisplayVersion,catalog_productLineVersion `
#    -nologo

$displayVersion = & vswhere `
    -latest `
    -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property catalog_productDisplayVersion,catalog_productLineVersion

$lineVersion = & vswhere `
    -latest `
    -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property catalog_productLineVersion

Write-Output $displayVersion
Write-Output $lineVersion
exit 1

$catalogProductDisplayVersion = $props[0].Trim()
$catalogProductLineVersion   = $props[1].Trim()
$catalogProductLineMajor = $catalogProductLineVersion.Split('.')[0]

Write-Output "Display Version: $catalogProductDisplayVersion"
Write-Output "Line Major:      $catalogProductLineMajor"
