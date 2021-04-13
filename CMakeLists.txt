cmake_minimum_required(VERSION 3.16.3)
project(OpGLProj VERSION 1.0.0)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/ReactPhysics3D)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/imgui)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)

set (CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/includes") # self resources
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")

set(GLAD_DIR "${CMAKE_SOURCE_DIR}/external/glad")

set(ALL_INCLUDES SmartThing Sky WindowPainter Shader Camera3D Boxy)

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
            ${GLAD_DIR}/src/glad.c
)

add_library(imgui
            ${IMGUI_SOURCES}
            ${IMGUI_HEADERS}
)

add_library(ReactPhysics3D 
            ${REACTPHYSICS3D_HEADERS}
            ${REACTPHYSICS3D_SOURCES}
)

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include/glad)
include_directories(${CMAKE_SOURCE_DIR}/external/glm/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/ReactPhysics3D/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/include)

set(srcs    ${CMAKE_SOURCE_DIR}/external/imgui/src/imgui_impl_glfw.cpp 
            ${CMAKE_SOURCE_DIR}/external/imgui/include/imgui_impl_glfw.h 
            ${CMAKE_SOURCE_DIR}/external/imgui/src/imgui_impl_opengl3.cpp 
            ${CMAKE_SOURCE_DIR}/external/imgui/include/imgui_impl_opengl3.h 
            Source.cpp)

add_executable(OpGL ${srcs})

target_link_libraries(OpGL ReactPhysics3D)

find_package(OpenGL REQUIRED)
target_link_libraries(OpGL OpenGL::GL)

target_link_libraries(OpGL imgui)
target_link_libraries(OpGL glad)
target_link_libraries(OpGL glfw ${GLFW_LIBRARIES})

foreach(n IN LISTS ALL_INCLUDES)
    target_link_libraries(OpGL ${n})
    target_compile_definitions(${n} PUBLIC BASE_DIR="${CMAKE_SOURCE_DIR}")
endforeach()

