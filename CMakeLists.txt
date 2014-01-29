CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(CollisionAlgorithms)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -O3")

FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

INCLUDE_DIRECTORIES(
  ${OGRE_INCLUDE_DIR}
  .
  )

LINK_DIRECTORIES(
  ${OGRE_LIBRARY_DIR}
  )

SET(CMAKE_AUTOMOC ON)

SET(LIBS OgreMain boost_system)

QT5_WRAP_UI(ui_mainwindow.h mainwindow.ui)

SET(HEADERS opttritri.hpp collisiondetector.hpp ogrewidget.hpp scene.hpp object.hpp mainwindow.hpp ui_mainwindow.h)
SET(SOURCES opttritri.cpp collisiondetector.cpp ogrewidget.cpp scene.cpp object.cpp mainwindow.cpp main.cpp)

ADD_EXECUTABLE(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
        )

QT5_USE_MODULES(${PROJECT_NAME} Widgets)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS} )
