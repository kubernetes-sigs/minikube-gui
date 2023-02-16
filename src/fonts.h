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

#ifndef FONTS_H
#define FONTS_H

#include <QWidget>

class Fonts
{
public:
    static void initFonts();
    static void setFontAwesome(QWidget *wid);

private:
    static void loadFontAwesome();
    static void setToolTipStyle();
    static QFont fontAwesome;
};

#endif // FONTS_H
