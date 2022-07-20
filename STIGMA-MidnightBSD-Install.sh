#!/bin/sh
su -l root -c '/bin/sh -c "mport install curl wget git gcc gdb gmake cmake pkgconf flac opus opusfile mpg123 libmodplug libsndfile protobuf protobuf-c glew glm png libGLU libvpx mesa-libs openal-soft libogg libsndfile libvorbis vorbis-tools Box2D dumb sdl2 sdl2_mixer freetype2 libffi libX11 libXrandr libXinerama libXtst openjdk8 jna rapidjson libyaml boost-libs pugixml yaml-cpp grpc zenity kdialog && git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/local/bin/stigma-dev && ln -s /usr/local/include/google /usr/local/bin/stigma-dev/shared/protos/google && ln -s /usr/local/include/google /usr/local/bin/stigma-dev/CommandLine/emake/google && cd /usr/local/bin/stigma-dev && gmake clean && gmake && gmake && chmod +x /usr/local/bin/stigma-dev/install.sh && /usr/local/bin/stigma-dev/install.sh && rm -f /usr/local/bin/stigma-dev/plugins/shared/jna.jar && ln -s /usr/local/share/java/classes/jna.jar /usr/local/bin/stigma-dev/plugins/shared/jna.jar && cp -f /usr/local/bin/stigma-dev/stigma-dev-alt.desktop /usr/local/share/applications/stigma-dev.desktop && chmod +x /usr/local/share/applications/stigma-dev.desktop"' && rm -rf ~/.enigma && mkdir -p ~/.enigma/Preprocessor_Environment_Editable && echo "Installation Complete! You may now run the \"stigma-dev.desktop\" Desktop Entry!" && echo "Make sure you put this in your \"/etc/fstab\" if it isn't in there already:" && echo 'fdesc   /dev/fd     fdescfs     rw  0   0' && echo 'proc    /proc       procfs      rw  0   0'
