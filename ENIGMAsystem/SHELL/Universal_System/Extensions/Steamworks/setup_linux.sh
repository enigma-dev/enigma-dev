############################################################################
# Copyright (C) 2023-2024 Saif Kandil
#
# This file is a part of the ENIGMA Development Environment.
#
# ENIGMA is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, version 3 of the license or any later version.
#
# This application and its source code is distributed AS-IS, WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along
# with this code. If not, see <http://www.gnu.org/licenses/>
############################################################################

# Append the lines to .bashrc
echo "export LD_LIBRARY_PATH=~/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/game_client/Steamv157/sdk/redistributable_bin/linux64:${LD_LIBRARY_PATH}" >> ~/.bashrc
echo "export LD_PRELOAD=~/.local/share/Steam/ubuntu12_64/gameoverlayrenderer.so:${LD_PRELOAD}" >> ~/.bashrc

# Source the modified .bashrc to apply changes immediately
source ~/.bashrc
