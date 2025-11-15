
# Forked by Muffintrap
The purpose of this fork is to:
* Continue and extend the original MTek-GDL library
* Remove deprecated or extraneous features
* Add functions and classes that are useful in Demoscene context
* Learn more about the Wii to maybe someday port Raylib to it.

All changes made to the original are clearly commented in the code, as required by the GPL-2.0 License. Also the github commit history shows what has been changed.

If you use this fork for making a Wii game or demo, I won't be available to give tech support. And naturally, there is no warranty of any kind.

## Version number
This is version 0.100.4-muffintrap "assembly"

# Installation
This library allows to use the same codebase to make executables for Wii, Windows, Mac and Linux.
The first part is installing the environment for creating executables for the Wii.
After that are the different platforms. If you only ever want to make your demo/game work on Wii, you don't have to install the Win/Mac/Linux tools.

## How to compile and install the library and run the example with Dolphin and Wii
1. Install devkitpro following instructions here: [DevkitPro Getting Started](https://devkitpro.org/wiki/Getting_Started)
2. Add DEVKITPRO and DEVKITPPC to your environment variables. If you used the default install location, they should be:
```sh
	DEVKITPRO : /opt/devkitpro
	DEVKITPPC : /opt/devkitpro/devkitPPC
```
3. Using (dkp-)pacman, install the package **wii-dev**
4. Using (dkp-)pacman, install these packages if they are not already installed:
```sh
	ppc-libpng ppc-zlib ppc-libvorbis ppc-libvorbisidec ppc-libogg wii-glu wii-opengx wii-freeglut
```
5. Clone this repository to your computer. In the next steps the folder of the repository is called **mtek-gdl/**
6. Go to mtek-gdl/3rdparty/libs-wii/ and run these commands. They will compile and install libraries that mtek-gdl uses.
```sh
	make
	sudo -E make install
```
7. Go to mtek-gdl/ and run these commands. They will compile and install the library itself.
```sh
	make
	sudo -E make install
```
8. Go to mtek-gdl/example_project/ and run the command to compile it. It should produce a file called **boot.dol**
```sh
	make
```
9. Open the Dolphin emulator.
* Open Configuration and select the Wii tab.
* Configure SD card folder to be the example_projec folder
* Pack the SD card so that the exe can see the assets
* Select File > Open... and select the file **boot.dol**


## How to compile and install tools to make Windows executables using MSYS2
### Development environment and libraries
1. Install [MSYS2](https://www.msys2.org/) (This is a different version from the one that comes with DevkitPro) [Detailed instructions for install](https://www.freecodecamp.org/news/how-to-install-c-and-cpp-compiler-on-windows/)
2. Launch the UCRT64 variant and update the packages as explained in the Detailed instructions above.
3. Install the compiler 
<code>pacman -S mingw-w64-ucrt-x86_64-clang</code>
4. Install the required libraries:
<code>pacman -S mingw-w64-ucrt-x86_64-openal mingw-w64-ucrt-x86_64-freeglut mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-libsndfile make</code>

####  Link to DLL files
- Add the folder <code>C:\msys64\ucrt64\bin</code> to System PATH on windows. That way windows will look there for the dll files when running your executable.

#### These are the dll files that the program needs to run.
You need to include these files when you release your executable. There is a ready made copy_dll_files.bat file that does this in example_project/windows_release/
- libgcc_s_seh-1.dll
- libfreeglut.dll
- libopenal-1.dll
- libpng16-16.dll
- libsndfile-1.dll
- libstdc++-6.dll
- libwinpthread-1.dll
- zlib1.dll
- libFLAC.dll
- libmpg123-0.dll
- libmp3lame-0.dll
- libogg-0.dll
- libvorbis-0.dll
- libopus-0.dll
- libvorbisenc-2.dll
- libvorbisfile-3.dll

### Compiling the framework
1. Navigate to the github repository.
2. Run commands:

```sh
make -f Makefile_win.mk
make -f Makefile_win.mk install
```

Now you should be able to compile the example project also to Windows by giving the command
```sh
make win
```
in the mtek-gdl/example_project directory

## How to compile and install tools to make Windows executables without MSYS2
You need the following libraries:
    - [FreeGLUT](https://www.transmissionzero.co.uk/software/freeglut-devel/)
    - [OpenAL](https://openal.org/downloads/)
    - [libsndfile](https://github.com/libsndfile/libsndfile)
    - [libogg & libvorbis](https://xiph.org/downloads/)

Libogg and libvorbis need to be built. They include Visual Studio projects. Libvorbis requires libogg.

The included Visual Studio project expects to find these libraries in C:\Dev. If you place them somewhere else you need to modify the project.

Preprocessor defines for Windows build:
```
MGDL_PLATFORM_WINDOWS
MGDL_PLATFORM="Windows Native"
MGDL_WINDOWS_NATIVE
MGDL_USE_CCVECTOR
UFBX_REAL_IS_FLOAT
_XKEYCHECK_H
```
These .dll files need to be in the same folder with the executable when giving it to someone else to play:
- freeglut.dll
- OpenAL32.dll  ( C:\Windows\SysWOW64 )
- sndfile.dll

## How to install the framework and create executables on Ubuntu/Debian Linux:
### List of needed packages:

These packages are needed for C/C++ Development in general:
- build-essential

Mtek-GDL makefiles for linux use clang: (but g++ works too)
- clang

Mtek-gdl uses these libraries:
- libsndfile1-dev
- libopenal-dev
- freeglut3-dev
- libpng-dev

Linux makefile is Makefile_linux.mk, use it with:
```sh
make -f Makefile_linux.mk
make -f Makefile_linux.mk install
```

### Rocket editor
These libraries are needed if you build the rocket editor on Linux

- libqt5websockets5-dev
- qml-module-qtwebsockets

## How to compile and install on MacOS

### Programs
Open a terminal and give the following commands

> % xcode-select --install
This will open a pop-up asking if you want to install command line tools. Click Install.

### Libraries
Install homebrew or MacPorts. Homebrew might not be available if your Mac is too old :(

**MacPorts**
MacPorts can be found at [www.macports.org](www.macports.org)

**Homebrew**
Homebrew can be found at [brew.sh](brew.sh)

**Libraries**
After installing, install these ports:
- libpng
- libsndfile
- libogg
- libopus
- libvorbis

MacOS makefile is Makefile_mac.mk
```sh
make -f Makefile_mac.mk
make -f Makefile_mac.mk install
```

## Documentation and examples
* [Doxygen generated Documentation](https://muffintrap.github.io/mtek-gdl/html/index.html)

### Demos and games done using the framework
These all use different versions and won't compile directly with the current version, but they can still be useful as complete examples
on how to organize things. Newest projects first.

**[Neko City] (https://github.com/MuffinTrap/nova25)**
Demo for NOVA 2025. [Demozoo](https://demozoo.org/productions/373529/) *mtek-gdl 0.100.3*

**[NekoStation] (https://github.com/MuffinTrap/graffathon25)**
Little intro for Graffathon 2025. This demo inspired the addition of random number generation. [Demozoo](https://demozoo.org/productions/373273/) *mtek-gdl 0.100.3*

**[Marmot Spacelines 2024] (https://github.com/MuffinTrap/wii-inercia24)**
Demo for Inercia 2024. FBX model loading and matcap materials. [Demozoo](https://demozoo.org/productions/362559/) *mtek-gdl 0.100.2*

**[A Keir for Summer's End] (https://github.com/MuffinTrap/wii-skrolli24)**
Demo for Skrolli Party 2024. Rocket integration in the framework and callbacks. [Demozoo](https://demozoo.org/productions/357207/) *mtek-gdl 0.100.2*

**[NUMBERS] (https://github.com/MuffinTrap/particle-demo)**
Demo for Assembly 2024 by Marmot. First project to have cross platform compilation. [Demozoo](https://demozoo.org/productions/354174/) *mtek-gdl 0.100.1*

**[Frog Snack] (https://github.com/MuffinTrap/retro-ribbit)**
Game for Retro Game Jam 2024. *mtek-gdl 0.100.0*

**[Wii & Chill] (https://github.com/MuffinTrap/combochiller)**
First Demo I did for the Wii. [Demozoo](https://demozoo.org/productions/345939/) *mtek-gdl 0.100.0*

**[Pyromancer] (https://github.com/MuffinTrap/pyromancer)**
Incomplete game for Ludum Dare 55. *mtek-gdl 0.100.0*

## External libraries and resources

This library contains code from the following libraries under these licenses:
* [WFLCG](https://github.com/WarpRules/WFLCG) MIT license
* [ufbx](ufbx.github.io) MIT license 
* [rocket](https://github.com/rocket/rocket) Zlib license
* [ccVector](https://github.com/jobtalle/ccVector) Public Domain
* [stb](https://github.com/nothings/stb) Public Domain

This library contains the following font:
* [Toshiba Satellite 8x8](https://int10h.org/oldschool-pc-fonts/fontlist/font?toshibasat_8x8)

This library uses colors from this palette as part of the default palette:
* [Blessing](https://lospec.com/palette-list/blessing)

This file was changed for version 0.100.0-muffintrap by muffintrap to add explanation of the fork.
Original contents of README.md are below:

# MTek-GDL
A cross-platform game development framework library for the Nintendo Wii and PC with an API very similar to SFML designed mainly to make homebrew software and game development across supported platforms much easier and straight forward to do for both beginners and advanced programmers. Designed mostly for 2D game development but lower level GX or OpenGL commands can be used in conjunction for advanced 2D or 3D graphics programming.

This repository houses the most recent version of the MTek-GDL libraries straight from the original author and contributions by other people who are willing to help improve this library and often has features that are not yet present in the official releases of this library. However, beware of possible syntax changes if you're working on an existing project that uses an older version of this library and expect some bugs when using the newly added features. Avoid using undocumented functions as they are usually not finalized by the original author of this library and will most likely undergo significant changes.

If you're new to MTek-GDL, it is highly recommended that you start out with one of the official release packages which can be found right here: http://www.wiibrew.org/wiki/MTek-GDL

# Documentation
Documentation of the library can be found as Doxygen blocks inside the header files. It is recommended to parse said files through Doxygen so you'll get yourself an HTML manual of the library.

Example programs of this library can be found in this repository:
https://github.com/Lameguy64/mtek-gdl-examples

# Setting up the Wii Library
Assuming that you already have DevkitPro with DevkitPPC already set up and ready to use, you first have to install the libraries provided within 3rdparty/libs-wii by simply running the makefile inside it. Once all the libraries have finished compiling, install them to your DevkitPPC toolchain by entering 'make install'.

Now that you have the 3rd party libraries MTek-GDL needs to work installed, run makefile.wii by entering 'make -f makefile.wii'. Once compiling has finished, you're all set to use MTek-GDL in your projects.

You may either install the library to your DevkitPPC environment by entering 'make -f makefile.wii install' to the make program. However it is recommended to just set appropriate search directories to your project's makefile instead of installing it into your development environment.
