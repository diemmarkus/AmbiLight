
# check cmake requirements
cmake_minimum_required(VERSION 3.5)

######################## begin of project
project(AmbiLight)

set(BINARY_NAME ${CMAKE_PROJECT_NAME})

# include macros needed
include("Utils.cmake")

# load paths from the user file if exists
if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeUser.cmake)
	include(${CMAKE_SOURCE_DIR}/CMakeUser.cmake)
endif()

# find Qt
NMC_FINDQT()

file(GLOB NOMACS_EXE_SOURCES "src/*.cpp")
file(GLOB NOMACS_EXE_HEADERS "src/*.h")

# gather information for building
include_directories (
	${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

# create the targets
set(BINARY_NAME ${PROJECT_NAME})
add_executable(${BINARY_NAME} WIN32 ${NOMACS_EXE_SOURCES} ${NOMACS_EXE_HEADERS})
target_link_libraries(${BINARY_NAME})

qt5_use_modules(${BINARY_NAME} Widgets Gui)

set(QTLIBLIST Qt5Core Qt5Gui Qt5Widgets)
foreach(qtlib ${QTLIBLIST})
	get_filename_component(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)
	file(COPY ${QT_DLL_PATH_tmp}/${qtlib}.dll DESTINATION ${CMAKE_BINARY_DIR}/Release)
	file(COPY ${QT_DLL_PATH_tmp}/${qtlib}.dll DESTINATION ${CMAKE_BINARY_DIR}/RelWithDebInfo)
	file(COPY ${QT_DLL_PATH_tmp}/${qtlib}d.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug)
endforeach(qtlib)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release/platforms)
file(COPY ${QT_DLL_PATH_tmp}/../plugins/platforms/qwindows.dll DESTINATION ${CMAKE_BINARY_DIR}/Release/platforms/)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release/printsupport)
file(COPY ${QT_DLL_PATH_tmp}/../plugins/printsupport/windowsprintersupport.dll DESTINATION ${CMAKE_BINARY_DIR}/Release/printsupport)

# set properties for Visual Studio Projects
add_definitions(/Zc:wchar_t-)
set(CMAKE_CXX_FLAGS_DEBUG "/W4 /EHsc ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "/W4 /O2 /EHsc -DDK_INSTALL -DQT_NO_DEBUG_OUTPUT ${CMAKE_CXX_FLAGS_RELEASE}")

file(GLOB NOMACS_AUTOMOC "${CMAKE_BINARY_DIR}/*_automoc.cpp")
source_group("Generated Files" FILES ${NOMACS_RCC} ${NOMACS_UI} ${NOMACS_RC} ${NOMACS_QM} ${NOMACS_AUTOMOC})
