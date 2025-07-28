
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

## How to compile and install the library and run the example with Dolphin
1. Install devkitpro following instructions here: [DevkitPro Getting Started](https://devkitpro.org/wiki/Getting_Started)
2. Add DEVKITPRO and DEVKITPPC to your environment variables. If you used the default install location, they should be:
```sh
	DEVKITPRO : /opt/devkitpro
	DEVKITPPC : /opt/devkitpro/devkitPPC
```
3. Using (dkp-)pacman, install the package **wii-dev**
4. Using (dkp-)pacman, install these packages if they are not already installed:
	* ppc-libpng
	* ppc-zlib
	* ppc-libvorbis
	* ppc-libvorbisidec
	* ppc-libogg
	* wii-glu
	* wii-opengx
	* wii-freeglut
5. Clone this repository to your computer. In the next steps the folder of the repository is called **mtek-gdl/**
5. Go to mtek-gdl/3rdparty/libs-wii/ and run these commands. They will compile and install libraries that mtek-gdl uses.
```sh
	make
	sudo -E make install
```
6. Go to mtek-gdl/ and run these commands. They will compile and install the library itself.
```sh
	make
	sudo -E make install
```
7. Go to mtek-gdl/example_project/ and run the command to compile it. It should produce a file called **boot.dol**
```sh
	make
```
8. Open the Dolphin emulator, Select File > Open... and select the file **boot.dol**



## How to compile and install on Windows
### Development environment and libraries
1. Install [MSYS2](https://www.msys2.org/) (This is a different version from the one that comes with DevkitPro) [Detailed instructions for install](https://www.freecodecamp.org/news/how-to-install-c-and-cpp-compiler-on-windows/)
2. Launch the UCRT64 variant and update the packages as explained in the Detailed instructions above.
3. Install the compiler 
<code>pacman -S mingw-w64-ucrt-x86_64-gcc</code>
4. Install the required libraries:
<code>pacman -S mingw-w64-ucrt-x86_64-openal mingw-w64-ucrt-x86_64-freeglut mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-libsndfile make</code>

### Compiling the executable and getting the .dll files
1. Navigate to the github repository.
2. Run command <code>make -f Makefile_win</code>

####  Link to DLL files
- Add the folder <code>C:\msys64\ucrt64\bin</code> to PATH on windows. That way windows will look there for the dll files when running your executable.

#### These are the dll files that the program needs to run.
You need to include these files when you release your executable.
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

Windows makefile is Makefile_win.mk

```sh
make -f Makefile_win.mk
make -f Makefile_win.mk install
```

## How to compile and install on Ubuntu/Debian Linux:
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

## External libraries and resources

This library contains code from the following libraries under these licenses:
* [WFLCG] (https://github.com/WarpRules/WFLCG) MIT license
* [ufbx] (ufbx.github.io) MIT license 
* [rocket] (https://github.com/rocket/rocket) Zlib license
* [ccVector] (https://github.com/jobtalle/ccVector) Public Domain

This library contains the following font:
* [Toshiba Satellite 8x8](https://int10h.org/oldschool-pc-fonts/fontlist/font?toshibasat_8x8)

This library uses colors from this palette as part of the default palette:
* [Blessing] (https://lospec.com/palette-list/blessing)

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
