add_executable(${PROJECT_NAME}_test_online_statistics test_online_statistics.cpp )
target_link_libraries(${PROJECT_NAME}_test_online_statistics ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_online_statistics PRIVATE -std=c++17)
add_test(test_online_statistics ${PROJECT_NAME}_test_online_statistics)

add_executable(${PROJECT_NAME}_test_rate_monitoring test_rate_monitoring.cpp )
target_link_libraries(${PROJECT_NAME}_test_rate_monitoring ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_rate_monitoring PRIVATE -std=c++17)
add_test(test_rate_monitoring ${PROJECT_NAME}_test_rate_monitoring)
