@echo off
set dir=%~dp0
if exist %dir%\build (
    rd /s /Q %dir%\build
    md %dir%\build
) else (
    md %dir%\build
)
if not exist %dir%\target (
    md %dir%\target
) else (
    rd /s /Q %dir%\target
    md %dir%\target
)
copy %dir%\bin\*.dll %dir%\build