cmake_minimum_required(VERSION 2.8)
project( apriltag_mods )
find_package( OpenCV REQUIRED )

add_executable( pnm2mat pnm2mat.cpp )
target_link_libraries( pnm2mat ${OpenCV_LIBS} )
target_link_libraries( pnm2mat ${CMAKE_SOURCE_DIR}/libapriltag.a )

add_executable( rgb2lab rgb2lab.cpp )
target_link_libraries( rgb2lab ${OpenCV_LIBS} )
