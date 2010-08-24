read -p "This will overwrite any changes to the makefiles. Are you sure? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
	cd ./Universal_System
	  ../Developer/automake.sh
	  cd ../
	for bigdir in Graphics_Systems Platforms Audio_Systems Collision_Systems ; do
        	for dir in $bigdir/*/ ; do
			cd $dir
			if [ -e automake.sh ]
			then
				./automake.sh
			else
			  	../../Developer/automake.sh
			fi; cd ../../
		done
	done
	echo Done
fi
