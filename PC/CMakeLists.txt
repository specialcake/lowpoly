cmake_minimum_required(VERSION 3.12)
project(Tmper)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(opengl_header ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLStuff/include)
set(glfw_lib ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLStuff/lib/libglfw3.a)
set(assimp_lib ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLStuff/lib/libassimp.so.4)
set(freetype_lib ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLStuff/lib/libfreetype.a)

include_directories(${opengl_header})
link_libraries(${glfw_lib})
link_libraries(${assimp_lib})
link_libraries(${freetype_lib})
find_package(ZLIB)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()

file(GLOB source_file ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLStuff/src/glad.c)

add_executable(${CMAKE_PROJECT_NAME} ${source_file})

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_DL_LIBS} ${ZLIB_LIBRARIES} X11 Xrandr Xinerama Xi Xxf86vm Xcursor pthread png freetype harfbuzz bz2)
