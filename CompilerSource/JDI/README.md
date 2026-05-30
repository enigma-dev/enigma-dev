#  JustDefineIt ⁂ [![Build Status](https://travis-ci.com/JoshDreamland/JustDefineIt.svg?branch=master)](https://travis-ci.com/JoshDreamland/JustDefineIt)

JustDefineIt is a C/C++ definition crawler. Its purpose is to harvest large amounts of information from C/C++ header files for use by modules outside of the compiler.

## Pardon our dust
This project is under heavy rennovation. It was designed mostly between 2008 and 2010, served its purpose in some capacity for a decade, and now is in need of refresh for C++11-20.

### Status:
- Master contains a new lexer (Jan2020) that should support everything up through C++20. Tests are written for some of the nichier caveats of the C preprocessor.
- Master has a *lot* of trouble with C++11-20. Missing features such as inline namespaces and variadic templates leave namespace std vacant when C++17 is set as the standard. The ENIGMA project works around this by asking G++ for its C++03 preprocessor configuration.
- Complicated specialization resolution is supported, but constexpr and parameter packs are not, so a lot of rennovation will be required there before fully-accurate comprehension can be achieved for new standards. Template comprehension through C++03 should be fine.
- Error reporting for tokens originating from macros is busted up pretty good.

## Goals
This parser is designed to be small and portable, in the sense that it is cheap to add into an existing project's code base. Its goal is to be capable and modular enough to meet a variety of needs, from code completion in IDEs to—with some extension—use as a complete C++ interoperablity layer. It should carry as few dependencies as possible while remaining small and simple to understand (unlike Clang, which depends on parts of LLVM that has proven difficult to separate).

The code is set up to compile in GNU G++, using all warning flags except -winline and -weff-c++. The -werr, -pedantic, and -pedantic-errors flags are also set. The project is carefully monitored for memory leaks before each commit, via Valgrind, although the switch to smart pointers has largely obviated the need for this.

Docs are not currently included as anyone with Doxygen can simply generate them. Still, the best documentation is the kind that compiles. See [`MAIN.cc`](https://github.com/JoshDreamland/JustDefineIt/blob/master/test/MAIN.cc) for examples of how to configure the tool.

## Building
We support CMake, and ship a Code::Blocks project file.
