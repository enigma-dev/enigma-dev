#!/bin/sh

get_latest () {
  local release=$(curl -k --silent "https://api.github.com/repos/$1/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')
  echo "$release"
}

grab_latest() {
  # 1 = repo path, 2 = version, 3 = local file
  local fileName=$(basename $3)
  echo -e "\e[32mAttempting to download $fileName to ${PWD}/$3 from https://github.com/$1/releases/download/$2/$fileName \e[0m"
  curl -k -L -o "$3" "https://github.com/$1/releases/download/$2/$fileName"
}

mkdir -p "plugins/shared"

#This is some ancient jna version we depend on 
if [ -f "plugins/shared/jna.jar" ]; then
  echo -e "plugins/shared/jna.jar \e[32mAlready up to date, skipping...\e[0m"
else
  jnaJar='https://enigma-dev.org/bin/jna.jar'
  echo -e "\e[32mAttempting to download jna.jar to ${PWD}/plugins/shared/jna.jar from $jnaJar \e[0m"
  curl -k -L -o "plugins/shared/jna.jar" "$jnaJar"
fi

download_latest() {
  latest=$(get_latest "$1")
  grab_latest "$1" "$latest" "$2"
}

download_latest "enigma-dev/lgmplugin" "plugins/enigma.jar"
download_latest "IsmAvatar/LateralGM" "lateralgm.jar"

if [ $(uname) = "Darwin" ]; then
  chmod -R 777 . ;
elif [ $(uname) = "Linux" ]; then
  chmod -R 777 . ;
elif [ $(uname) = "FreeBSD" ]; then
  chmod -R 777 . ;
elif [ $(uname) = "DragonFly" ]; then
  chmod -R 777 . ;
elif [ $(uname) = "NetBSD" ]; then
  chmod -R 777 . ;
elif [ $(uname) = "OpenBSD" ]; then
  chmod -R 777 . ;
elif [ $(uname) = "SunOS" ]; then
  chmod -R 777 . ;
fi
