CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(APP_NAME point_light_per_pixel)

PROJECT(${APP_NAME})

IF(WIN32)
	ADD_DEFINITIONS(-D_WIN32)
    SET(SOURCE_FILES 
        src/main.cpp
        src/example.cpp
        src/glwindow.cpp
		src/targa.cpp
		src/terrain.cpp
		src/glee/GLee.c
    )
ELSE(WIN32)    
    SET(SOURCE_FILES 
        src/main.cpp
        src/example.cpp
        src/glxwindow.cpp
		src/targa.cpp
		src/terrain.cpp
		src/glee/GLee.c
    )
ENDIF(WIN32)

LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/lib )
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include )

IF(WIN32)
	ADD_EXECUTABLE(${APP_NAME} WIN32 ${SOURCE_FILES})
ELSE(WIN32)
	ADD_EXECUTABLE(${APP_NAME} ${SOURCE_FILES})
ENDIF(WIN32)

IF(WIN32)
	SET(LIBRARIES OPENGL32 GLU32)
ELSE(WIN32)
	SET(LIBRARIES GL GLU Xxf86vm)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${APP_NAME} ${LIBRARIES})
