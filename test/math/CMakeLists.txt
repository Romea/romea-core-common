add_executable(${PROJECT_NAME}_test_math_algorithm test_math_algorithm.cpp )
target_link_libraries(${PROJECT_NAME}_test_math_algorithm ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_math_algorithm PRIVATE -std=c++17)
add_test(test_math_algorithm ${PROJECT_NAME}_test_math_algorithm)

add_executable(${PROJECT_NAME}_test_math_euler test_math_euler.cpp )
target_link_libraries(${PROJECT_NAME}_test_math_euler ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_math_euler PRIVATE -std=c++17)
add_test(test_math_euler ${PROJECT_NAME}_test_math_euler)

add_executable(${PROJECT_NAME}_test_math_histogram test_math_histogram.cpp )
target_link_libraries(${PROJECT_NAME}_test_math_histogram ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_math_histogram PRIVATE -std=c++17)
add_test(test_math_histogram ${PROJECT_NAME}_test_math_histogram)

add_executable(${PROJECT_NAME}_test_math_interval test_math_interval.cpp )
target_link_libraries(${PROJECT_NAME}_test_math_interval ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_math_interval PRIVATE -std=c++17)
add_test(test_math_interval ${PROJECT_NAME}_test_math_interval)

add_executable(${PROJECT_NAME}_test_math_matrix test_math_matrix.cpp )
target_link_libraries(${PROJECT_NAME}_test_math_matrix ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_math_matrix PRIVATE -std=c++17)
add_test(test_math_matrix ${PROJECT_NAME}_test_math_matrix)

add_executable(${PROJECT_NAME}_test_transformation test_transformation.cpp )
target_link_libraries(${PROJECT_NAME}_test_transformation ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_transformation PRIVATE -std=c++17)
add_test(test_math_matrix ${PROJECT_NAME}_test_transformation)
