# This script is to be run from the trunk
# or other toplevel directory.

# Get the revision number
rev=`svn info | grep -i -o -m 1 "rev\(ision\)\?: [0-9]\+"`
rev=`echo "$rev" | grep -o "[0-9]\+"`

# Get the platform
case "$1" in
	"Win"|"win"|"Windows"|"windows")
		platform="win"
		;;
esac

if [[ ! -n $platform ]]
then
	echo "Valid platform not given."
	echo "Usage: makeRelease.sh [PLATFORM]"
	exit 1
fi

echo "Prepare install-ready zip for revision $rev"

# Create the directory
odir=ENIGMA-R4-r$rev-$platform
if mkdir -p $odir;
then
	echo "Made directory for output"
else
	echo "Failed to create directory!"
	exit 1
fi

# Copy shit into it
case "$platform" in
	"win")
		cp ./CompilerSource/stupidity-buffer/ENIGMA.exe $odir/
		cp ./l*.jar $odir/
		cp -r ./plugins $odir/
		cp -r ./Autoconf $odir/
		mv $odir/Autoconf/svnkit.jar $odir/plugins/shared/
		mkdir -p $odir/Compilers/Windows
		;;
esac

# Enter our directory for sprucing and trimming
cd $odir

# Remove SVN files
svns=`find ./ -regex ".*/\.svn" -exec rm -rf {} \;`

# Zip the directory
cd ../
rm -f $odir.zip
zip -r $odir.zip $odir

rm -rf $odir
