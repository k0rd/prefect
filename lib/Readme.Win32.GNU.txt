-------------------------------------
CRM32Pro SDK file package description
-------------------------------------

/docs: documentation on html format

/licenses: library licenses

/SDL: SDL include headers

History.txt: CRM32Pro SDK change log.

HowToInstall.txt: steps for installing the SDK.

Readme.Linux.txt: this file

CRM32Pro.h: main library header to include on your code. 
            It needs the SDL subdirectory (with all SDL includes).
            Supports Win32/Linux/MacOSX with Visual Studio, MinGW and GNU C.

CRM32Pro.gnu.dll: shared library to publish with your application. 
                  Only for Win32 with MinGW and GNU C.

CRM32Pro.gnu.lib: import library to be linked with your code. 
                  Only for Win32 with MinGW and GNU C.

sdlmain.gnu.lib: static library with WinMain definition to be linked with your code. 
                 Without output errors.
                 Only for Win32 with MinGW and GNU C.

sdlmainIO.gnu.lib: static library with WinMain definition to be linked with your code. 
                   With output errors.
                   Only for Win32 with MinGW and GNU C.

---------------------------------
Information updated: 18-August-2011


