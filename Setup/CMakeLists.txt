cmake_minimum_required(VERSION 3.31)

set(OCEAN_PROJECT Setup)
project(${OCEAN_PROJECT})

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_SOURCE_DIR $ENV{HOME}/git/Ocean/)
set(CMAKE_BINARY_DIR $ENV{HOME}/OceanDev)



set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/x86_64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/x86_64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x86_64)


include_directories(${PROJECT_SOURCE_DIR}/include)

# Add an executable target
add_executable(${OCEAN_PROJECT} ${OCEAN_PROJECT}.cpp)

# Add the library search path for this target only
target_link_directories(${OCEAN_PROJECT} PRIVATE ${PROJECT_SOURCE_DIR}/Shlib)
# Link against the shared library (without 'lib' prefix or extension)
target_link_libraries(${OCEAN_PROJECT} PRIVATE Ocean)

#target_link_libraries(Setup PRIVATE -L/home/sirius/git/Ocean/Shlib -lOcean)

