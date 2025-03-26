add_executable(${PROJECT_NAME}_test_lexical test_lexical.cpp )
target_link_libraries(${PROJECT_NAME}_test_lexical ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_lexical PRIVATE -std=c++17)
add_test(test_math_matrix ${PROJECT_NAME}_test_lexical)
