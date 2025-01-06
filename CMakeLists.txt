cmake_minimum_required(VERSION  3.31)
project("Darkest Planet") 

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include FetchContent module
include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest # You can replace 'latest' with a specific version tag or commit hash
)
FetchContent_MakeAvailable(glfw)

# Fetch stb 
FetchContent_Declare(
    stb 
    GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# Create a library target for stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
add_executable("DarkestPlanet" ${SOURCES})

find_package(Vulkan REQUIRED)

# Link GLFW to your project
target_link_libraries(DarkestPlanet PRIVATE glfw stb Vulkan::Vulkan)

# For macOS, ensure linking with Cocoa, IOKit, and CoreVideo
if(APPLE)
	target_link_libraries(DarkestPlanet PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# For Linux, ensure linking with X11 and other necessary libraries
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
	target_link_libraries(DarkestPlanet PRIVATE ${X11_LIBRARIES})
endif()
