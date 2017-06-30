# setup guide
* Create VS C++ win32 console project
* Right click project > properties > general
* Add **../../../include** into Additional Include Directories
* In properties go to linker > general
* Add **../../../lib/win32;** into Additional Library Directories
* In properties go to linker > input
* Remove everything in Additional Dependencies and replace with **edk.lib**
* ???
* profit

**Note: This is for setting up for a project within the SDK itself, if you create a project outside the SDK you will need to change the directories.**

Include is in emotiv-sdk > include

win32 is in emotiv-sdk > lib > win32
