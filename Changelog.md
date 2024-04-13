# Changes made by muffintrap

### 13.4.2024
Bringing a lot of changes to the code.

+ Added install.bat from the 2.6 release.
+ Added install.sh to help installing on Linux systems.
+ Added installation.txt from the 2.6 release and modified it.
+ Added readme.txt from the 2.6 release and modified it.
+ Added LoadImageBuffer to mgdl-image.cpp and .h to load pngs from buffers made by bin2obj utility. 
+ Added makefile from the 2.6 release.
+ Modified mgdl-input-keyboard.cpp to fix a compilation error.
+ Modified mgdl-main.cpp function _resetcallback to fix a compilation error caused by ogc sending two parameters for _resetCallback. Note: ogc2 does not send any, so they probably are not important :D
+ Modified mgdl-texture.cpp. Removed register keyword from a variable since C++17 does not allow it. Added check for NULL before freeing mipmapBuff[1] in _ConvertMipmap()
+ Modified mgdl-main.h. Added overload for SetClearColor() that takes a single color. 
+ Modified mgdl-sprites.h and .c. Added a function that load sprites from Image and configuration struct.
+ Added file mgdl-util.h and .c
