cmake_minimum_required(VERSION 3.15)
project(imageviewer)

set(CMAKE_CXX_STANDARD 14)

# X11
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})

# Magic++
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
link_directories(${ImageMagick_LIBRARIES})

add_executable(imageviewer main.cpp)
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
target_link_libraries(imageviewer ${X11_LIBRARIES})
target_link_libraries(imageviewer ${ImageMagick_LIBRARIES})