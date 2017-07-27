# Software Requirements
* Anaconda 2 32-Bit https://repo.continuum.io/archive/Anaconda2-4.4.0-Windows-x86.exe
* Atom Text Editor (recommeneded) https://atom.io/
* Visual Studio (2015 recommended) https://www.visualstudio.com/downloads/
* NAO Robot Pyton SDK http://doc.aldebaran.com/1-14/dev/python/install_guide.html#python-install-guide

# Hardware Requirements
* NAO Robot
* Emotiv EPOC+ headset

# Atom Setup Guide
* Install packages: **atom-pyton-run**, **autocomplete-python**, **python-indent**, **python-tools**
* Open settings for **atom-python-run**, for 'Command of F5' enter the directory for python.exe in Anaconda to e.g **D:\Anaconda2x86\python.exe "{file}"**

# Visual Studio Setup Guide
* Create VS C++ win32 console project in the **src** folder
* Delete the resources, headers and sources folders.
* Make a new C++ class in the project and call in main, delete the header file.
* Right click project > properties > general
* Add **../../../emotiv-sdk/bin/win64** into Output Directory
* In properties go to C/C++ > general
* Add **../../../emotiv-sdk/include** into Additional Include Directories
* In properties go to linker > general
* Add **../../../emotiv-sdk/lib/win64;** into Additional Library Directories
* In properties go to linker > input
* Remove everything in Additional Dependencies and replace with **edk.lib**

Include is in emotiv-sdk > include
win32 is in emotiv-sdk > lib > win32

# Facial Commands
* Neutral
* Blink
* Wink left
* Wink right
* Raise brow (surprise)
* Furrow brow (frown)
* Smile
* Clench (teeth clench)
* Laugh

# Mental Commands
* Neutral
* Push
* Pull
* More...
