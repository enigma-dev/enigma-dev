#!/bin/sh
cd "${0%/*}";
if [ `uname` = "Darwin" ]; then
  gmake -C "." Game WORKDIR="${0%/*}/" CODEGEN="${0%/*}/stigma/" GMODE="Compile" GRAPHICS="OpenGL1" AUDIO="SDL" COLLISION="Precise" WIDGETS="None" NETWORKING="None" PLATFORM="SDL" TARGET-PLATFORM="MacOSX" CC="clang" CXX="clang++" MAKE="gmake" COMPILEPATH="MacOSX/MacOSX/Clang" EXTENSIONS=" Universal_System/Extensions/HideErrors Universal_System/Extensions/Alarms Universal_System/Extensions/Paths" OUTPUTNAME="${0%/*}/build/Key to Success"
elif [ $(uname) = "Linux" ]; then
  make -C "." Game WORKDIR="${0%/*}/" CODEGEN="${0%/*}/stigma/" GMODE="Compile" GRAPHICS="OpenGL1" AUDIO="SDL" COLLISION="Precise" WIDGETS="None" NETWORKING="None" PLATFORM="SDL" TARGET-PLATFORM="Linux" CC="gcc" CXX="g++" MAKE="make" COMPILEPATH="Linux/Linux/GCC" EXTENSIONS=" Universal_System/Extensions/HideErrors Universal_System/Extensions/Alarms Universal_System/Extensions/Paths" OUTPUTNAME="${0%/*}/build/Key to Success"
elif [ $(uname) = "FreeBSD" ]; then
  gmake -C "." Game WORKDIR="${0%/*}/" CODEGEN="${0%/*}/stigma/" GMODE="Compile" GRAPHICS="OpenGL1" AUDIO="SDL" COLLISION="Precise" WIDGETS="None" NETWORKING="None" PLATFORM="SDL" TARGET-PLATFORM="FreeBSD" CC="clang" CXX="clang++" MAKE="gmake" COMPILEPATH="FreeBSD/FreeBSD/Clang" EXTENSIONS=" Universal_System/Extensions/HideErrors Universal_System/Extensions/Alarms Universal_System/Extensions/Paths" OUTPUTNAME="${0%/*}/build/Key to Success"
elif [ $(uname) = "DragonFly" ]; then
  gmake -C "." Game WORKDIR="${0%/*}/" CODEGEN="${0%/*}/stigma/" GMODE="Compile" GRAPHICS="OpenGL1" AUDIO="SDL" COLLISION="Precise" WIDGETS="None" NETWORKING="None" PLATFORM="SDL" TARGET-PLATFORM="DragonFlyBSD" CC="gcc" CXX="g++" MAKE="gmake" COMPILEPATH="DragonFlyBSD/DragonFlyBSD/GCC" EXTENSIONS=" Universal_System/Extensions/HideErrors Universal_System/Extensions/Alarms Universal_System/Extensions/Paths" OUTPUTNAME="${0%/*}/build/Key to Success"
elif [ $(uname) = "OpenBSD" ]; then
  gmake -C "." Game WORKDIR="${0%/*}/" CODEGEN="${0%/*}/stigma/" GMODE="Compile" GRAPHICS="OpenGL1" AUDIO="SDL" COLLISION="Precise" WIDGETS="None" NETWORKING="None" PLATFORM="OpenAL" TARGET-PLATFORM="OpenBSD" CC="clang" CXX="clang++" MAKE="gmake" COMPILEPATH="OpenBSD/OpenBSD/Clang" EXTENSIONS=" Universal_System/Extensions/HideErrors Universal_System/Extensions/Alarms Universal_System/Extensions/Paths" OUTPUTNAME="${0%/*}/build/Key to Success"
else
  make -C "." Game WORKDIR="${0%/*}/" CODEGEN="${0%/*}/stigma/" GMODE="Compile" GRAPHICS="OpenGL1" AUDIO="SDL" COLLISION="Precise" WIDGETS="None" NETWORKING="None" PLATFORM="SDL" TARGET-PLATFORM="Windows" CC="gcc" CXX="g++" MAKE="make" COMPILEPATH="Windows/Windows/GCC" EXTENSIONS=" Universal_System/Extensions/HideErrors Universal_System/Extensions/Alarms Universal_System/Extensions/Paths" OUTPUTNAME="${0%/*}/build/Key to Success.exe"
fi
