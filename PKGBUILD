pkgname="stigma-dev"
pkgver="1.0.0.0"
pkgrel="1"
pkgdesc="STIGMA Development Environment. A fork of the ENIGMA Development Environment that strives to go a different direction than that of the software it stemmed from, in hopes to provide a much more stable and appealing game dev software for desktop platorms."
arch=("any")
license=("GPL")
depends=('alure' 'boost' 'box2d' 'cmake' 'curl' 'dumb' 'flac' 'fluidsynth' 'freetype2' 'gcc' 'gdb' 'git' 'glew' 'glfw' 'glib2' 'glm' 'glu' 'grpc' 'gtk3' 'java-runtime' 'kdialog' 'libepoxy' 'libffi' 'libmodplug' 'libogg' 'libpng' 'libvorbis' 'libvpx' 'libx11' 'libxinerama' 'libxrandr' 'libxtst' 'make' 'mesa' 'mpg123' 'openal' 'opus' 'opusfile' 'pkg-config' 'protobuf' 'protobuf-c' 'pugixml' 'pulseaudio' 'rapidjson' 'sdl2' 'sdl2_mixer' 'vorbis-tools' 'yaml-cpp' 'zenity' 'zlib')
url="https://github.com/time-killer-games/stigma-dev"
packager="Samuel Venable"

pre_install() {
	sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/bin/stigma-dev
	[ -d "/usr/bin/stigma-dev" ] && cd /usr/bin/stigma-dev && sudo make clean && sudo make && sudo make emake && sudo chmod +x /usr/bin/stigma-dev/install.sh && sudo /usr/bin/stigma-dev/install.sh && sudo cp -f /usr/bin/stigma-dev/stigma-dev.desktop /usr/share/applications/stigma-dev.desktop && sudo chmod +x /usr/share/applications/stigma-dev.desktop
	sudo mkdir -p /usr/share/doc/stigma-dev
	sudo cp -f /usr/bin/stigma-dev/LICENSE /usr/share/doc/stigma-dev/license
	echo "Installation Complete! You may now run the \"stigma-dev.desktop\" Desktop Entry!"
}

pre_upgrade() {
	pre_install
}

pre_remove() {
	[ -d "/usr/bin/stigma-dev" ] && sudo rm -fr /usr/bin/stigma-dev && sudo rm -f /usr/share/applications/stigma-dev.desktop && rm -fr /usr/share/doc/stigma-dev
}
