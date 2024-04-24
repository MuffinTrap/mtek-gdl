@echo off
cls
echo MTek-GDL Wii Library Installer


rem Make sure devkitpro and devkitppc toolchains are installed
if not defined DEVKITPRO goto noDevkitPro
if not defined DEVKITPPC goto noDevkitPPC


rem Starting menu
echo.
echo This batch program will install the MTek-GDL Wii library (or just its necessary
echo dependencies) into your DevkitPPC environment so you can start developing Wii
echo projects with MTek-GDL.
echo.
echo The installer will also compile and install the following libraries as well
echo which are required for MTek-GDL to compile properly:
echo    zlib
echo    png
echo    vorbisidec
echo    oggplayer (modified to allow file streaming)
echo    valloc (for MEM2 memory management)
echo    dolexec
echo.
echo You can either install both the MTek-GDL library in binary form and its
echo dependencies or just the dependencies if you prefer the latter.
echo.
echo Installer options:
echo  1.) Install dependencies and MTek-GDL (recommended for beginners).
echo  2.) Install dependencies only.
echo  3.) Exit installer.
echo.
:menuOptionRetry
set op=3
set /p op=Enter an option: 
if "%op%"=="1" goto :install
if "%op%"=="2" goto :skipLibrary
if "%op%"=="3" goto :eof
goto :menuOptionRetry


rem Install dependencies
:install
echo.
echo Compiling dependencies (note: warning messages can simply be ignored)...
echo.
cd libs\wii
make
if NOT "%ERRORLEVEL%"=="0" goto :makeError

echo.
echo Installing dependencies...
echo.
make install
make clean
if NOT "%ERRORLEVEL%"=="0" goto :makeError
cd ..\..

rem Skip installing MTek-GDL if specified
if "%op%"=="2" goto :skipLibrary

echo.
echo Compiling MTek-GDL Wii library...
echo.
cd source\wii
make
if NOT "%ERRORLEVEL%"=="0" goto :makeError

echo.
echo Installing MTek-GDL Wii library...
echo.
make install
make clean
if NOT "%ERRORLEVEL%"=="0" goto :makeError
cd ..\..

:skipLibrary

echo.
echo.
if "%op%"=="1" echo MTek-GDL Wii library and its dependencies were successfully installed.
if "%op%"=="2" echo Dependencies were successfully installed.
echo.
echo Would you also like to install a patched version of libogc which makes memory
echo allocation routines only allocate in MEM1 and possibly a fix that corrects a
echo major video init bug when your program is run on a WiiU under Wii mode. In
echo most cases, it is recommended that you apply this patch unless you work on
echo projects that rely on the MEM2 memory allocation jump when MEM1 becomes full.
echo.
echo The old libogc library will be renamed to libogc.a.old prior to applying the
echo patch in case you want to revert back to the old library.
echo.
:patchPromptRetry
set op=N
set /p op=Apply patched libogc library? (Y/N): 
if "%op%"=="y" goto :installPatch
if "%op%"=="Y" goto :installPatch
if "%op%"=="n" goto :noInstallPatch
if "%op%"=="N" goto :noInstallPatch
goto :patchPromptRetry


:installPatch
rem Convert linux pathname set by the DevkitPro installer to a windows compatible pathname
set dkp_path=%DEVKITPRO:~1,1%:%DEVKITPRO:~2%
rem Check if libogc has already been patched
if exist %dkp_path%/libogc/lib/wii/libogc.a.old goto :patchApplied
rem Rename old library and then copy the new one in
mv %DEVKITPRO%/libogc/lib/wii/libogc.a %DEVKITPRO%/libogc/lib/wii/libogc.a.old
cp "patches/libogc patch/libogc.a" %DEVKITPRO%/libogc/lib/wii/
echo.
echo Patch applied successfully!
echo.
pause
goto :eof


:noInstallPatch
echo.
echo Patch not applied then.
echo.
pause
goto :eof


:patchApplied:
echo.
echo A patched libogc library appears to be already applied.
echo.
pause
goto :eof


rem Error handlers

:makeError
echo.
echo ERROR: MAKE error %ERRORLEVEL% occured.
echo.
echo Make sure you have DevkitPro, DevkitPPC and Minimal System installed
echo successfully from the DevkitPro installer.
echo.
echo If problemss persists, you can ask for help in the WiiBrew Forums at
echo http://forum.wiibrew.org
echo.
pause
goto :eof

:noDevkitPro
echo.
echo ERROR: DevkitPro appears to be not installed.
echo.
echo Download the DevkitPro installer here: http://devkitpro.org/
echo.
pause
goto :eof

:noDevkitPPC
echo.
echo ERROR: DevkitPPC toolchain appears to be not installed.
echo.
echo Run the DevkitPro installer again and install the required toolchain
echo through that program.
echo.
pause
goto :eof
