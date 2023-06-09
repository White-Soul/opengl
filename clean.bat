@echo off
set dir=%~dp0
if exist %dir%\build (
    rd /s /Q %dir%\build
)
if exist %dir%\target (
    rd /s /Q %dir%\target
)