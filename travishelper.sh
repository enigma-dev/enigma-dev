#!/bin/bash
# - travishelper.sh
# - helps set up travis with the proper gcc/clang

echo CXX: $1

#remove postgresql
sudo apt-get --purge remove postgresql-9.1 postgresql-9.1-postgis-2.1 postgresql-9.1-postgis-scripts postgresql-9.2 postgresql-9.2-postgis-2.1 postgresql-9.2-postgis-scripts postgresql-9.3 postgresql-9.3-postgis-2.1 postgresql-9.3-postgis-scripts postgresql-9.4 postgresql-9.4-postgis-2.1 postgresql-9.4-postgis-scripts postgresql-client postgresql-client-9.1 postgresql-client-9.2 postgresql-client-9.3 postgresql-client-9.4 postgresql-client-common postgresql-common postgresql-contrib-9.2 postgresql-contrib-9.3 postgresql-contrib-9.4

#add rayflood gcc repo
sudo add-apt-repository -y ppa:h-rayflood/gcc-upper
sudo apt-get update -qq
sudo apt-get -o Dpkg::Options::="--force-confnew" dist-upgrade -qq

#install gcc 4.9
if [ "$1" == "g++" ]; then
    sudo apt-get install -qq gcc-4.9 g++-4.9
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 90
fi

#install clang 3.5
if [ "$1" == "clang++" ]; then
    sudo add-apt-repository -y ppa:h-rayflood/llvm-upper
    sudo apt-get update -qq
    sudo apt-get -o Dpkg::Options::="--force-confnew" dist-upgrade -qq
    sudo apt-get install -qq clang-3.5
    sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.5 90
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/clang++-3.5 90
fi
