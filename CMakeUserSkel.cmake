# If you want to use prefix paths with cmake, copy and rename this file to CMakeUser.cmake
# Do not add this file to GIT!

# set your preferred Qt Library path
IF (CMAKE_CL_64)
	SET(CMAKE_PREFIX_PATH "D:/Qt/qt-everywhere-opensource-src-5.7.0-x64/qtbase/bin/")
ELSE ()
	SET(CMAKE_PREFIX_PATH "D:/Qt/qt-everywhere-opensource-src-5.7.0-x86/qtbase/bin/")
ENDIF ()
