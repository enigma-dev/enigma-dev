Command Line Interface
=======================
The following are instructions for building the engine on various platforms. If you have used the Windows zip installer or the python install script and have the required dependencies you can simply start the LateralGM IDE from a terminal with `java -jar lateralgm.jar` and do NOT!!! try open her by double clicking or right clicking and opening with JDK, she must be opened from a terminal. The first time LateralGM opens she will begin parsing ENIGMA's engine, the first time you hit run she will build the engine, the first time you hit debug she will compile the engine for debug mode with debug symbols for use with GDB. The first build takes a little bit, but after this you will not have to build the engine again, unless of course you plan on modifying its source code.

Dependencies
-----------------------
- Java Runtime Environment (JRE) Version 5 or higher, though we recommend the latest, if you intend to use the LateralGM IDE
On linux say the following on a terminal to determine your JDK version...
`java -version`
If you do not have Java installed, say the following on a terminal...
`sudo apt-get install java`
also...
`sudo apt-get install openjdk-`
and hit tab several times and it will list the available JDK packages you can install.
- ENIGMA also requires a few packages on Linux, say the following on a terminal to obtain them...
 `sudo apt-get install g++ zlib1g-dev libglu1-mesa-dev libalure-dev libvorbisfile3 libvorbis-dev libdumb1-dev`

Python Installation
-----------------------
If you have python installed already you need to say the following on a terminal...
`python install.py`
That will fetch the latest LateralGM IDE jar and its plugin for ENIGMA. If you do not have python installed, say the following on a terminal...
`sudo apt-get install python`

For Macintosh
-----------------------

For Windows
-----------------------

For Linux
-----------------------
Execute the following on a terminal...
```
cd $enigma-dev
make
```
You can also use `make clean` to clean make files.

Code::Blocks Project 
-----------------------
It is also possible to compile the engine, compiler, and command line interface via the Code::Blocks projects provided in each system.
