get_filename_component(TEST_WITH_DATA_TEST_DIR "data" ABSOLUTE)
configure_file(test_helper.h.in test_helper.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(GTest REQUIRED)
include(${PROJECT_SOURCE_DIR}/cmake/romea_add_gtest.cmake)

add_subdirectory(containers)
# add_subdirectory(control)
add_subdirectory(math)
add_subdirectory(geodesy)
add_subdirectory(geometry)
add_subdirectory(pointset)
add_subdirectory(transform)
add_subdirectory(monitoring)
add_subdirectory(diagnostics)
add_subdirectory(lexical)


romea_add_gtest(test_time test_time.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_coordinates test_coordinates.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_lexical_cast test_lexical_cast.cpp LIBRARIES ${PROJECT_NAME})
