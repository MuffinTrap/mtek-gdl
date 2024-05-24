# Changes made by muffintrap


### Version 0.100.1-muffintrap
Fixes and little additions. Breaks SpriteSet API.

#### New files
+ Added gitignore_example.txt to project_template

#### Modifications to library
+ Removed SpriteSetConfig and made LoadSprites to calculate dimensions from sprites per row and sprite height given as parameters.
+ Moved deltatime and elapsed to class variables in template.h. Elapsed time is measured in milliseconds
+ Added GetRoll() to WiiInput. Returns the roll of the wiimote in radians.
+ Modified install.sh: added checks and instruction
+ Renamed makefile.wii to Makefile, since there is no makefile.pc
+ Changed makefile to Makefile in project_template since Linux cares about case

#### Old files
+ Restored Tremor files in 3rdparty/libs-wii/oggplayer.

#### Removals
+ Removed .gitignore from include/mgdl-wii/
+ Removed file install since there is also install.sh

### Version 0.100.0-muffintrap
Bringing a lot of changes to the code. Removing things that are no longer needed, are provided by the environment or do not work.

#### New files
+ Copied install.bat from the 2.6 release.
+ Added install.sh to help installing on Linux systems.
+ Copied installation.txt from the 2.6 release and modified it.
+ Copied readme.txt from the 2.6 release and modified it.
+ Added function LoadImageBuffer to mgdl-image.cpp and .h to load pngs from buffers made by bin2obj utility. 
+ Copied makefile from the 2.6 release.
+ Copied mgui.h and .cpp from tools/ folder and renamed to mgdl-gui, changed them to work like immediate mode gui.
+ Copied font.c from tools/ and renamed it to mgdl-defaultfont.h to always have a default font to use.
+ Added mgdl-input-wii.h and .cpp to provide more convenient access to libogc:s input state
+ Added mgdl-assert.h and .cpp to provide asserts and on screen messages when they happen
+ Added project_template folder and files to have a template for a project and example code how to use input, audio and image functions
+ Brought images over from gdl-examples repository and added licenses.txt to clarify the licenses of assets in project_template
+ Added the Codeblocks Template folder from 2.6 release. Wrote more instructions for setting up Codeblocks under Windows 10.

#### Modifications to library
+ Modified mgdl-input-keyboard.cpp to fix a compilation error.
+ Modified mgdl-main.cpp function _resetcallback to fix a compilation error caused by ogc sending two parameters for _resetCallback. Note: ogc2 does not send any, so they probably are not important :D
+ Modified mgdl-texture.cpp. Removed register keyword from a variable since C++17 does not allow it. Added check for NULL before freeing mipmapBuff[1] in _ConvertMipmap()
+ Modified mgdl-main.h. Added overload for SetClearColor() that takes a single color. 
+ Modified mgdl-sprites.h and .c. Added a function that load sprites from Image and configuration struct.
+ Added file mgdl-util.h and .cpp
+ Changed #define header guards to #pragma once in all files
+ Changed mgdl-audio.h and .cpp to read music files from memory buffers
+ Changed 3rdparty/oggplayer files: added function PlayOggFilePtr to play from given file pointer
    TODO: Restored oggplayer to the version that isn the devkitPro examples folder
+ Changed mgdl-font.h and .cpp to allow loading only a range of characters instead of always 256. Removed the Font class since it loaded DirectQB font files and DirectQB is from 1999.
+ Added include of <cstring> to mgdl-font, because strlen, memset and memcpy were no longer available after removing the libraries
+ Changed usage of memalign to aligned_alloc because memalign is obsolete. Source: https://www.gnu.org/software/libc/manual/html_node/Aligned-Memory-Blocks.html


#### Removals
+ Removed tools folder. It contained the source for a Sheetmapper program, that does not compile without the pc version of mtek-gdl, which is not available. It might be possible to recreate using raylib or something similar.
+ Removed png library from 3rdparty/libs-wii, since devkitpro enviroment has a newer version of it installed when you install the wii pack. Added portlibs directories to template project makefile and makefile-wii 
+ Removed zlib library from 3rdparty/libs-wii, since devkitpro environment has a newer version of it installed when you install the wii pack
+ Removed tremor from 3rdparty/libs-wii/oggplayer since devkitpro environment has it already.

