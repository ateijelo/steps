#!/bin/sh

#export QT_DEBUG_PLUGINS=1
export QT_PLUGIN_PATH=/usr/local/steps/usr/lib/x86_64-linux-gnu/qt5/plugins
export LD_LIBRARY_PATH=/usr/local/steps/usr/lib/x86_64-linux-gnu/
exec /usr/local/steps/usr/bin/steps "$@" --librarypath /usr/local/steps/usr/lib/x86_64-linux-gnu/qt5/plugins
