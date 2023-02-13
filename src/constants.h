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
//    golden ration
    const static int basicViewHeight = 405;
    const static int basicViewWidth = 250;
    static const QString startIcon;
    static const QString stopIcon;
    static const QString pauseIcon;
    static const QString unPauseIcon;
    static const QString deleteIcon;
    static const QString reloadIcon;
    static const QString createIcon;
};

#endif // CONSTANTS_H
