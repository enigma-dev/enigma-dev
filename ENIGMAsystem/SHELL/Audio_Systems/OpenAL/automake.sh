# Makefile generator instantiated from /ENIGMAsystem/SHELL/Developer/.

echo "#Awesome Locally Generated Makefile" > Makefile
echo "" >> Makefile;

for file in *.cpp ;
  do
  {
    printf ".eobjs_\$(MODE)/${file%.cpp}.o: $file" >> Makefile;
    for i in `c_incl $file | gawk '/\/usr\/include/ { next } { print } '`;
    do
      printf " $i" >> Makefile;
    done;
    echo "" >> Makefile;
    
    echo "	g++ -c $file		-o .eobjs_\$(MODE)/${file%.cpp}.o \$(FLAGS)"  >> Makefile;
  };
  done;

echo "" >> Makefile;

#create the eobjs folder
echo "mkObjDir:" >> Makefile;
echo "	-mkdir .eobjs_\$(MODE)" >> Makefile;
echo "" >> Makefile;

#generate targets for each ENIGMA mode.
for modename in Run Debug Build Release;
do
  printf "$modename: mkObjDir " >> Makefile;
  for file in *.cpp ;
    do printf ".eobjs_$modename/${file%.cpp}.o " >> Makefile; 
    done;
  printf "\n" >> Makefile;
  echo "	cd alure  && \$(MAKE) static DEST=\"../.eobjs_$modename\" ECFLAGS=\"\$(ECFLAGS) ECPPFLAGS=\"\$(ECPPFLAGS)\"" >> Makefile;
  echo "	cd dumb   && \$(MAKE) static DEST=\"../.eobjs_$modename\" ECFLAGS=\"\$(ECFLAGS) ECPPFLAGS=\"\$(ECPPFLAGS)\"" >> Makefile;
  echo "	cd ogg    && \$(MAKE) static DEST=\"../.eobjs_$modename\" ECFLAGS=\"\$(ECFLAGS) ECPPFLAGS=\"\$(ECPPFLAGS)\"" >> Makefile;
  echo "	cd vorbis && \$(MAKE) static DEST=\"../.eobjs_$modename\" ECFLAGS=\"\$(ECFLAGS) ECPPFLAGS=\"\$(ECPPFLAGS)\"" >> Makefile;
  echo "" >> Makefile;
done;

echo "" >> Makefile;
echo "clean:" >> Makefile;
echo "	-rm .eobjs*/*" >> Makefile;
echo "	cd alure	&& \$(MAKE) clean" >> Makefile;
echo "	cd dumb		&& \$(MAKE) clean" >> Makefile;
echo "	cd ogg		&& \$(MAKE) clean" >> Makefile;
echo "	cd vorbis	&& \$(MAKE) clean" >> Makefile;

cd ./alure/
./automake.sh
cd ../ogg
./automake.sh
cd ../vorbis
./automake.sh
cd ../dumb
./automake.sh

