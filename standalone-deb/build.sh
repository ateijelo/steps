#!/bin/sh

PKGS="libgeographic17 libicu57 libjpeg62-turbo libpcre16-3 libpng16-16 libqt5core5a libqt5dbus5 libqt5gui5 libqt5network5 libqt5sql5 libqt5sql5-sqlite libqt5widgets5 libqt5xml5 libstdc++6 libx11-xcb1 libxcb1 libxcb-glx0 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render0 libxcb-render-util0 libxcb-shape0 libxcb-shm0 libxcb-sync1 libxcb-util0 libxcb-xfixes0 libxcb-xinerama0 libxcb-xkb1 libxi6 libxkbcommon0 libxkbcommon-x11-0 libxrender1"

for PKG in $PKGS
do
    apt-get download $PKG
    dpkg -x $PKG usr-local-steps/
done

# remove some stuff we don't use
rm -rf usr-local-steps/usr/share/doc/*

# build the binary package
dpkg-buildpackage -b
