cmake_minimum_required(VERSION 3.31)

set(OCEAN_PROJECT Synthesizer )
project(${OCEAN_PROJECT})

FILE(GLOB PROJECT_SRC *.cpp)

# Add an executable target
add_executable(${OCEAN_PROJECT}
${PROJECT_SRC} 
)

target_include_directories( ${OCEAN_PROJECT} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/ ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# Add the library search path for this target only
target_link_directories(${OCEAN_PROJECT} PRIVATE 
${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Link against the shared library (without 'lib' prefix or extension)
target_link_libraries(${OCEAN_PROJECT} PRIVATE Ocean tinyxml2)

