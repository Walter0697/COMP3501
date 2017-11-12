cmake_minimum_required(VERSION 2.6)

# Name of project
project(FlyingUndersizedControlledKiller)

# Specify project files: header files and source files
set(HDRS
    asteroid.h camera.h CameraNode.h game.h resource.h resource_manager.h scene_graph.h scene_node.h Rocket.h fly.h Human.h Character.h Collidable.h Spider.h DragonFly.h Enemy.h Environment.h
)
 
set(SRCS
    asteroid.cpp camera.cpp CameraNode.cpp game.cpp main.cpp resource.cpp resource_manager.cpp scene_graph.cpp scene_node.cpp Rocket.cpp fly.cpp Human.cpp Spider.cpp DragonFly.cpp Environment.cpp material_vp.glsl material_fp.glsl texture_vp.glsl texture_fp.glsl

)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(FlyingUndersizedControlledKiller ${HDRS} ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(FlyingUndersizedControlledKiller ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(FlyingUndersizedControlledKiller ${GLEW_LIBRARY})
target_link_libraries(FlyingUndersizedControlledKiller ${GLFW_LIBRARY})
target_link_libraries(FlyingUndersizedControlledKiller ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(FlyingUndersizedControlledKiller PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
