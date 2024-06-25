
# Forked by Muffintrap
The purpose of this fork is to:
* Continue and extend the original MTek-GDL library
* Remove deprecated or extraneous features
* Add functions and classes that are useful in Demoscene context
* Learn more about the Wii to maybe someday port Raylib to it.

All changes made to the original are clearly commented in the code, as required by the GPL-2.0 License.

If you use this fork for making a Wii game or demo, I won't be available to give tech support. And naturally, there is no warranty of any kind.

## How to compile and install the library and run the example with Dolphin
1. Install devkitpro following instructions here: [DevkitPro Getting Started](https://devkitpro.org/wiki/Getting_Started)
2. Add DEVKITPRO and DEVKITPPC to your environment variables. If you used the default install location, they should be:
	DEVKITPRO : /opt/devkitpro
	DEVKITPPC : /opt/devkitpro/devkitPPC
3. Using (dkp-)pacman, install the package **wii-dev**
4. Using (dkp-)pacman, install these packages if they are not already installed:
	* ppc-png
	* ppc-zlib
	* ppc-libvorbis
	* ppc-libvorbisidec
	* ppc-libogg
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
7. Go to mtek-gdl/example_project/ and run the command to compile it. It should produce a file called **example_project.dol**
```sh
	make
```
8. Open the Dolphin emulator, Select File > Open... and select the file **example_project.dol**


## Version numbers
This is now version 0.100.2-muffintrap "nova"

### Future plans
1. Add support for reading glTF scenes with tinygltf
2. Add Rocket integration for syncing
3. Add useful math and lerp functions
4. Add functions to help rendering 3D scenes and objects

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
