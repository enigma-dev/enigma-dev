read -p "This will overwrite any changes to the makefiles. Are you sure? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
	cd ./Universal_System
	  ../Developer/automake.sh
	  cd ../
	cd Graphics_Systems/OpenGL
	  ../../Developer/automake.sh
	  cd ../../
	cd Platforms/windows
	  ../../Developer/automake.sh
	  cd ../../
	cd Platforms/xlib
	  ../../Developer/automake.sh
	  cd ../../
	echo Done
fi
