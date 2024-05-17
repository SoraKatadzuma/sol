cmake_minimum_required(VERSION 3.16)
project(sol VERSION 1.0.0 LANGUAGES CXX)


set(SOL_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(SOL_TOP_LEVEL ON)
endif()


add_library(${PROJECT_NAME} INTERFACE)
add_library(sk::sol ALIAS ${PROJECT_NAME})
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE "${PROJECT_SOURCE_DIR}/include"
)


option(SOL_BUILD_TESTS "" ${SOL_TOP_LEVEL})
if(SOL_BUILD_TESTS)
  enable_testing()

  add_executable(sol_tests tests/sol_test.cpp)
  add_test(NAME sol_tests COMMAND sol_tests)
  target_link_libraries(sol_tests PRIVATE sk::sol)
endif()
