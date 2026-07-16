romea_add_gtest(test_geometry_pose2d test_geometry_pose2d.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_geometry_pose3d test_geometry_pose3d.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_geometry_position test_geometry_position.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_geometry_pose_and_twist test_geometry_pose_and_twist.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_geometry_shapes test_geometry_shapes.cpp LIBRARIES ${PROJECT_NAME})

romea_add_gtest(test_geometry_twist test_geometry_twist.cpp LIBRARIES ${PROJECT_NAME})
