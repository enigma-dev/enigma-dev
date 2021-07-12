#!/bin/sh
cd "${0%/*}"
echo "/*"
echo ""
echo "  Made with script from (needs ImageMagick):"
echo "  https://unix.stackexchange.com/a/219526"
echo "  Usage: ./joshcontroller.sh joshcontroller.png > joshcontroller.h"
echo ""
echo "  #!/bin/sh"
echo '  cd "${0%/*}"'
echo "  echo 'unsigned long joshcontroller[] = {'"
echo "  identify -format '%w, %h,' \$1"
echo "  convert \$1 -color-matrix '0 0 1 0, 0 1 0 0, 1 0 0 0, 0 0 0 1' RGBA:- | hexdump -v -e '1/4 "0x%08x,\\\n"'"
echo "  echo '};'"
echo ""
echo "*/"
echo ""
echo 'unsigned long joshcontroller[] = {'
identify -format '%w, %h,' $1
convert $1 -color-matrix '0 0 1 0, 0 1 0 0, 1 0 0 0, 0 0 0 1' RGBA:- | hexdump -v -e '1/4 "0x%08x,\n"'
echo '};' 