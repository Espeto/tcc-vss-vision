add_library(PipeInterface INTERFACE)
target_include_directories(PipeInterface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/PipeInterface>)