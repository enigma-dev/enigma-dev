#!/bin/sh
su -l root -c '/bin/sh -c "rm -fr /usr/local/bin/stigma-dev && rm -f /usr/local/share/applications/stigma-dev.desktop"' && echo 'Uninstallation Complete! Run "pkg autoremove" to remove unwanted dependencies.'
