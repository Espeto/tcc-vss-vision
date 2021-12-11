cmake_minimum_required(VERSION 3.0)

project( TCC )

find_package( OpenCV REQUIRED )
find_package( OpenMP REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(libs/helpers)
add_subdirectory(libs/libobjects)
add_subdirectory(libs/globals)
add_subdirectory(libs/libvision)
add_subdirectory(libs/interfaces)
add_subdirectory(libs/types)

add_subdirectory(classes/Pipe/PreProcess)

add_subdirectory(classes/Pipe/SegEx)
add_subdirectory(classes/Pipe/SegEx/Extraction)
add_subdirectory(classes/Pipe/SegEx/HSSegReduced)
add_subdirectory(classes/Pipe/SegEx/HSSegFull)


add_executable( TCC main.cpp )

target_link_libraries( 
    TCC PUBLIC ${OpenCV_LIBS} OpenMP::OpenMP_CXX
    PipeInterface
    ImgProcess Robo Ball Colors Global Helper
    preprocess segex fullseg reducedseg extraction
)

target_include_directories(TCC PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/libs/globals/"
                            "${PROJECT_SOURCE_DIR}/libs/libobjects/"
                            "${PROJECT_SOURCE_DIR}/libs/libvision/"
                            "${PROJECT_SOURCE_DIR}/libs/helpers/"
                            "${PROJECT_SOURCE_DIR}/libs/interfaces/"
                        )