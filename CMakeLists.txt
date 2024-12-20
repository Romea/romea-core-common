cmake_minimum_required(VERSION 3.5)

project(
  "romea_core_common"
  VERSION 0.1.0
  #  DESCRIPTION
  #  HOMEPAGE_URL
  LANGUAGES CXX)


#find_package( PkgConfig )
#pkg_check_modules( EIGEN3 REQUIRED eigen3 )

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_library(${PROJECT_NAME} SHARED
  src/containers/grid/GridIndexMapping.cpp
  src/containers/grid/RayTracing.cpp
  src/containers/boundingbox/AxisAlignedBoundingBox.cpp
  src/containers/boundingbox/OrientedBoundingBox.cpp
  src/control/PID.cpp
  src/control/PIDFeedForward.cpp
  src/diagnostics/CheckupRate.cpp
  src/diagnostics/CheckupReliability.cpp
  src/diagnostics/Diagnostic.cpp
  src/diagnostics/DiagnosticReport.cpp
  src/diagnostics/DiagnosticStatus.cpp
  src/geometry/Circle.cpp
  src/geometry/Ellipse.cpp
  src/geometry/Pose2D.cpp
  src/geometry/Pose3D.cpp
  src/geometry/PoseAndTwist2D.cpp
  src/geometry/PoseAndTwist3D.cpp
  src/geometry/Position2D.cpp
  src/geometry/Position3D.cpp
  src/geometry/Twist2D.cpp
  src/geometry/Twist3D.cpp
  src/geodesy/EarthEllipsoid.cpp
  src/geodesy/ECEFConverter.cpp
  src/geodesy/ENUConverter.cpp
  src/geodesy/LambertConverter.cpp
  src/geodesy/GeodeticCoordinates.cpp
  src/geodesy/WGS84Coordinates.cpp
  src/monitoring/RateMonitoring.cpp
  src/monitoring/OnlineAverage.cpp
  src/monitoring/OnlineVariance.cpp
  src/regression/leastsquares/LeastSquares.cpp
  src/regression/leastsquares/MEstimator.cpp
  src/regression/leastsquares/NLSE.cpp
  src/regression/ransac/Ransac.cpp
  src/regression/ransac/RansacModel.cpp
  src/regression/ransac/RansacIterations.cpp
  src/regression/ransac/RansacRandomCorrespondences.cpp
  src/pointset/algorithms/Correspondence.cpp
  src/pointset/algorithms/PointSetPreconditioner.cpp
  src/pointset/algorithms/PreconditionedPointSet.cpp
  src/pointset/algorithms/NormalAndCurvatureEstimation.cpp
  src/pointset/KdTree.cpp
  src/pointset/kdtree/KdMapNode.cpp
  src/pointset/KdMap.cpp
  src/signal/Noise.cpp
  src/signal/FirstOrderButterworth.cpp
  src/time/Timer.cpp
  src/transform/estimation/FindRigidTransformationByICP.cpp
  src/transform/estimation/FindRigidTransformationByLeastSquares.cpp
  src/transform/estimation/RansacRigidTransformationModel.cpp
  src/transform/estimation/FindRigidTransformationBySVD.cpp
  src/transform/SmartRotation3D.cpp
  )

target_include_directories( ${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

target_include_directories( ${PROJECT_NAME} SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O3 -std=c++17)

include(GNUInstallDirs)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES package.xml DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

option(BUILD_TESTING "BUILD WITH TESTS" ON)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif(BUILD_TESTING)
