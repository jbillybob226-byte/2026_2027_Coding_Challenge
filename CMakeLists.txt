cmake_minimum_required(VERSION 3.16)
project(LowPassFilterChallenge VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(low_pass_filter STATIC src/low_pass_filter.c)
target_include_directories(low_pass_filter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

include(CTest)
if(BUILD_TESTING)
  add_executable(lpf_testbench testbench/low_pass_filter_testbench.c)
  target_link_libraries(lpf_testbench PRIVATE low_pass_filter)
  if(NOT MSVC)
    target_link_libraries(lpf_testbench PRIVATE m)
  endif()
  add_test(NAME low_pass_filter_correctness COMMAND lpf_testbench)
  add_custom_target(run_tests
    COMMAND $<TARGET_FILE:lpf_testbench>
    DEPENDS lpf_testbench USES_TERMINAL VERBATIM)
endif()

