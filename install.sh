#!/bin/sh


key0="enigma-dev/lgmplugin"
key1="IsmAvatar/LateralGM"
dep0="plugins/enigma.jar"
dep1="lateralgm.jar"

get_latest () {
  local release=$(curl --silent "https://api.github.com/repos/$1/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')
  echo "$release"
}

grab_latest() {
  # 1 = repo path, 2 = version, 3 = local file
  local fileName=$(basename $3)
  echo -e "\e[32mAttempting to download $fileName to ${PWD}/$3 from https://github.com/$1/releases/download/$2/$fileName \e[0m"
  curl -L -o "$3" "https://github.com/$1/releases/download/$2/$fileName"
}

mkdir -p "plugins/shared"

#This is some ancient jna version we depend on 
if [ -f "plugins/shared/jna.jar" ]; then
  echo -e "plugins/shared/jna.jar \e[32mAlready up to date, skipping...\e[0m"
else
  jnaJar='https://enigma-dev.org/bin/jna.jar'
  echo -e "\e[32mAttempting to download jna.jar to ${PWD}/plugins/shared/jna.jar from $jnaJar \e[0m"
  curl -L -o "plugins/shared/jna.jar" "$jnaJar"
fi

download_latest() {
  latest0=$(get_latest "$key0")
  grab_latest "$key0" "$latest0" "$dep0"
  latest1=$(get_latest "$key1")
  grab_latest "$key1" "$latest1" "$dep1"
}

download_latest
