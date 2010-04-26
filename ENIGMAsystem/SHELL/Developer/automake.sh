# Call this file from the directory in question if you would like a generic
# makefile generated which invokes GCC for each individual file.

echo "build:" > makefile;
echo "	-mkdir .eobjs/" > makefile;
for file in *.cpp ;
  do echo "	g++ -c $file		-o .eobjs/${file%.cpp}.o \$(FLAGS)" ;
done >> makefile
echo "" >> makefile;
echo "clean:" >> makefile;
echo "	rm -f .eobjs/*" >> makefile;
