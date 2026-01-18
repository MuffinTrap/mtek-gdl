# Version history and a summary of changes

### Version 0.100.5-muffintrap "ggj26"
This version removes old Wii code, adds cross platform Ogg and Mp3 players.

#### Changes
+ Windows version can now be built using Visual Studio. The old Windows port is now called msys2. Windows version uses XInput and DirectSound.
+ Glut joystick added for msys2.
+ Refactored how joysticks and controllers are related
+ Added dr_mp3 to play mp3 files
+ Added stb_vorbis to play ogg files
+ Removed references to libsndfile, libvorbis, libogg. Wii still uses libpng.
+ Created unified Audio API that makes it easy to add more music players in future.
+ Added allocation functions for General and Graphics, but not really used yet.
+ Xbox 360 gamepad works on Windows and Linux. Limited working on msys2. Not at all on MacOS.

### Version 0.100.4-muffintrap "assembly"
This version changes to C and breaks all backwards compatibility.

#### Changes

+ Removed asset loading functions that load from a memory buffer.
+ Removed namespace gdl mostly.
+ WiiController converted to C. Removed deadzones since they were not used anyway.
+ Font converted to C and removed redundant UVs that were stored.
+ All cross platform and PC code is now in C, but some C++ things are used internally.
+ Image was renamed to Texture
+ Rocket integration was added to the library properly
+ WFLCG random number generation library was added to the library
+ Default font was added


### Version 0.100.3-muffintrap "skrolli"
This version is the last one to be completely written in C++. To better integrate this library with other tools the next version will be in C except where C++ makes something much easier.

#### Changes
- Added the ufbx library and support for FBX files. Also adds a scene and material classes to better use those models
- Added a splash screen
- Added matcap material support for meshes
- Added Ogg playback on PC platforms (it is just loading the whole song to memory though)
- Removed glm library dependency, the ccVector header only library is now included in the library.

### Version 0.100.2-muffintrap "nova"

This is a big refactor that makes the library cross platform. The library can now be used on Windows, Linux, OS X(Mac computers) and Wii.
This is made possible with OpenGL and OpenAL that work on all of them.
The OpenGL is now the default way to draw anything. The files using GX are still in the framework, but
they can only be used in a Wii-only program.
There are lots of comments missing and things to tidy up (and Mac support is not really tested yet), but it
"should work"

#### New files
+ Added mgdl-rendering.h and .cpp for functions that help drawing in perspective.
+ Added mgdl-vector-operations.h and .cpp that contain overloaded vectors and utility functions for guVector
+ New cross platform files for all the main classes
+ New pc files for Win/Mac/Linux

#### Modifications to library
+ Fixed off-by-one errors in mgdl-font and mgdl-sprites
+ Added random number utility functions to mgdl-util
+ Added OpenGL as the rendering library
+ Modified example and template makefiles to use portlibs/wii in include and library path
+ Added functions Stop() and GetElapsedSeconds() to Sound class.

### Version 0.100.1-muffintrap "retrojam"
Fixes and little additions. Breaks SpriteSet API.

#### New files
+ Renamed project_template to project_example
+ Added simpler project_template
+ Added gitignore_example.txt to project_example and project_template

#### Modifications to library
+ Removed SpriteSetConfig and made LoadSprites to calculate dimensions from sprites per row and sprite height given as parameters.
+ Moved deltatime and elapsed to class variables in template.h. Elapsed time is measured in milliseconds
+ Added GetRoll() to WiiInput. Returns the roll of the wiimote in radians.
+ Modified install.sh: added checks and instruction
+ Renamed makefile.wii to Makefile, since there is no makefile.pc
+ Changed makefile to Makefile in project_template since Linux cares about case
+ Modified installation.txt to have instructions on KDevelop and libraries that need to be installed with pacman

#### Removals
+ Removed .gitignore from include/mgdl-wii/
+ Removed file install since there is also install.sh

### Version 0.100.0-muffintrap "instanssi"
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

