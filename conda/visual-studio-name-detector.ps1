#$props = & vswhere `
#    -latest `
#    -products * `
#    -property catalog_productDisplayVersion,catalog_productLineVersion `
#    -nologo

$props = & vswhere -latest
Write-Output $props
exit 1

$catalogProductDisplayVersion = $props[0].Trim()
$catalogProductLineVersion   = $props[1].Trim()
$catalogProductLineMajor = $catalogProductLineVersion.Split('.')[0]

Write-Output "Display Version: $catalogProductDisplayVersion"
Write-Output "Line Major:      $catalogProductLineMajor"
