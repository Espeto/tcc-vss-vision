add_library(segexpipe1 SegexPipe1.cpp)
target_link_libraries(segexpipe1 PUBLIC PipeInterface types)

add_library(segex2 Segex2.cpp)
target_link_libraries(segex2 PUBLIC PipeInterface types segmentationpipe1 segmentation2)