#!/bin/sh
cd "${0%/*}"
windres "${0%/*}/Resources.rc" "${0%/*}/Resources.o"
g++ -c "${0%/*}/STIGMA.cpp" "${0%/*}/shared/apifilesystem/filesystem.cpp" "${0%/*}/shared/apiprocess/process.cpp" -std=c++17
g++ "${0%/*}/STIGMA.o" "${0%/*}/filesystem.o" "${0%/*}/process.o" "${0%/*}/Resources.o" -o "${0%/*}/STIGMA.exe" -std=c++17 -static -lshell32 -lole32 -luuid -lntdll -Wl,--subsystem,windows
