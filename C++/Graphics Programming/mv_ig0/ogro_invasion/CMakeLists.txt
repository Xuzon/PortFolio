CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(APP_NAME ogro_invasion)

PROJECT(${APP_NAME})

IF(WIN32)
    ADD_DEFINITIONS(-D_WIN32)
    SET(SOURCE_FILES 
        src/main.cpp
        src/example.cpp
        src/glwindow.cpp
		src/targa.cpp
		src/glee/GLee.c
		src/camera.cpp
		src/collider.cpp
		src/enemy.cpp
		src/entity.cpp
		src/explosion.cpp
		src/freetypefont.cpp
		src/frustum.cpp
		src/gameworld.cpp
		src/landscape.cpp
		src/md2model.cpp
		src/ogro.cpp
		src/player.cpp
		src/rocket.cpp
		src/spherecollider.cpp
		src/terrain.cpp
		src/terraincollider.cpp
		src/tree.cpp
    )
ELSE(WIN32)
    SET(SOURCE_FILES 
        src/main.cpp
        src/example.cpp
        src/glxwindow.cpp
		src/targa.cpp
		src/glee/GLee.c
		src/camera.cpp
		src/collider.cpp
		src/enemy.cpp
		src/entity.cpp
		src/explosion.cpp
		src/freetypefont.cpp
		src/frustum.cpp
		src/gameworld.cpp
		src/landscape.cpp
		src/md2model.cpp
		src/ogro.cpp
		src/player.cpp
		src/rocket.cpp
		src/spherecollider.cpp
		src/terrain.cpp
		src/terraincollider.cpp
		src/tree.cpp
    )
ENDIF(WIN32)

IF(WIN32)
	INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/freetype/include ${CMAKE_SOURCE_DIR}/src/freetype/include/freetype2)
	LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src/freetype/lib )	
	ADD_EXECUTABLE(${APP_NAME} WIN32 ${SOURCE_FILES})
ELSE(WIN32)
	INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include /usr/include/freetype2)
	ADD_EXECUTABLE(${APP_NAME} ${SOURCE_FILES})
ENDIF(WIN32)

IF(WIN32)
	SET(LIBRARIES OPENGL32 GLU32 freetype)
ELSE(WIN32)
	SET(LIBRARIES GL GLU Xxf86vm freetype)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${APP_NAME} ${LIBRARIES})
