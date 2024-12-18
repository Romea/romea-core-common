add_executable(${PROJECT_NAME}_test_bounding_box test_bounding_box.cpp )
target_link_libraries(${PROJECT_NAME}_test_bounding_box ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_bounding_box PRIVATE -std=c++17)
add_test(test_bounding_box ${PROJECT_NAME}_test_bounding_box test_containers_bb)

add_executable(${PROJECT_NAME}_test_grid_index_mapping test_grid_index_mapping.cpp )
target_link_libraries(${PROJECT_NAME}_test_grid_index_mapping ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_grid_index_mapping PRIVATE -std=c++17)
add_test(test_grid_index_mapping ${PROJECT_NAME}_test_grid_index_mapping)


add_executable(${PROJECT_NAME}_test_grid test_grid.cpp )
target_link_libraries(${PROJECT_NAME}_test_grid ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_grid PRIVATE -std=c++17)
add_test(test_grid ${PROJECT_NAME}_test_grid)

add_executable(${PROJECT_NAME}_test_ray_tracing test_ray_tracing.cpp )
target_link_libraries(${PROJECT_NAME}_test_ray_tracing ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_ray_tracing PRIVATE -std=c++17)
add_test(test_ray_tracing ${PROJECT_NAME}_test_ray_tracing)

