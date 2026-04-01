@echo off
setlocal enabledelayedexpansion

rem CHANGE HERE TO YOUR FOLDER
set "SRC=source"

set "BASE=%USERPROFILE%"

for /R "%SRC%" %%F in (*.h) do (
    set "header=%%F"
    set "name=%%~nxF"
    set "found=0"

    for /R "%BASE%" %%T in (!name!) do (
        echo %%T | findstr /I /C:"%SRC%" >nul
        if errorlevel 1 (
            set "found=1"
            rem Compare files
            fc /B "!header!" "%%T" >nul
            if errorlevel 1 (
                echo Updating %%T
                copy /Y "!header!" "%%T" >nul
            )
        )
    )

    if !found! EQU 0 echo No match for !name!
)

endlocal
pause