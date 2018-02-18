Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.AppActivate "Khazama AVR Programmer"
WScript.Sleep 1000
WshShell.SendKeys "^p"
WScript.Sleep 1500
WshShell.SendKeys "{ENTER}"
WshShell.SendKeys "%{F4}"