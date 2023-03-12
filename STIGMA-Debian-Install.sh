#!/bin/sh
sudo apt-get update && sudo apt-get install wget curl git gcc g++ make cmake libc-dev libgtk-3-dev libglib2.0-dev libprotobuf-dev protobuf-compiler protobuf-compiler-grpc libgrpc-dev libgrpc++-dev zlib1g-dev libvpx-dev libglew-dev libglm-dev libpng-dev libglu1-mesa-dev libopenal-dev libfluidsynth-dev libogg-dev libalure-dev libvorbisfile3 libvorbis-dev libbox2d-dev libdumb1-dev libsdl2-dev libsdl2-mixer-dev libfreetype6-dev libffi-dev libx11-dev libxrandr-dev libxinerama-dev libxtst-dev libepoxy-dev default-jre pkg-config rapidjson-dev libyaml-cpp-dev libboost-dev libboost-filesystem-dev libboost-system-dev libboost-program-options-dev libboost-iostreams-dev libprocps-dev pulseaudio libpugixml-dev zenity kdialog libflac-dev libopus-dev libopusfile-dev libsndfile-dev libmodplug-dev libmpg123-dev
sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/bin/stigma-dev
[ -d "/usr/bin/stigma-dev" ] && cd /usr/bin/stigma-dev && sudo make clean && sudo make && sudo make emake && sudo chmod +x /usr/bin/stigma-dev/install.sh && sudo /usr/bin/stigma-dev/install.sh && sudo cp -f /usr/bin/stigma-dev/stigma-dev.desktop /usr/share/applications/stigma-dev.desktop && sudo chmod +x /usr/share/applications/stigma-dev.desktop
echo "Installation Complete! You may now run the \"stigma-dev.desktop\" Desktop Entry!"
