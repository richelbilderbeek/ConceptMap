win32 {
  # Windows only
  message("Console application, built for Windows")
  QMAKE_CXXFLAGS += -std=c++1y -Wall -Wextra -Weffc++
}

macx {
  # Mac only
  message("Console application, built for Mac")
  QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+
  CONFIG +=c++1y
}

unix:!macx{
  # Linux only
  message("Console application, built for Linux")

  CONFIG += c++14
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14
}

# Go ahead and use Qt.Core: it is about as platform-independent as
# the STL and Boost
QT += core

# Go ahead and use Qt.Gui: it is about as platform-independent as
# the STL and Boost. It is needed for QImage
QT += gui

# Don't define widgets: it would defy the purpose of this console
# application to work non-GUI
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

#
#
# Type of compile
#
#

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}


#INCLUDEPATH += C:/Boost/include
#DEPENDPATH += C:/Boost/include

#INCLUDEPATH += C:/Boost
#LIBS += C:/Boost/stage/lib/libboost_filesystem-mgw49-mt-1_59.a
#LIBS += C:/Boost/stage/lib/libboost_system-mgw49-mt-d-1_59.a
#LIBS += C:/Boost/stage/lib/libboost_graph-mgw49-mt-d-1_59.a

#include(../RibiLibraries/ConsoleApplicationNoWeffcpp.pri)
#include(../RibiLibraries/GeneralConsole.pri)
#include(../RibiLibraries/Apfloat.pri)
#include(../RibiLibraries/BoostAll.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

#include(../RibiClasses/CppAbcFile/CppAbcFile.pri)


include(../RibiClasses/CppContainer/CppContainer.pri)
#include(../RibiClasses/CppCounter/CppCounter.pri)
include(ConceptMap.pri)
include(ConceptMapTest.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
#include(../RibiClasses/CppGeometry/CppGeometry.pri)
include(../RibiClasses/CppFileIo/CppFileIo.pri)

#include(../RibiClasses/CppMusic/CppMusic.pri)

#include(../RibiClasses/CppPlane/CppPlane.pri)
#include(../RibiClasses/CppPlane/CppPlaneTest.pri)
#include(../RibiClasses/CppRibiRandom/CppRibiRandom.pri)
include(../RibiClasses/CppRibiRegex/CppRibiRegex.pri)
#include(../RibiClasses/CppRibiSystem/CppRibiSystem.pri)
#include(../RibiClasses/CppRibiTime/CppRibiTime.pri)
include(../RibiClasses/CppTrace/CppTrace.pri)
include(../RibiClasses/CppXml/CppXml.pri)
#include(../RibiClasses/CppXml/CppXmlTest.pri)

SOURCES += main_test.cpp

# Boost.Test
LIBS += -lboost_unit_test_framework

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# Boost.Graph
LIBS += \
  -lboost_date_time \
  -lboost_graph \
  -lboost_regex

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable
