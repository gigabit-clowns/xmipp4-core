$props = & vswhere `
    -latest `
    -products * `
    -property catalog_productDisplayVersion,catalog_productLineVersion `
    -nologo

Write-Host $props
exit 1

$catalogProductDisplayVersion = $props[0].Trim()
$catalogProductLineVersion   = $props[1].Trim()
$catalogProductLineMajor = $catalogProductLineVersion.Split('.')[0]

Write-Host "Display Version: $catalogProductDisplayVersion"
Write-Host "Line Major:      $catalogProductLineMajor"
