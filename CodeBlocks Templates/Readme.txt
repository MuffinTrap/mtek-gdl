These CodeBlocks templates will help make it easier to create a new Wii or PC project with MTek-GDL as these
templates already have the neccessary libraries and files already configured, saving you from the hassle of
setting up MTek-GDL for a new project yourself.

Take note that the Wii template assumes that you have DevkitPro with DevkitPPC in the root directory of drive
C: as well as the mtek-gdl directory too.


Installation (Windows):

  - Open your CodeBlocks Application Data folder by entering %APPDATA% in the path box on the File Explorer
    window. If you cannot find the CodeBlocks folder in there, you must run CodeBlocks at least once for it to
    be created but don't leave it running while applying the templates.

  - Copy the share and UserTemplate folders from the template-win folder into the CodeBlocks Application Data
    folder, click Yes or Continue when warned about overwriting the two folders.

  - Open the CB Share Config program that came with Codeblocks. Click the 3 dotted button on the destination
    configuration panel on the right and select default.conf then click the other 3 dotted button on the source
    configuration panel on the left and select compiler.conf inside the template-win folder.
	
  - Check the item on the left that says <compiler><user_sets><devkitppc_gnu_gcc_compiler_for_nintendo_wii>
    then click the Transfer button. Click yes when prompted for a confirmation and once it has been transferred
    successfully, click Save then click yes when prompted.

  - Your now set, simple as that. Just head to the User Templates menu when creating a new project and select
    which project template you want to use. If you get warning messages saying that no valid debugger is set for
    the DevkitPPC compiler, just select GDB/CDB as your active debugger to remove the warnings.


Notes (Wii):

  - The DevkitPPC GNU GCC compiler option already has the core system libraries (libogc, libfat, etc) already added.
    You may want to customize the custom compiler option yourself to suit your preferences if you're an advanced
    programmer.

  - If you create a new project using the 'DevkitPPC-Wii Project with MTek-GDL' user template with a project name
    other than the default name provided, you'll get an error in the post-build steps when compiling... This is
    due to a bug with CodeBlocks where the PROJECT_NAME variable was not updated to reflect to the new name but it
    can easily be fixed by opening the Build Options dialog then clicking Ok.

  - The post-build script is to allow immediate execution of your successfully compiled project when pressing the
    compile key (F9) since the Run step cannot be customized in CodeBlocks. The IDE will still give out a Failed
    to Execute message but it can be ignored.