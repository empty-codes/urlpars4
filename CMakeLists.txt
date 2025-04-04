cmake_minimum_required(VERSION 3.10)
project(urlpars4)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

find_package(glfw3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/imgui)

set(SOURCES
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/misc/cpp/imgui_stdlib.cpp
)

add_executable(UrlPars4 ${SOURCES})

target_link_libraries(UrlPars4 PRIVATE glfw OpenGL::GL)

