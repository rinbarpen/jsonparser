cmake_minimum_required(VERSION 3.12)
project(JsonParser)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the path of header files
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# googletest required
find_package(GTest REQUIRED)

# output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(test)

# add_custom_target(test
#   COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
#   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/gtest*.cc
# )

# install
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# For other programs
find_program(CLANG_FORMAT_EXECUTABLE clang-format)

if(CLANG_FORMAT_EXECUTABLE)
  # clang-format
  # find all code files
  file(GLOB_RECURSE CODE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hh
  )

  # press 'make format' to format your source files
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXECUTABLE}
      -i
      -style="{BasedOnStyle: Google, IndentWidth: 2}"
      ${CODE_FILES}
  )
else()
  message(WARNING "clang-format not found. Please make sure it is installed and accessible.")
endif()
