@echo off
echo %1,%2,%3 > d:\1.log
"C:\Program Files\Ghostgum\gsview\gsprint.exe" -color -noquery -all -printer "%1" -copies %2 %3
