# Changes made by muffintrap

### Version 2.6.1
Bringing a lot of changes to the code. Removing things that are no longer needed, are provided by the environment or do not work.

#### New files
+ Copied install.bat from the 2.6 release.
+ Added install.sh to help installing on Linux systems.
+ Copied installation.txt from the 2.6 release and modified it.
+ Copied readme.txt from the 2.6 release and modified it.
+ Added function LoadImageBuffer to mgdl-image.cpp and .h to load pngs from buffers made by bin2obj utility. 
+ Copied makefile from the 2.6 release.

#### Modifications to library
+ Modified mgdl-input-keyboard.cpp to fix a compilation error.
+ Modified mgdl-main.cpp function _resetcallback to fix a compilation error caused by ogc sending two parameters for _resetCallback. Note: ogc2 does not send any, so they probably are not important :D
+ Modified mgdl-texture.cpp. Removed register keyword from a variable since C++17 does not allow it. Added check for NULL before freeing mipmapBuff[1] in _ConvertMipmap()
+ Modified mgdl-main.h. Added overload for SetClearColor() that takes a single color. 
+ Modified mgdl-sprites.h and .c. Added a function that load sprites from Image and configuration struct.
+ Added file mgdl-util.h and .c

#### Removals
+ Removed tools folder. It contained the source for a Sheetmapper program, that does not compile without the pc version of mtek-gdl, which is not available. It might be possible to recreate using raylib or something similar.
+ Removed png library from 3rdparty/libs-wii, since devkitpro enviroment has a newer version of it installed when you install the wii pack. Added portlibs directories to template project makefile and makefile-wii 
+ Removed zlib library from 3rdparty/libs-wii, since devkitpro environment has a newer version of it installed when you install the wii pack
+ Removed tremor from 3rdparty/libs-wii/oggplayer since devkitpro environment has it already.
+ Added include of <cstring> to mgdl-font, because strlen, memset and memcpy were no longer available after removing the libraries
