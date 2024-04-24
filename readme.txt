MTek-GDL (Wii & PC) Release 2.6 by TheCodingBrony/Lameguy64
2015 Meido-Tek Productions


About:

    MTek-GDL is a simple object-oriented game development library written in C++ designed to make
    homebrew game development on the Nintendo Wii a heck of a lot easier to do. It features hardware
    accelerated 2D graphics functions, image/texture loading, a built-in sprite system and music/sound
    playback capabilities. This library can also be used as a base for developing custom 2D or 3D game
    engines to extend the capabilitiesof this library provided that you're familiar with the GX graphics
    API for the Nintendo Wii.

    The main goal of this project is to provide Wii homebrew programmers a fast, simple, efficient yet
    easy to understand library for developing games on the Nintendo Wii platform as part of an attempt
    to bring back the Wii homebrew game development scene once again while providing an option for
    developers to be able to port their projects over to the PC with minimal code changes (unless you
    use custom graphics code).

    Being that this project is still fairly new, you must have at least a decent amount of experience
    in C++ programming as there is barely any documentation of this library for now and that there are
    not that many examples made for it in the mean time, especially when there's only one person working
    on this as a hobby project. Hopefully, the library will slowly but surely improve eventually and
    helpful contributions are greatly appreciated.


Features:

    - Simple object-oriented based programming interface inspired from SFML for handling images, sprite
       sets, fonts and sound effects.

    - Simplified system initialization with automatic video mode selection and aspect ratio correction.

    - Almost all graphics functions are hardware accelerated for maximum performance.

    - Stream music off of OGG files with little CPU and memory usage.

    - Simple sound system with volume controls and 2D stereo separation (might be improved upon in the
    future).

    - Can be used as a base for developing custom graphics engines without having to write the basic
      stuff from scratch.

    - Includes a PC port of the library using SFML and GLEW so you can port your game project to the PC
      with ease (depends on how you write your code though).

    - And more!


Questions and Answers (more entries will be added in the future):

    Q: Are there any plans for this library to be ported to other platforms besides PC?
    A: I don't have any plans on porting this library to other console platforms as my current focus on
       this library is the Nintendo Wii and PC versions. And no, there will be no DS or 3DS version
       whatsoever.

    Q: What's up with the pony stuff featured in one of the examples?
    A: Because I'm one of dem' bronies... But at least I'm not one of those abnoxious stereotypes who
       tend to be really annoying at times.

    Q: Now I absolutely hate this library because it was made by a freaking brony.
    A: Oh well, haters gonna hate. But you're going to miss out on the potentiality of this library in
       the future.

    Q: Are there any tutorials regarding on using this library?
    A: There are plans but the lack of a good enough following makes it tough to make such a thing.
       Hopefully, the included Wii examples are good enough to get you started on making your own games
       provided that you have a decent amount of experience in C++ programming.

    Q: Where are the instructions for compiling the PC version of the library on Linux & Mac systems?
    A: There's none for now as I haven't tried compiling this library on Linux and Mac platforms yet.
       If you're programming stuff for those platforms, you'd most likely figure it out yourself anyway.

    Q: Is there a static library version of this?
    A: Not yet as the library has not yet been finalized and turning it into a static library will only
       make it harder to debug and continue development on.

    Q: Why does the Wii version of this library only support OGG Vorbis files for music?
    A: Because it is the best streaming audio format that I could get to work properly on the Wii.
       Besides, OGG Vorbis has a better quality-to-space ratio especially on bitrates as low as 128Kbit
       plus, a lot of indie games use this audio format for music as well.

    Q: Why are most of the graphics examples only specific to the Nintendo Wii platform?
    A: Because there are already tons of OpenGL and SFML examples and tutorials all over the internet
       which you can easily adapt to MTek-GDL PC. Also, there are not that many examples on how to do 3D
       graphics on the Nintendo Wii as far as I'm aware.

    Q: I've found a strange bug with the library, what should I do?
    A: First, you must make sure that the bug is not caused by your own code. If it isn't and is indeed a
       bug in the library, report it as an issue in the official forum thread.

    Q: Why does MTek-GDL not handle input for both platforms (Wii and PC)?
    A: Because both platforms have different input device options and/or APIs (Wii has a 
       Wiimote+Nunchuk/Classic Controller and GC gamepads while PC has a keyboard, mouse, and USB gamepads).
       Wrapping all of those devices into MTek-GDL for both platforms would be very difficult for both the
       developer of the library and the end user and is just not practical. It is best to just write your
       own input wrapper instead and its not that hard anyway.

    Q: When the Wii is set to widescreen mode, is it really running in 854x480/576 mode?
    A: No, the Wii's graphics processor has a maximum framebuffer width of 640 pixels making it impossible
       to achieve 'true widescreen' modes. Instead, Widescreen is done anamorphically by emulating an
       854x480/576 screen through a projection matrix and tweaking the video registers to eliminate the
       borders which works well in most cases. However, if you use Low Resolution mode, a true widescreen
       resolution of 432x240 on NTSC or 432x264 on PAL is used instead.

    Q: Why is the PC version of the library not as good as the Wii version?
    A: Because currently, most of the development effort goes into the Wii version and the PC version is
       just a side-line to help simplify porting Wii game projects onto the PC.

    Q: Why when I specify something like gdl::LN_MM_LN to the gdl::Image::LoadImage() class, it does not
       work properly.
    A: Use the gdl::Image::LoadImageMipmap() function for mipmapped images instead.


Wii Programming Tips and Recommendations (more entries will be added in the future):

    - Do not use absolute file paths (such as /data/mystuff/myfile.dat)! Instead, only specify paths from
      where your Wii program will reside (such as mystuff/myfile.dat) and put your files and directories
      next to it. When executing applications through a network or USB Gecko, it's current folder is the
      root of the card/drive.

    - Use malloc2()/free2() for handling large amounts of data since MEM2 has 64MB (51.8 usable) while
      MEM1 only has 24MB (really Nintendo?).

    - If you use malloc2()/free2() and/or textures and images stored in MEM2, avoid exceeding the MEM1
      memory area with plain malloc()/free() to avoid corrupting what was stored inside MEM2 unless you
      use the included patched version of libogc which prevents malloc() from jumping into MEM2 when MEM1
      becomes full.


License Stuff:

    MTek-GDL is protected by the GNU GPL license which means you can freely use it, study, copy, modify
    and redistribute the product provided that you follow the following rules:

    - The origin of the library and it's source code must not be represented.
    - Modified versions of this library must be plainly marked as such and must not be represented as the
      original source.
    - Projects that use this library must follow the same license.

    This library may not be used to develop closed-source commercial games/applications as it may violate
    the licenses of the 3rd party libraries used so better not use this library if you plan to make
    commercial indie games with it and just make your own game development library instead. Also, making
    commercial homebrew for the Nintendo Wii may result in you getting sued by the big N.


Credits:

    Nintendo Wii Version:
    - Some bits of code used were taken from GRRLIB by the GRRLIB team and libprofile by Ian Callaghan.
    - 'zlib' general purpose compression library by Jean-loup Gailly and Mark Adler.
    - 'png' library by Glenn Randers-Pehrson.
    - 'tremor' Vorbis codec library by Xiph.Org foundation.
    - 'oggplayer' library by Francisco Muñoz 'Hermes'.
    - 'dolexec' library made by me and is based from FIX94's loadMii forwarder.

    PC Version:
    < currently unavailable >

    Both ports of this library were developed by Jobert 'Lameguy' Villamor (TheCodingBrony/Lameguy64) of
    Meido-Tek Productions.


Changelog:

    Release 2.6 (07-12-2015):

    Wii library v0.66 changes:

        - Fixed major low-resolution mode bug when console is set for progressive-scan mode and
          gdl::PrepDisplay() not resetting the projection matrix.

    Release 2.5 (07-03-2015):

    - PC library dropped in this release due to major overhaul reasons. Most people who use MTek-GDL use
      the Wii version of this library anyway.
    
    - Added library installer batch file.

    - Wii library manual is now in a standalone .chm format file, fixed up the formatting a bit and made
      major corrections with the Wii specs documentation.

    - Included MTek-GDL logo as an .svg vector file for customizability.

    - Updated wiimote.c controller handler in the examples to use a much more efficient way to query
      wiimote data.

    - Added slightly modified libogc library which disables memory allocation routines from jumping into
      MEM2 if MEM1 becomes full making dedicated MEM2 memory management much safer to do in memory
      intensive projects and a possible video init bug fix when the program is run on a WiiU under Wii
      mode.

    - Added MTek-GDL templates for CodeBlocks to ease creating projects with the Wii version of this
      library.

    - Spritesets and Whack-a-Cirra examples now run in low-res mode and fixed a bug in Whack-a-Cirra where
	  if you plot a Cirra at the bottom most part of the screen, the game will lock up.
	  
    - Increased oggplayer thread priority slightly to fix audio stutter issues when the main program is
      processing too much (one example is killing a boatload of Cirras in the Whack-a-Cirra minigame).

    Wii library v0.65 changes:

	- Added gdl::SetErrorCallback() and gdl::CallErrorCallback() functions for custom error handling.
		
        - Added malloc2() and free2() alias macros for valloc() and vfree() as it makes a lot more sense
          naming wise.

        - Added out of memory checks in the gdl::Texture and gdl::Image classes.

        - The library can now be compiled as a binary library which makes compile times much faster and
          it improves documentation formatting as well. The old #include method will still work but your
          project will not compile as fast as with the binary version of the library.

        - Added gdl::Texture::CreateMipmapped(), gdl::Image::CreateMipmapped() and
          and gdl::Image::LoadImageMipmapped() functions which finally adds built-in mipmap support to the
          librrary.

        - Fixed a bug in gdl::FFont class where the characters appear to be touching each other.

        - Added gdl::Font::LoadFontMem() function.

        - Added gdl::wii::ComponentCableUsed() function.

        - Added gdl::wii::SetNonInterlaced() function.

        - Added gdl::wii::LowResolutionMode flag (true if system is running in low-resolution mode).

        - Added gdl::wii::Progressive flag (true if system is running in progressive scan mode).

        - Added gdl::wii::Widescreen flag (true if system is in widescreen).

        - Added gdl::DrawPoint() function.

        - Added a fix that makes lines draw properly in low-res mode but make sure you set the line width
          to 8 if you changed it.

        - Added optional extra parameter for gdl::Font::CalcStrLen() function (see docs for details).

        - Finally added real-time delta calculation so if you've paid attention to gdl::Delta, your program
          is now framerate independent. For best results, intensive routines must be placed between a
          gdl::PrepDisplay() and gdl::Display() pair for the delta value to be calculated properly.

        - Fixed some incorrectly defined color define values.

        - Fixed minor memory leak bug in gdl::Image::LoadImage() and removed default handlers as specifying
          0 in the format parameter conflicted with specifying texture format gdl::I4.

        - Fixed a bug inside gdl::Texture::LoadTexture() where it won't close an opened file handle if it
          failed to load a texture file.

    Release 2 (03-20-2015):

    - Added DirectQB font support through the new gdl::Font class.

    - Fixed some framework design issues.

    - Removed quite a lot of commented out dead code.

    - Fixed some typos in the documentation.

    - Removed gdl::ShowLogo() function (it was useless for the most part).

    Wii library v0.52b changes:

        - Optimized image, sprite, and text drawing code a bit and removed gdl::wii::DefaultCol struct.

        - Low-resolution mode can now be in progressive scan mode (through render-to-texture techniques)
          mainly to fix 'input lag' due to HDTVs trying to deinterlace the low-resolution video.

        - Framebuffers are now cleared during initialization.

        - Tremor library now in source form.

        - Fixed possible playback bug in oggplayer library when playing mono OGG files (whoops!) as well as the
          very brief reverse stereo click bug.

        - Removing Gamecube compatibility, especially when there's hardly anyone doing homebrew on the Gamecube
          nowadays.

        - Organized library functions into several .cpp files.

        - Sound data is now stored in MEM2 (using valloc()/vfree() functions).

        - All graphics functions now use GX_ClearVtxDesc() when reconfiguring the vertex descriptor which helps make
          advanced graphics programming less prone to annoying crashes due to stray vertex descriptors.

    PC library v0.48b changes:

    - Code fixes to eliminate 'missing braces' compiler warnings.

    - Framerate limiter now set automatically (fixes jitter issues on certain graphics cards and heavy CPU usage).

    - Optimized sprite and text drawing code.

    - Fixed a bug in gdl::SpriteSets class which didn't delete vertex buffer objects when it was deconstructed.

    - Fixed frame delta calculation issues (when moving the window while your game is running).

    Release 1.1 (1-25-2015):

    - Minor Wii library update.

    - Wii library documentation for gdl::Texture::ConvertRawImage() is now up-to-date (forgot about it in relese 1).

    - Corrected some entries in the Wii library documentation.

    Wii library v0.43b changes:

        - Made low-res mode more HDTV friendly by enabling interlace jitter when component cables are used (after
          having a debate with tueidj).

        - Removed useless *colTable parameter in gdl::Texture::ConvertRawImage().

    Release 1 (01-22-2015):

    - PC version of the library now included (there might still be some hiccups here and there as most of the
      development effort went to the Wii version of the library).

    - Sprite Mapper tool now included (it is kinda bloaty as it was recycled from an ill-fated game engine I 
      made with QB64 2 years ago).

    - Created and added dolexec forwarder library based on FIX94's loadMii forwarder (no example of it for now
      but its EXTREMELY easy to use, no joke... Not defined in the template makefile though but its easy to
      implement anyway.

    - Forgot to include bulb.ply for the wii/graphics/3dsample example... Silly me!

    - Added spritesets example (WARNING: features ponies).

    - Added a silly whack-a-cirra minigame example.

    - Added wii/graphics/mipmap example.

    - Added wii/graphics/tilemap example.

    - Added wii/graphics/displaylist example.

    Wii library v0.42b changes:

        - Added built-in power off/reset handling with fade-out (only effective during a gdl::PrepDisplay()/
          gdl::Display() loop). Pressing the reset button won't do a hot-reset (restarts the running application)
          but instead, brings you back to the Homebrew Channel. This can be overridden with SYS_SetResetCallback()
          and SYS_SetPowerCallback().

        - Added non-interlaced low resolution mode (320x240/432x240 on NTSC/MPAL/EURGB60, 320x264/432x264
          on PAL) which will do great for retro styled games or emulators. Will always be recognized as 480i
          even with component cables.

        - Added gdl::Texture::LoadTexture() function supporting single texture TPL files... Mipmapped textures
        are accepted but color-index textures and TPL files containing more than 1 texture are not supported.

        - Added TPL texture file support in gdl::Image::LoadImage()... Filter mode and texture format parameters
        specified to the function are ignored when loading TPL files.

        - Added I4, I8, IA4, and IA8 texture format support for the gdl::Texture and gdl::Image classes.

        - gdl::Texture::ConvertFromRaw() and gdl::Image::LoadImage() now uses MEM2 for storing temporary pixel data
          to help prevent malloc from jumping into MEM2 when loading lots of large images, can be reverted back to MEM1
          allocation by specifying -DGDL_GC_COMPAT when compiling.

        - gdl::Image::LoadImage() now supports 8-bit palettized PNG images but it'll be converted into an RGB image
          upon loading.

    Release 0 (01-01-2015):

    - Initial "New Year's Eve" release.