REM Put this file into folder where your windows
REM release is and run it
REM It copies over the needed dll files from default
REM msys64 location
@echo off
xcopy C:\msys64\ucrt64\bin\libgcc_s_seh-1.dll 
xcopy C:\msys64\ucrt64\bin\libfreeglut.dll
xcopy C:\msys64\ucrt64\bin\libopenal-1.dll
xcopy C:\msys64\ucrt64\bin\libpng16-16.dll
xcopy C:\msys64\ucrt64\bin\libsndfile-1.dll
xcopy C:\msys64\ucrt64\bin\libstdc++-6.dll
xcopy C:\msys64\ucrt64\bin\libwinpthread-1.dll
xcopy C:\msys64\ucrt64\bin\zlib1.dll
xcopy C:\msys64\ucrt64\bin\libFLAC.dll
xcopy C:\msys64\ucrt64\bin\libmpg123-0.dll
xcopy C:\msys64\ucrt64\bin\libmp3lame-0.dll
xcopy C:\msys64\ucrt64\bin\libogg-0.dll
xcopy C:\msys64\ucrt64\bin\libvorbis-0.dll
xcopy C:\msys64\ucrt64\bin\libopus-0.dll
xcopy C:\msys64\ucrt64\bin\libvorbisenc-2.dll
xcopy C:\msys64\ucrt64\bin\libvorbisfile-3.dll
