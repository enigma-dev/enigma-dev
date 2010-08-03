read -p "This will overwrite any changes to the makefiles. Are you sure? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
	cd ./Universal_System
	  ../Developer/automake.sh
	  cd ../
      for dir in Graphics_Systems/*/ ; do
		cd $dir
		if [ -e automake.sh ]
		then
			./automake.sh
		else
		  	../../Developer/automake.sh
		fi; cd ../../
	done
	for dir in Platforms/*/ ; do
		cd $dir
		if [ -e automake.sh ]
		then
			./automake.sh
		else
		  	../../Developer/automake.sh
		fi; cd ../../
	done
	for dir in Audio_Systems/*/ ; do
		cd $dir
		if [ -e automake.sh ]
		then
			./automake.sh
		else
		  	../../Developer/automake.sh
		fi; cd ../../
	done
	echo Done
fi
