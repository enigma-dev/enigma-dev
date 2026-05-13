#!/bin/sh
cd "${0%/*}"
if [ $(uname) = "Darwin" ]; then
  clang++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -arch arm64 -arch x86_64; ./external;
elif [ $(uname) = "Linux" ]; then
  if [ -f "/bin/g++" ]; then
    g++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./external;
  else
    clang++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall; ./external;
  fi
elif [ $(uname) = "FreeBSD" ]; then
  clang++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -static; ./external;
elif [ $(uname) = "DragonFly" ]; then
  g++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./external;
elif [ $(uname) = "NetBSD" ]; then
  g++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./external;
elif [ $(uname) = "OpenBSD" ]; then
  clang++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -lkvm -static; ./external;
elif [ $(uname) = "SunOS" ]; then
  g++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -static-libgcc -lproc; ./external;
elif [ $(uname) = "Haiku" ]; then
  g++ __getexecname/external.cpp external.cpp -o external -std=c++17 -Wall -static-libgcc -static-libstdc++ -static; ./external;
else
  g++ __getexecname/external.cpp external.cpp -o external.exe -std=c++17 -Wall -static-libgcc -static-libstdc++ -static -Wl,--subsystem,console; ./external.exe;
fi
