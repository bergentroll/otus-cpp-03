cmake_minimum_required(VERSION 3.12.4)

if($ENV{TRAVIS_BUILD_NUMBER})
  set(VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
  set(VERSION 0.0.0)
endif()
project(allocator VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 17)

set(GTEST_SRCDIR "/usr/src/gtest/")

set(SRC_PATH "src/")
set(TEST_PATH "test/")
include_directories("inc/")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_compile_definitions(DEBUG)
endif()

find_package(benchmark)

add_executable(allocator ${SRC_PATH}/main.cpp)

if(benchmark_FOUND)
  add_executable(profiling ${TEST_PATH}/profiling.cpp)
  target_link_libraries(profiling benchmark)
else()
  message(
    WARNING
    "Profiling will not be launched because benchmark library is not found.")
endif()

if(EXISTS "${GTEST_SRCDIR}/CMakeLists.txt")
  add_subdirectory(${GTEST_SRCDIR}/ ${CMAKE_BINARY_DIR}/gtest/ EXCLUDE_FROM_ALL)

  add_executable(test_suite ${TEST_PATH}/test.cpp)
  target_link_libraries(
    test_suite
    PRIVATE
    gtest)
  add_test(test_suite test_suite)
else()
  message(
    WARNING
    "Test suite will not be launched because GTest sources is not found at "
    "${GTEST_SRCDIR}.")
endif()

enable_testing()

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT bergentroll@insiberia.net)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>=2.15)")
include(CPack)
