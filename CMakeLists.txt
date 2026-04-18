cmake_minimum_required(VERSION 3.20)
project(VecFields)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define root paths
set(VECFIELDS_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(VECFIELDS_INCLUDE_DIR "${VECFIELDS_ROOT}/include")
set(VECFIELDS_EXTERNAL_DIR "${VECFIELDS_ROOT}/external")

# Global includes
include_directories(${VECFIELDS_INCLUDE_DIR})
include_directories(${VECFIELDS_EXTERNAL_DIR}/eigen)

# Testing
enable_testing()
add_subdirectory(external/googletest)
add_subdirectory(tests)

# Optional: include apps and tutorials
# add_subdirectory(app)
# add_subdirectory(tutorial)
