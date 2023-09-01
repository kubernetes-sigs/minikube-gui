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
build-linux: ## Build minikube-gui for Linux
	qmake
	make -f Makefile
	scripts/build-linux.sh
	export VERSION=$(subst v,,$(VERSION)) && \
	(cd ./bin && linuxdeployqt usr/share/applications/minikube-gui.desktop -verbose=1 -appimage)
	mv ./bin/*.AppImage ./minikube-gui-linux.AppImage
	tar -czvf minikube-gui-linux.tar.gz -C ./bin .

.PHONY: build-macos
build-macos: ## Build minikube-gui for macOS
	qmake
	make -f Makefile
	(cd ./bin && macdeployqt ./minikube-gui.app -qmldir=. -verbose=1 -dmg)
	mv ./bin/minikube-gui.dmg ./minikube-gui-macos.dmg
	tar -czf minikube-gui-macos.tar.gz ./bin

.PHONY: build-windows
build-windows: ## Build minikube-gui for Windows
	scripts\build-windows.cmd

.PHONY: bump-version
bump-version: ## Bumps the version of minikube-gui
	sed -i s/QVersionNumber::fromString\(.*\)/QVersionNumber::fromString\(\"$(VERSION)\"\)/ src/window.cpp

.PHONY: help
help:
	@printf "\033[1mAvailable targets for minikube-gui\033[21m\n"
	@printf "\033[1m----------------------------------\033[21m\n"
	@grep -h -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: update-releases-json
update-releases-json: ## Updates the releases.json file with the latest release
	(cd scripts && go run update-releases-json.go -version=$(VERSION))
