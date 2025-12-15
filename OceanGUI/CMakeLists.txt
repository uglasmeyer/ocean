cmake_minimum_required(VERSION 3.31)

set(OCEAN_PROJECT OceanGUI)
project(${OCEAN_PROJECT})


FILE(GLOB PROJECT_SRC *.cpp)

# Add an executable target
add_executable(${OCEAN_PROJECT}
	${PROJECT_SRC} 
)

set ( QTMAKEFILE Makefile_${QTVERSION} )
set ( QMAKE /usr/lib/${QTVERSION}/bin/qmake )
set ( QMAKE_TARGET qmakeTarget)
add_custom_target(${QMAKE_TARGET}
	COMMAND  ${QMAKE} -o ${QTMAKEFILE} OceanGUI.pro -spec linux-clang
	# CONFIG+=debug CONFIG+=qml_debug
	COMMAND make -f ${QTMAKEFILE} compiler_uic_make_all
	COMMAND make -f ${QTMAKEFILE} mocables
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
)
add_dependencies( ${OCEAN_PROJECT} ${QMAKE_TARGET})

target_include_directories( ${OCEAN_PROJECT} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/ 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
  ${QTLIB}/QtWidgets
  ${QTLIB}/QtCore
  ${QTLIB}/QtGui
  ${QTLIB} 
 )

# Add the library search path for this target only
target_link_directories(${OCEAN_PROJECT} PRIVATE 
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Link against the shared library (without 'lib' prefix or extension)
target_link_libraries(${OCEAN_PROJECT} PRIVATE 
	Ocean 
	tinyxml2
	Qt6Core
	Qt6Gui
	Qt6Widgets
)

