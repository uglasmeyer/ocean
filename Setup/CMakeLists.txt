cmake_minimum_required(VERSION 3.31)

set(OCEAN_PROJECT Setup)
project(${OCEAN_PROJECT})

set(WITHOUT_SHM 1)
# Add an executable target
add_executable(${OCEAN_PROJECT} 
	${OCEAN_PROJECT}.cpp
)

target_include_directories( ${OCEAN_PROJECT} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/ )

# Add the library search path for this target only
target_link_directories(${OCEAN_PROJECT} PRIVATE 
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Link against the shared library (without 'lib' prefix or extension)
target_link_libraries(${OCEAN_PROJECT} PRIVATE Ocean)

execute_process(
COMMAND 			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Setup -S ${PROJECT_SOURCE_DIR}/..
WORKING_DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}
)
