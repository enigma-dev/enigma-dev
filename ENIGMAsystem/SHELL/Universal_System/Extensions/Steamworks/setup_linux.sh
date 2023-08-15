# Append the lines to .bashrc
echo "export LD_LIBRARY_PATH=~/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/game_client/Steamv157/sdk/redistributable_bin/linux64:${LD_LIBRARY_PATH}" >> ~/.bashrc
echo "export LD_PRELOAD=~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so:${LD_PRELOAD}" >> ~/.bashrc

# Source the modified .bashrc to apply changes immediately
source ~/.bashrc
