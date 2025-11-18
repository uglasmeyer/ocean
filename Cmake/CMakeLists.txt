cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_SOURCE_DIR $ENV{HOME}/git/Ocean/)
set(Qt6 /usr/include/x86_64-linux-gnu/qt6)

set(CMAKE_BINARY_DIR $ENV{HOME}/OceanDev)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/x86_64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/x86_64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x86_64)

project(Ocean)

# Add sub-directories
add_subdirectory(../src 		build_src )
add_subdirectory(../Audioserver build_Audioserver )
add_subdirectory(../Composer 	build_Composer )
add_subdirectory(../OceanGUI 	build_OceanGUI )
add_subdirectory(../Rtsp 		build_Rtsp )
add_subdirectory(../SDSview 	build_SDSview )
add_subdirectory(../Synth 		build_Synth )


