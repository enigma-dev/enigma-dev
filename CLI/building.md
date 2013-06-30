The following are instructions for building the Command Line Interface on different platforms. You may also build the CLI from the Code::Blocks project so long as you have the required dependencies.

Dependencies
-------------------
* The CLI requires the zlib package on Ubuntu for EGM files, say the following on a terminal...
`sudo apt-get install libzip-dev`
* test
#test
#test

For Macintosh
-------------------

For Windows
-------------------

For Linux
-------------------
Execute the following on a terminal...
```
cd $enigma-dev/CLI
make
```
You can also use `make clean` to clean make files.

Running
-------------------
To run the CLI, simply back out from the directory...
`cd ..`
And then execute the exe...
`./enigmacli`

It is important to note you must cd to enigma-dev folder and run the CLI from there.
