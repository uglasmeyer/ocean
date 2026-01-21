cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 23)

set ( PROJECT_SOURCE_DIR 	$ENV{PWD})
set ( CMAKE_BINARY_DIR 	$ENV{HOME}/OceanDev)
set ( QTVERSION qt6 )

execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Host Achitecture (uname -m): ${ARCH}" )
set ( QTLIB /usr/include/${ARCH}-linux-gnu/${QTVERSION})

if(EXISTS "${QTLIB}" AND IS_DIRECTORY "${QTLIB}")
    message(STATUS "${QTLIB} OK")
else()
    message(FATAL_ERROR "Directory does not exist: ${QTLIB} ")
endif()

execute_process(
    COMMAND nproc
    OUTPUT_VARIABLE NPROC
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if( "${NPROC}" STREQUAL "" )
	message( STATUS "no nproc" )
else()
	message(STATUS "using ${NPROC} parallel builds" )
	set(ENV{MAKEFLAGS} "-j${NPROC}" )
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${ARCH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${ARCH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${ARCH})



project(Ocean)

# create external build directories as defined before
add_custom_target(build-time-make-directory ALL
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# uniform compile switches
add_compile_options(-Wall -frecord-gcc-switches -pthread -O2 -g )

# Add sub-directories
add_subdirectory(../src 		build_src )
add_subdirectory(../Audioserver build_Audioserver )
add_subdirectory(../Composer 	build_Composer )
add_subdirectory(../OceanGUI 	build_OceanGUI )
add_subdirectory(../Rtsp 		build_Rtsp )
add_subdirectory(../SDSview 	build_SDSview )
add_subdirectory(../Synth 		build_Synth )
add_subdirectory(../TestPrj 		build_TestPrj )

# create a runtime directory
add_subdirectory(../Setup 		build_Setup )

