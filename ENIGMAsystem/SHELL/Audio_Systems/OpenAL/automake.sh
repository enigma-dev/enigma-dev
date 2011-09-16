# Makefile generator instantiated from /ENIGMAsystem/SHELL/Developer/.

echo "#Awesome Locally Generated Makefile" > Makefile
echo "" >> Makefile;

for file in *.cpp ;
  do
  {
    printf "\$(OBJECTS)/${file%.cpp}.o: $file" >> Makefile;
    for i in `c_incl $file | gawk '/\/usr\/include/ { next } { print } '`;
    do
      printf " $i" >> Makefile;
    done;
    echo "" >> Makefile;
    
    echo "	g++ -c $file		-o \$(OBJECTS)/${file%.cpp}.o \$(FLAGS)"  >> Makefile;
  };
  done;

echo "" >> Makefile;

#create the eobjs folder
echo "\$(OBJECTS):" >> Makefile;
echo "	-mkdir -p \$(OBJECTS) || mkdir \"\$(OBJECTS)\"" >> Makefile;
echo "" >> Makefile;

#generate targets for each ENIGMA mode.

printf "this: \$(OBJECTS) " >> Makefile;
for file in *.cpp ;
  do printf "\$(OBJECTS)/${file%.cpp}.o " >> Makefile; 
  done;
printf "\n" >> Makefile;
echo "	cd alure  && \$(MAKE) static DEST=\"../\$(OBJECTS)\" ECFLAGS=\"\$(ECFLAGS)\" ECPPFLAGS=\"\$(ECPPFLAGS)\"" >> Makefile;
echo "	cd ../Codecs &&\$(MAKE) " >> Makefile;
echo "" >> Makefile;

echo "" >> Makefile;
echo "clean:" >> Makefile;
echo "	-rm .eobjs/*/*/*/*" >> Makefile;
echo "	cd alure && \$(MAKE) clean" >> Makefile;

cd ./alure/ && ./automake.sh


