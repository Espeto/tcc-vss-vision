add_library(VisionManager VisionManager.cpp)
add_library(PreProcess PreProcess.cpp)
add_library(Colors Colors.cpp)
target_link_libraries(VisionManager PRIVATE Global Helper Robo Ball PreProcess)