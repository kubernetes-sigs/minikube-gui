# Copyright 2023 The Kubernetes Authors All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

.PHONY: build-linux
build-linux:
	qmake
	make
	tar -czvf minikube-gui-linux.tar.gz -C ./bin ./minikube-gui

.PHONY: build-macos
build-macos:
	qmake
	make
	(cd ./bin && macdeployqt ./minikube-gui.app -qmldir=. -verbose=1 -dmg)
	mv ./bin/minikube-gui.dmg ./minikube-gui-macos.dmg

.PHONY: build-windows
build-windows:
	scripts\build-windows.cmd

.PHONY: bump-version
bump-version:
	sed -i s/QVersionNumber::fromString\(.*\)/QVersionNumber::fromString\(\"$(VERSION)\"\)/ src/window.cpp
