cmake_minimum_required(VERSION 3.14)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_C_FLAGS -m32)
#set(CMAKE_CXX_FLAGS -m32)

add_executable(untitled main.cpp)

FIND_PACKAGE(OpenGL REQUIRED)
##FIND_PACKAGE(GLUT REQUIRED)
#
#set(GLUT_INCLUDE_DIRS freeglut/include)
#set(GLUT_LIBRARY_DIRS freeglut/lib)
#set(GLUT_LIBRARIES freeglut)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw-3.3)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
#link_directories(${GLUT_LIBRARY_DIRS} ${OPENGL_LIBRARY_DIRS})
#add_definitions(${GLUT_DEFINITIONS} ${OPENGL_DEFINITIONS})

#target_link_libraries(untitled ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
#target_link_libraries(untitled ${OPENGL_LIBRARIES} glfw)
#target_link_libraries(untitled glfw)
target_link_libraries(untitled ${OPENGL_LIBRARIES} glfw)


