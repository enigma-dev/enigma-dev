# ENIGMA

ENIGMA, the Extensible Non-Interpreted Game Maker Augmentation, is an open source cross-platform game development environment derived from that of the popular software Game Maker. Its intention is to provide you with a quality game creation tool and a bridge between high- and low-level programming languages. It can be used either through an IDE, namely, its sister project, LateralGM, or through a Command line interface.

Like Game Maker, ENIGMA comprises both an easy to use Drag & Drop system as well as its own programming language. This programming language, known as EDL, is essentially a mix between C++ and Game Maker's GML. Part of ENIGMA's goal is to remain backwards compatible with Game Maker, serving for some intents and purposes as a Game Maker compiler, however, EDL offers many very powerful features which simply aren't present in the alternative. Such features include the ability to compile DLLs and other C/C++ scripts right into the program and access C++ types, templates, and functions.

Although EDL adopts GML's very lax syntax in its scripting, the inputted code is actually parsed and translated into valid C++. This compilation improves size and performance of the language by incredible amounts, while almost magically maintaining its simplicity of use.

## Install
In order to keep the ENIGMA repository small and fast binary files are excluded. To install you will need to obtain the binary files for your chosen platform.

### MACOSX
After cloning the repository run the following command in terminal to install the main binary files

python install.py mac
