#!/bin/sh
ADWAITAQT_VERSION=1.4.2

sudo -n apt-get update ||:
sudo -n apt-get install -y cmake make pkg-config libx11-dev xcb libx11-xcb-dev libxkbcommon-dev sassc ||:

test -r adwaita-qt-${ADWAITAQT_VERSION}.tar.gz || \
curl -RLOJ https://github.com/FedoraQt/adwaita-qt/archive/refs/tags/${ADWAITAQT_VERSION}.tar.gz
mkdir -p bin
cd bin
tar xzf ../adwaita-qt-${ADWAITAQT_VERSION}.tar.gz
cd adwaita-qt-${ADWAITAQT_VERSION}
patch -p1 <<EOF
diff --git a/CMakeLists.txt b/CMakeLists.txt
index de9249e..9558d77 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -1,6 +1,6 @@
 project(Adwaita)
 
-cmake_minimum_required(VERSION 3.17)
+cmake_minimum_required(VERSION 3.16)
 
 set(ADWAITAQT_VERSION_MAJOR 1)
 set(ADWAITAQT_VERSION "1.4.2")
@@ -13,7 +13,7 @@ if (USE_QT6)
     set(CMAKE_CXX_STANDARD_REQUIRED ON)
     set(ADWAITAQT_SUFFIX "6")
 else()
-    set(QT_MIN_VERSION "5.15.2")
+    set(QT_MIN_VERSION "5.12.8")
     set(CMAKE_CXX_STANDARD 11)
     set(CMAKE_CXX_STANDARD_REQUIRED ON)
     set(ADWAITAQT_SUFFIX "")
EOF
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DQT_PLUGINS_DIR=/usr/plugins -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j2 VERBOSE=1
cd ..
mkdir dest
make -C build -s install DESTDIR=$PWD/dest
make -C build -s clean
cd ..
set -x
mkdir -p usr/lib
cp adwaita-qt-${ADWAITAQT_VERSION}/dest/usr/lib/*/libadwaitaqt.so.1 usr/lib
cp adwaita-qt-${ADWAITAQT_VERSION}/dest/usr/lib/*/libadwaitaqtpriv.so.1 usr/lib
mkdir -p usr/plugins/styles
cp adwaita-qt-${ADWAITAQT_VERSION}/dest/usr/plugins/styles/adwaita.so usr/plugins/styles
set +x
rm -r adwaita-qt-${ADWAITAQT_VERSION}
cd ..
