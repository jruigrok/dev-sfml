cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Set project name and version
project(SFMLCircle VERSION 0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" 
      FORCE
    )
endif()

# Find SFML shared libraries
find_package(SFML 2.5
  COMPONENTS 
    system window graphics network audio REQUIRED
  )

# Compile executable
add_executable(SFMLCircle main.cpp)

#Define base directory for artifacts

cmake_path(SET ArtifactsPath "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/")
set(ARTIFACTS_PATH_STRING "${ArtifactsPath}")
message("Cmake Artifacts path is ${ArtifactsPath}")

# Generate config.h
configure_file(config.h.in config.h)

# Set include directory search paths
target_include_directories(SFMLCircle 
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}"
  )

# Link executable to required SFML libraries
target_link_libraries(SFMLCircle sfml-graphics)

# Install target
install(TARGETS SFMLCircle DESTINATION bin)

