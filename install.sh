#!/bin/bash

declare -A deps
deps["lateralgm.jar"]="IsmAvatar/LateralGM"
deps["plugins/enigma.jar"]="enigma-dev/lgmplugin"

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

if [ ! -f ".deps" ]; then
  touch ".deps"
fi
  
lineCount=$(wc -l < ".deps")
lineNum=1;

for key in ${!deps[@]}; 
do
  
  latest=$(get_latest ${deps[$key]})

  if [ "$lineCount" -lt "$lineNum" ]; then
    echo "$key $latest" >> ".deps"
    grab_latest "${deps[$key]}" "$latest" "$key"
  else
    line=$(sed -n ${lineNum}p < ".deps")
     if [ "$line" != "$latest" ]; then
       grab_latest "${deps[$key]}" "$latest" "$key"
       sed -i "${lineNum}s/.*/${latest}/" ".deps"
     else
       echo -e "$key \e[32mAlready up to date, skipping...\e[0m"
     fi
  fi
  
  lineNum=$(($lineNum+1))
  
done
