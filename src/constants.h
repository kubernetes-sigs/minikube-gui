/*
Copyright 2023 The Kubernetes Authors All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

class Constants
{
public:
    const static int basicViewHeight = 300;
    const static int basicViewWidth = 250;
    inline static const QString startIcon = "\uf04b";
    inline static const QString stopIcon = "\uf04d";
    inline static const QString pauseIcon = "\uf04c";
    inline static const QString unPauseIcon = "\uf04b";
    inline static const QString deleteIcon = "\uf1f8";
    inline static const QString reloadIcon = "\uf021";
    inline static const QString createIcon = "\uf0fe";
};

#endif // CONSTANTS_H
