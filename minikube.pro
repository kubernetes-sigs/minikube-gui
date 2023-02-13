HEADERS       = src/window.h \
                src/advancedview.h \
                src/basicview.h \
                src/cluster.h \
                src/commandrunner.h \
                src/constants.h \
                src/errormessage.h \
                src/fonts.h \
                src/hyperkit.h \
                src/logger.h \
                src/mount.h \
                src/operator.h \
                src/paths.h \
                src/progresswindow.h \
                src/serviceView.h \
                src/tray.h \
                src/updater.h
SOURCES       = src/main.cpp \
                src/advancedview.cpp \
                src/basicview.cpp \
                src/cluster.cpp \
                src/commandrunner.cpp \
                src/constants.cpp \
                src/errormessage.cpp \
                src/fonts.cpp \
                src/hyperkit.cpp \
                src/logger.cpp \
                src/mount.cpp \
                src/operator.cpp \
                src/paths.cpp \
                src/progresswindow.cpp \
                src/serviceView.cpp \
                src/tray.cpp \
                src/updater.cpp \
                src/window.cpp
RESOURCES     = minikube.qrc
ICON          = resources/images/minikube.icns

QT += widgets network
requires(qtConfig(combobox))

DISTFILES += \
    LICENSE

# Enabling qtermwidget requires GPL-v2 license
#CONFIG += gpl_licensed

gpl_licensed {
  win32: DEFINES += QT_NO_TERMWIDGET

  unix: CONFIG += link_pkgconfig
  unix: PKGCONFIG += qtermwidget5
} else {
  DEFINES += QT_NO_TERMWIDGET
}
