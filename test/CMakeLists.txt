get_filename_component(TEST_WITH_DATA_TEST_DIR "data" ABSOLUTE)
configure_file(test_helper.h.in test_helper.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(GTest REQUIRED)

add_subdirectory(containers)
add_subdirectory(math)
add_subdirectory(geodesy)
add_subdirectory(geometry)
add_subdirectory(pointset)
add_subdirectory(transform)
add_subdirectory(monitoring)
add_subdirectory(diagnostics)
add_subdirectory(lexical)


add_executable(${PROJECT_NAME}_test_time test_time.cpp )
target_link_libraries(${PROJECT_NAME}_test_time ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_time PRIVATE -std=c++17)
add_test(test_time ${PROJECT_NAME}_test_time)

add_executable(${PROJECT_NAME}_test_coordinates test_coordinates.cpp )
target_link_libraries(${PROJECT_NAME}_test_coordinates ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_coordinates PRIVATE -std=c++17)
add_test(test_coordinates ${PROJECT_NAME}_test_coordinates)

add_executable(${PROJECT_NAME}_test_lexical_cast test_lexical_cast.cpp )
target_link_libraries(${PROJECT_NAME}_test_lexical_cast ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_lexical_cast PRIVATE -std=c++17)
add_test(test_lexical_cast ${PROJECT_NAME}_test_lexical_cast)
