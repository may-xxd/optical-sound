cmake_minimum_required(VERSION 3.13)

project(optical-sound C ASM)

set (BUILD_SHARED OFF FORCE)
set (BUILD_STATIC ON FORCE)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(OPTICAL_SOUND_SRC
  src/main.c
  )

set(VIDEO_GENERATOR_SRC
  src/video_generator.c
  )

file(GLOB TRIPOD_SRC deps/tripod/src/tp/*.c deps/tripod/src/tp/*.S)


add_executable(optical-sound ${OPTICAL_SOUND_SRC} ${TRIPOD_SRC})
add_executable(video-generator ${VIDEO_GENERATOR_SRC} ${TRIPOD_SRC})

target_include_directories(optical-sound PRIVATE deps/tripod/src)
target_link_libraries(optical-sound PRIVATE m pulse-simple c)

target_include_directories(video-generator PRIVATE deps/tripod/src)
target_link_libraries(video-generator PRIVATE m pulse-simple c)

set_property(TARGET optical-sound PROPERTY C_STANDARD 99)
set_property(TARGET video-generator PROPERTY C_STANDARD 99)

# Enable all warnings
if (MSVC)
  target_compile_options(optical-sound PRIVATE /W4 /WX)
  target_compile_options(video-generator PRIVATE /W4 /WX)
else()
  target_compile_options(optical-sound PRIVATE -Wall -Wextra -pedantic -Werror -Wno-missing-braces -Wno-unused-function -fno-stack-protector)
  target_link_options(optical-sound PRIVATE -nostdlib)
  target_compile_options(video-generator PRIVATE -Wall -Wextra -pedantic -Werror -Wno-missing-braces -Wno-unused-function -fno-stack-protector)
  target_link_options(video-generator PRIVATE -nostdlib)
endif()

