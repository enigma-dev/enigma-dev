#!/bin/sh
cd "${0%/*}"
if [ $(uname) = "Darwin" ]; then
  clang++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -arch arm64 -arch x86_64; ./internal;
elif [ $(uname) = "Linux" ]; then
  if [ -f "/bin/g++" ]; then
    g++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./internal;
  else
    clang++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall; ./internal;
  fi
elif [ $(uname) = "FreeBSD" ]; then
  clang++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -static; ./internal;
elif [ $(uname) = "DragonFly" ]; then
  g++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./internal;
elif [ $(uname) = "NetBSD" ]; then
  g++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./internal;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -lkvm -static; ./internal;
elif [ $(uname) = "SunOS" ]; then
  g++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -static-libgcc; ./internal;
elif [ $(uname) = "Haiku" ]; then
  g++ __getexecname/internal.cpp internal.cpp -o internal -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./internal;
else
  g++ __getexecname/internal.cpp internal.cpp -o internal.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static -Wl,--subsystem,console; ./internal.exe;
fi
