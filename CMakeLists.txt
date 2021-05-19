cmake_minimum_required(VERSION 3.0)

project( TCC )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(libs/helpers)
add_subdirectory(libs/libobjects)
add_subdirectory(libs/globals)
add_subdirectory(libs/libvision)
add_subdirectory(libs/interfaces)
add_subdirectory(libs/types)


add_subdirectory(classes/Pipe1/PreProcess1)
add_subdirectory(classes/Pipe1/PreProcess2)
add_subdirectory(classes/Pipe1/SegEx)
add_subdirectory(classes/Pipe1/SegEx/Extraction)
add_subdirectory(classes/Pipe1/SegEx/HSSegmentation)

add_executable( TCC main.cpp )

target_link_libraries( 
    TCC PUBLIC ${OpenCV_LIBS} 
    PipeInterface
    PreProcess Robo Ball Colors Global Helper
    preprocesspipe1 preprocess2pipe1 segexpipe1 segmentationpipe1 extractionpipe1
)

target_include_directories(TCC PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/libs/globals/"
                            "${PROJECT_SOURCE_DIR}/libs/libobjects/"
                            "${PROJECT_SOURCE_DIR}/libs/libvision/"
                            "${PROJECT_SOURCE_DIR}/libs/helpers/"
                            "${PROJECT_SOURCE_DIR}/libs/interfaces/"
                        )