call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
qmake
nmake
powershell scripts\bundle-windows.ps1 minikube-gui-windows minikube-gui.exe
