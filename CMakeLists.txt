cmake_minimum_required(VERSION  3.31)
project("Darkest Planet") 

# Specify C++ Standard
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
add_executable("DarkestPlanet" ${SOURCES})

# Link GLFW to your project
target_link_libraries(DarkestPlanet PRIVATE glfw)

# For macOS, ensure linking with Cocoa, IOKit, and CoreVideo
if(APPLE)
	target_link_libraries(DarkestPlanet PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# For Linux, ensure linking with X11 and other necessary libraries
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
	target_link_libraries(DarkestPlanet PRIVATE ${X11_LIBRARIES})
endif()
