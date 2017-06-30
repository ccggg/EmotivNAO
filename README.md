# setup guide
* Create VS C++ win32 console project in the **src** folder
* Delete the resources, headers and sources folders.
* Make a new C++ class in the project and call in main, delete the header file.
* Right click project > properties > general
* Add **../../../emotiv-sdk/bin/win32** into Output Directory
* In properties go to C/C++ > general
* Add **../../../emotiv-sdk/include** into Additional Include Directories
* In properties go to linker > general
* Add **../../../lib/win32;** into Additional Library Directories
* In properties go to linker > input
* Remove everything in Additional Dependencies and replace with **edk.lib**

**Note: This is for setting up for a project within the SDK itself, if you create a project outside the SDK you will need to change the directories.**

Include is in emotiv-sdk > include

win32 is in emotiv-sdk > lib > win32

# todo list
* Take input of the emotiv 'commands': clench, wink, blink, etc.
* send command to nao robot.
* robot will output what you're doing.
