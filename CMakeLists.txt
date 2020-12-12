cmake_minimum_required(VERSION 3.0)

project( TCC )
find_package( OpenCV REQUIRED )

add_library(VisionManager VisionManager.cpp)
add_library(PreProcess PreProcess.cpp)
add_library(Global Global.cpp)
add_library(Robo Robo.cpp)
add_library(Ball Ball.cpp)
add_library(Colors Colors.cpp)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( TCC main.cpp )
target_link_libraries( TCC ${OpenCV_LIBS} VisionManager PreProcess Global Robo Ball Colors)