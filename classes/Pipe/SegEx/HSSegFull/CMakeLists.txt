add_library(fullseg FullSeg.cpp)
target_link_libraries(fullseg PUBLIC PipeInterface Global ImgProcess Helper types)