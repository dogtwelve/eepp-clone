#!/bin/sh
adb devices
cd $(dirname "$0")
export NDK_PROJECT_PATH=$(dirname "$0")
ndk-build NDK_LOG=1 NDK_DEBUG=1 -j`nproc`
rm -rf ./bin
rm -rf ./gen
ant debug
adb install -r bin/eepp-app-debug.apk
#ndk-gdb --force --start --verbose
