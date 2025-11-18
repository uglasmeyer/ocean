cmake_minimum_required(VERSION 3.31)

set(OCEAN_PROJECT libOcean)
project(${OCEAN_PROJECT})

FILE(GLOB OCEANLIB_SRC *.cpp)

# Add a shared library target
add_library(${OCEAN_PROJECT} SHARED
${OCEANLIB_SRC}
)

target_include_directories( ${OCEAN_PROJECT} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/ )
target_sources( ${OCEAN_PROJECT} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR} )
  
# Link against the shared library (without 'lib' prefix or extension)
target_link_libraries(${OCEAN_PROJECT} PRIVATE -ltinyxml2)

