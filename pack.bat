@echo off
set dir=%cd%
rem 新建目录
if not exist %dir%\pack (
    md %dir%\pack
) else (
    rd /s /Q %dir%\pack
    md %dir%\pack
)
md %dir%\pack\resource
set cp=%dir%\pack
cd %dir%\resource\
xcopy * %cp%\resource\ /y /e /i /q
cd %dir%\build\
copy *.exe %cp%
copy *.dll %cp%
cd %dir%\bin\
copy *.dll %cp%