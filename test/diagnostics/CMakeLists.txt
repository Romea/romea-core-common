romea_add_gtest(test_checkup_equal_to test_checkup_equal_to.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_checkup_greater_than test_checkup_greater_than.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_checkup_lower_than test_checkup_lower_than.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_checkup_greater_than_rate test_checkup_greater_than_rate.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_checkup_equal_to_rate test_checkup_equal_to_rate.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_checkup_reliability test_checkup_reliability.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_diagnostic_status test_diagnostic_status.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_diagnostic_report test_diagnostic_report.cpp LIBRARIES ${PROJECT_NAME})
