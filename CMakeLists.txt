cmake_minimum_required(VERSION 3.16.3)
project(OpGLProj VERSION 1.0.0)

add_definitions(-D _GLFW_WIN32)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/ReactPhysics3D)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)

file(GLOB IMGUI_HEADERS "${CMAKE_SOURCE_DIR}/external/imgui/include/*")
file(GLOB IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/external/imgui/src/*")

set (CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/includes") # self resources
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")

set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")

set(ALL_INCLUDES WindowPainter SmartThing Sky Shader Camera3D Boxy)

set(i 0)
foreach(n IN LISTS REACTPHYSICS3D_HEADERS)
    LIST_REPLACE(REACTPHYSICS3D_HEADERS ${i} ${CMAKE_SOURCE_DIR}/external/ReactPhysics3D/${n})
    MATH(EXPR i "${i}+1")
endforeach()

set(i 0)
foreach(n IN LISTS REACTPHYSICS3D_SOURCES)
    LIST_REPLACE(REACTPHYSICS3D_SOURCES ${i} ${CMAKE_SOURCE_DIR}/external/ReactPhysics3D/${n})
    MATH(EXPR i "${i}+1")
endforeach()

foreach(n IN LISTS ALL_INCLUDES)
    add_library(${n} ${INCLUDE_DIR}/${n}.h ${RESOURCE_DIR}/${n}.cpp)
endforeach()

add_library(glad 
            ${GLAD_DIR}/include/glad/glad.h 
            ${GLAD_DIR}/src/glad.c
)

add_library(imgui
            ${IMGUI_HEADERS}
            ${IMGUI_SOURCES}
)

add_library(ReactPhysics3D 
            ${REACTPHYSICS3D_HEADERS}
            ${REACTPHYSICS3D_SOURCES}
)

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include/glad)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include/GLFW)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/ReactPhysics3D/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/include)

add_executable(OpGL Source.cpp)

target_link_libraries(OpGL PUBLIC ReactPhysics3D)

target_link_libraries(OpGL PUBLIC opengl32.lib)
target_link_libraries(OpGL PUBLIC glfw)
target_link_libraries(OpGL PUBLIC glad)
target_link_libraries(OpGL PUBLIC imgui)

foreach(n IN LISTS ALL_INCLUDES)
    target_link_libraries(OpGL PUBLIC ${n})
    target_compile_definitions(${n} PUBLIC BASE_DIR="${CMAKE_SOURCE_DIR}")
endforeach()