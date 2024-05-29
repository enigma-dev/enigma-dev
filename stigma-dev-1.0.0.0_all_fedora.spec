Buildroot: /home/asahi/stigma-dev-1.0.0.0
Name: stigma-dev
Version: 1.0.0.0
Release: 1
Summary: STIGMA Development Environment
License: GPL
Distribution: Fedora
Group: games
Requires: wget, curl, git, gcc, g++, make, cmake, protobuf-devel, protobuf-compiler, grpc-cli, grpc-devel, zlib-devel, libvpx-devel, glew-devel, glfw-devel, hwloc-devel, glm-devel, libpng-devel, mesa-libGLU-devel, openal-soft-devel, fluidsynth-devel, libogg-devel, alure-devel, libvorbis-devel, dumb-devel, SDL2-devel, libffi-devel, libX11-devel, libXrandr-devel, libXinerama-devel, java-17-openjdk, pkgconf, rapidjson-devel, yaml-cpp-devel, boost-devel, procps-ng-devel, boost-program-options, boost-iostreams, boost-system, boost-filesystem, grpc-cpp, libstdc++-static, libstdc++-devel, pugixml-devel, zenity, kdialog, libmodplug-devel, libsndfile-devel, SDL2_mixer-devel, freetype-devel, libXtst-devel, flac-devel, mpg123-devel, opusfile-devel, opus-devel, gtk3-devel, glib-devel

%define _rpmdir ../
%define _rpmfilename stigma-dev-1.0.0.0_all_fedora.rpm
%define _unpackaged_files_terminate_build 0

%description
A fork of the ENIGMA Development Environment that strives to go a different direction than that of the software it stemmed from, in hopes to provide a much more stable and appealing game dev software for desktop platorms.

%files
%dir "/"
%dir "/usr/"
%dir "/usr/share/"
%dir "/usr/share/doc/"
%dir "/usr/share/doc/stigma-dev/"
"/usr/share/doc/stigma-dev/copyright"

%pre
sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/bin/stigma-dev
[ -d "/usr/bin/stigma-dev" ] && cd /usr/bin/stigma-dev && sudo make clean && sudo make && sudo make emake && sudo chmod +x /usr/bin/stigma-dev/install.sh && sudo /usr/bin/stigma-dev/install.sh && sudo cp -f /usr/bin/stigma-dev/stigma-dev.desktop /usr/share/applications/stigma-dev.desktop && sudo chmod +x /usr/share/applications/stigma-dev.desktop
echo "Installation Complete! You may now run the \"stigma-dev.desktop\" Desktop Entry!"

%postun
[ -d "/usr/bin/stigma-dev" ] && sudo rm -fr /usr/bin/stigma-dev && sudo rm -f /usr/share/applications/stigma-dev.desktop
