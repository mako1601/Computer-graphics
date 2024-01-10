cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME ComputerGraphics)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/ShaderProgram.h
	src/ShaderProgram.cpp
	src/Window.h
	src/Window.cpp
	src/Camera.h
	src/Camera.cpp
	src/TextRenderer.h
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR}/.. ABSOLUTE)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(${PARENT_DIR}/external/glfw ${CMAKE_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(${PARENT_DIR}/external/glad ${CMAKE_BINARY_DIR}/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(${PARENT_DIR}/external/glm ${CMAKE_BINARY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} glm)

add_subdirectory(${PARENT_DIR}/external/freetype ${CMAKE_BINARY_DIR}/freetype)
target_link_libraries(${PROJECT_NAME} freetype)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_draw.cpp
)

add_library(imgui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(imgui PUBLIC ../external)
target_include_directories(imgui PUBLIC ../external/imgui)
target_link_libraries(imgui PRIVATE glad glfw)

target_link_libraries(${PROJECT_NAME} imgui)