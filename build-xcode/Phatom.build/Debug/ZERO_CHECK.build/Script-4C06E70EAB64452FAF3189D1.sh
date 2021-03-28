#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/wyr/Desktop/Phatom/build-xcode
  make -f /Users/wyr/Desktop/Phatom/build-xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/wyr/Desktop/Phatom/build-xcode
  make -f /Users/wyr/Desktop/Phatom/build-xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/wyr/Desktop/Phatom/build-xcode
  make -f /Users/wyr/Desktop/Phatom/build-xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/wyr/Desktop/Phatom/build-xcode
  make -f /Users/wyr/Desktop/Phatom/build-xcode/CMakeScripts/ReRunCMake.make
fi

