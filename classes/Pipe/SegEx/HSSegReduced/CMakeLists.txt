add_library(reducedseg ReducedSeg.cpp)
target_link_libraries(reducedseg PUBLIC PipeInterface Global ImgProcess Helper types)