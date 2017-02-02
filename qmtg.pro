QT += qml quick

CONFIG += c++1z
nn
SOURCES += main.cpp \
    ecst/extlibs/vrm_core/include/vrm/core/assert/impl/assert.cpp \
    ecst/extlibs/vrm_core/include/vrm/core/ostream_utils/nullptr_printer.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Includepaths for dependencies
INCLUDEPATH += ecst/include
INCLUDEPATH += ecst/extlibs/vrm_core/include
INCLUDEPATH += ecst/extlibs/vrm_pp/include

BOOST_ROOT = "C:/msys64/mingw64/include"
# We also need boost
_BOOST_ROOT = $$BOOST_ROOT
isEmpty(_BOOST_ROOT) {
    message(\"Boost Library\" qmake value not detected...)

    # Try to use the system environment value.
    _BOOST_ROOT = $$(BOOST_ROOT)
}

isEmpty(_BOOST_ROOT) {
    message(\"Boost Library\" environment variable not detected...)
    !build_pass:error(Please set the environment variable `BOOST_ROOT`. For example, BOOST_ROOT=c:\\boost_1_53_0)
} else {
    message(\"Boost Library\" detected in BOOST_ROOT = \"$$_BOOST_ROOT\")
    INCLUDEPATH += $$_BOOST_ROOT
}


DISTFILES += \
    qtquickcontrols2.conf \
    main.qml \
    Hand.qml \
    Avatar.qml \
    Board.qml

HEADERS += \
    mtg/mtg.hpp
