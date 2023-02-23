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

[CmdletBinding()]
param (
    [string] $archiveName, [string] $targetName
)

$scriptDir = $PSScriptRoot
$currentDir = Get-Location
Write-Host "currentDir" $currentDir
Write-Host "scriptDir" $scriptDir

function Main() {
    New-Item -ItemType Directory $archiveName
    Copy-Item bin\$targetName $archiveName\
    windeployqt --qmldir . --plugindir $archiveName\plugins --no-translations --compiler-runtime $archiveName\$targetName
    $excludeList = @("*.qmlc", "*.ilk", "*.exp", "*.lib", "*.pdb")
    Remove-Item -Path $archiveName -Include $excludeList -Recurse -Force
    $redistDll="{0}{1}\*.CRT\*.dll" -f $env:VCToolsRedistDir.Trim(),$env:msvcArch
    Copy-Item $redistDll $archiveName\
    $sdkDll="{0}Redist\{1}ucrt\DLLs\{2}\*.dll" -f $env:WindowsSdkDir.Trim(),$env:WindowsSdkVersion.Trim(),$env:msvcArch
    Copy-Item $sdkDll $archiveName\
    Compress-Archive -Path minikube-gui-windows minikube-gui-windows.zip
}
Main


