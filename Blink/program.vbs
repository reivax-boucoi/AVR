Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.AppActivate "Khazama AVR Programmer"
WScript.Sleep 500
WshShell.SendKeys "^p"
WScript.Sleep 1000
WshShell.SendKeys "{ENTER}"
WScript.Sleep 500
WshShell.SendKeys "%{F4}"