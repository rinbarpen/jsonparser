# add source files
set(SOURCES
  ../jsonobject.cc
  ../jsonparser.cc
)

# find the test files
file(GLOB TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

message(STATUS "Found test files: ${TEST_SOURCES}")

add_executable(tests
  ${SOURCES}
  ${TEST_SOURCES}
)

target_link_libraries(tests
  GTest::GTest
  GTest::Main
)

foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)  # get the pure filename
  add_test(
    NAME ${TEST_NAME}
    COMMAND tests --gtest_filter=${TEST_NAME}.*
  )
endforeach(TEST_SOURCE)

message(STATUS "Added tests for each test source file")
