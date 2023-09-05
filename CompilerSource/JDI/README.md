JustDefineIt is a C/C++ definition crawler. Its purpose is to harvest large amounts
of information from C/C++ header files for use by modules outside of the compiler.

Project is currently in its early stages, and is progressing only as I find free time
to devote to it.

The makefile will come with project maturity; compilation is presently handled by the
Code::Blocks build system. If you wish to build the parser at this stage, it is best
to do so using the enclosed Code::Blocks project file.

This parser is designed to be small and portable, in the sense that it is easy to add
into an existing project's code base. Its goal is to be capable and modular enough to
meet a variety of needs, from code completion in IDEs to--with some extension--use as
a complete C++ lexer. It should carry as few dependencies as possible while remaining
small and simple to understand (unlike Clang, which depends on parts of LLVM that can
be difficult to separate).

The code is set up to compile in GNU G++, using all warning flags except -winline and
-weff-c++. The -werr, -pedantic, and -pedantic-errors flags are also set. The project
is carefully monitored for memory leaks before each commit, via Valgrind.

Docs are also not currently included as anyone with Doxygen can simply generate them.
When the project's size competes with that of its documentation, I will commit them.

