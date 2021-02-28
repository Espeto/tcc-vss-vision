cmake_minimum_required(VERSION 3.0)

project( TCC )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(libs/helpers)
add_subdirectory(libs/libobjects)
add_subdirectory(libs/globals)
add_subdirectory(libs/libvision)

add_executable( TCC main.cpp )

target_link_libraries( TCC PUBLIC ${OpenCV_LIBS} VisionManager PreProcess Robo Ball Colors Global)

target_include_directories(TCC PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/libs/globals/"
                            "${PROJECT_SOURCE_DIR}/libs/libobjects/"
                            "${PROJECT_SOURCE_DIR}/libs/libvision/"
                            "${PROJECT_SOURCE_DIR}/libs/helpers/"
                        )