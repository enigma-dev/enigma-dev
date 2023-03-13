::Clear Any existing msys
RD /S /Q "C:/msys64/"

::Download and Install 7zip
curl http://mirrors.enigma-dev.org/7z1900-x64.msi -o "%TMP%\7z.msi"
msiexec.exe /i "%TMP%\7z.msi" /QN /L*V "%TMP%\idgaf.log"

::Download and Extract NSIS
curl http://mirrors.enigma-dev.org/nsis-binary-7208-3.zip  -o "%TMP%\nsis.zip"
"%ProgramFiles%\7-zip/7z" -y -o"%TMP%" x "%TMP%\nsis.zip"

::Download and Extract MSYS
curl http://mirrors.enigma-dev.org/distrib/msys2-x86_64-latest.tar.xz -o "%TMP%\msys.xz"
"%ProgramFiles%\7-zip/7z" -y -o"%TMP%" x "%TMP%\msys.xz"
"%ProgramFiles%\7-zip/7z" -y -o"C:\" x "%TMP%\msys"

::Replace Pacman Config
curl http://mirrors.enigma-dev.org/pacman.conf -o "C:\msys64\etc\pacman.conf"

::Intiazlize MSYS
cmd /c C:\msys64\msys2_shell.cmd -defterm -no-start -c "ps -ef | grep '[?]' | awk '{print $2}' | xargs -r kill"

::Install the things
cmd /c C:\msys64\msys2_shell.cmd -defterm -no-start -c "pacman -Sy --noconfirm --needed git make rsync mingw-w64-x86_64-toolchain mingw-w64-x86_64-boost mingw-w64-x86_64-pugixml mingw-w64-x86_64-rapidjson mingw-w64-x86_64-yaml-cpp mingw-w64-x86_64-grpc mingw-w64-x86_64-protobuf mingw-w64-x86_64-glm mingw-w64-x86_64-libpng mingw-w64-x86_64-re2 mingw-w64-x86_64-box2d mingw-w64-x86_64-libffi mingw-w64-x86_64-glew mingw-w64-x86_64-openal mingw-w64-x86_64-dumb mingw-w64-x86_64-alure mingw-w64-x86_64-libmodplug mingw-w64-x86_64-libvorbis mingw-w64-x86_64-libogg mingw-w64-x86_64-flac mingw-w64-x86_64-mpg123 mingw-w64-x86_64-libsndfile mingw-w64-x86_64-libgme mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-libepoxy"

::Clear Pacman Cache
cmd /c C:\msys64\msys2_shell.cmd -defterm -no-start -c "pacman -Scc --noconfirm"

::Clone ENIGMA
::cmd /c C:\msys64\msys2_shell.cmd -defterm -no-start -c "git clone https://github.com/enigma-dev/enigma-dev.git C:/enigma-dev"

::Download and install LGM
curl http://mirrors.enigma-dev.org/LGM-1.0.msi -o "%TMP%\LGM.msi"
msiexec.exe /i "%TMP%\LGM.msi" INSTALLDIR="C:\enigma-dev" /QN /L*V "%TMP%/idgaf.log"

::Build emake
cmd /c C:\msys64\msys2_shell.cmd -defterm -mingw64 -no-start -c "cd C:/enigma-dev && make emake"

::Build Installer
::"%TMP%\nsis-binary-7208-3\Bin\makensis" /V4 "C:/enigma-dev/enigma.nsi"

::Open LGM
::cmd /c C:\msys64\msys2_shell.cmd -defterm -mingw64 -no-start -c "cd C:/enigma-dev && ./LGM.exe"
