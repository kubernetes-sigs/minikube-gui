#!/bin/sh
set -x
mkdir -p ./bin/usr/bin
cp bin/minikube-gui ./bin/usr/bin
mkdir -p ./bin/usr/share/applications
cp resources/minikube-gui.desktop ./bin/usr/share/applications
mkdir -p ./bin/usr/share/icons/hicolor/512x512/apps
cp resources/images/minikube.png ./bin/usr/share/icons/hicolor/512x512/apps
