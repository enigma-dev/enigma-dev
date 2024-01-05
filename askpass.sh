#!/bin/sh
pw="$(osascript -e 'Tell application "System Events" to display dialog "Password:" default answer "" with hidden answer' -e 'text returned of result' 2>/dev/null)" && echo "$pw"
