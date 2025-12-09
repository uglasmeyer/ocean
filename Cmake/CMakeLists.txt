cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 23)

set(PROJECT_SOURCE_DIR 	$ENV{PWD})
set(CMAKE_BINARY_DIR 	$ENV{HOME}/OceanDev)

execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Host Achitecture (uname -m): ${ARCH}" )
set(Qt6 /usr/include/${ARCH}-linux-gnu/qt6)

if(EXISTS "${Qt6}" AND IS_DIRECTORY "${Qt6}")
    message(STATUS "${Qt6} OK")
else()
    message(FATAL_ERROR "Directory does not exist: ${Qt6} ")
endif()
    
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${ARCH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${ARCH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${ARCH})


project(Ocean)

add_custom_target(build-time-make-directory ALL
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_compile_options(-Wall -frecord-gcc-switches -pthread -O2 -g )


# Add sub-directories
add_subdirectory(../src 		build_src )
add_subdirectory(../Audioserver build_Audioserver )
add_subdirectory(../Composer 	build_Composer )
add_subdirectory(../OceanGUI 	build_OceanGUI )
add_subdirectory(../Rtsp 		build_Rtsp )
add_subdirectory(../SDSview 	build_SDSview )
add_subdirectory(../Synth 		build_Synth )

# create a runtime directory
add_subdirectory(../Setup 		build_Setup )

