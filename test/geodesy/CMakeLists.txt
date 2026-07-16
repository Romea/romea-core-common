romea_add_gtest(test_ecef_converter test_ecef_converter.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_enu_converter test_enu_converter.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_geodetic_coordinates test_geodetic_coordinates.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_lambert_converter test_lambert_converter.cpp LIBRARIES ${PROJECT_NAME})
