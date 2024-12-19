// Copyright 2022 INRAE, French National Research Institute for Agriculture, Food and Environment
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// gtest
#include <gtest/gtest.h>

// std
#include <vector>
#include <string>

// local
#include "../test/test_helper.h"
#include "../pointset/test_pointset_utils.hpp"
#include "romea_core_common/transform/estimation/FindRigidTransformationByLeastSquares.hpp"
#include "romea_core_common/transform/estimation/FindRigidTransformationBySVD.hpp"
#include "romea_core_common/transform/estimation/FindRigidTransformationByICP.hpp"

const Eigen::Affine2d transformation2d =
  Eigen::Translation2d(0.1, 0.2) *
  Eigen::Rotation2Dd(0.05);

const Eigen::Affine3d transformation3d =
  Eigen::Translation3d(0.1, 0.2, -0.3) *
  Eigen::AngleAxisd(0.03, Eigen::Vector3d::UnitX()) *
  Eigen::AngleAxisd(-0.02, Eigen::Vector3d::UnitZ()) *
  Eigen::AngleAxisd(0.01, Eigen::Vector3d::UnitZ());


//-----------------------------------------------------------------------------
template<template<class PoinType> class EstimatorType, class PointType>
void testWithoutPreconditionning(
  const std::string & scanFileName,
  const Eigen::Transform<typename PointType::Scalar,
  romea::core::PointTraits<PointType>::DIM, Eigen::Affine> & transformation)
{
  romea::core::PointSet<PointType> sourcePoints = loadScan<PointType>(scanFileName);
  romea::core::PointSet<PointType> targetPoints = projectScan(sourcePoints, transformation);
  romea::core::NormalSet<PointType> targetNormals = computeNormals(targetPoints);
  std::vector<romea::core::Correspondence> correspondences =
    fakeCorrespondences(sourcePoints.size());

  EstimatorType<PointType> estimator;
  auto estimatedTransformation = estimator.find(
    sourcePoints,
    targetPoints,
    targetNormals,
    correspondences);

  EXPECT_NEAR(
    romea::core::PointTraits<PointType>::DIM + 1,
    (transformation.matrix().inverse() * estimatedTransformation).array().sum(), 0.01);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindByLeastSquaresWithoutPreconditionningc)
{
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector2d>(
    "/scan2d.txt", transformation2d);
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindByLeastSquaresWithoutPreconditionningh)
{
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates2d>(
    "/scan2d.txt", transformation2d);
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindBySVDWithoutPreconditionningc)
{
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector2d>(
    "/scan2d.txt", transformation2d);
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindBySVDWithoutPreconditionningh)
{
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates2d>(
    "/scan2d.txt", transformation2d);
  testWithoutPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates3d>(
    "/scan3d.txt", transformation3d);
}


//-----------------------------------------------------------------------------
template<template<class PoinType> class EstimatorType, class PointType>
void testWithPreconditionning(
  const std::string & scanFileName,
  const Eigen::Transform<typename PointType::Scalar,
  romea::core::PointTraits<PointType>::DIM, Eigen::Affine> & transformation)
{
  romea::core::PointSet<PointType> sourcePoints = loadScan<PointType>(scanFileName);
  romea::core::PointSet<PointType> targetPoints = projectScan(sourcePoints, transformation);
  romea::core::NormalSet<PointType> targetNormals = computeNormals(targetPoints);
  std::vector<romea::core::Correspondence> correspondences =
    fakeCorrespondences(sourcePoints.size());

  EstimatorType<PointType> estimator;
  romea::core::PointSetPreconditioner<PointType> targetPointsPreconditioner(targetPoints);
  romea::core::PreconditionedPointSet<PointType> preconditionedSourcePoints(
    sourcePoints, targetPointsPreconditioner.getScale());
  romea::core::PreconditionedPointSet<PointType> preconditionedTargetPoints(
    targetPoints, targetPointsPreconditioner.getScale());

  estimator.setPreconditioner(
    preconditionedSourcePoints,
    preconditionedTargetPoints);

  auto estimatedTransformation = estimator.find(
    preconditionedSourcePoints,
    preconditionedTargetPoints,
    targetNormals,
    correspondences);

  EXPECT_NEAR(
    romea::core::PointTraits<PointType>::DIM + 1,
    (transformation.matrix().inverse() * estimatedTransformation).array().sum(), 0.01);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindByLeastSquaresWithPreconditionningc)
{
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector2d>(
    "/scan2d.txt", transformation2d);
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindByLeastSquaresWithPreconditionningh)
{
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates2d>(
    "/scan2d.txt", transformation2d);
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindBySVDWithPreconditionningc)
{
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector2d>(
    "/scan2d.txt", transformation2d);
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares, Eigen::Vector3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindBySVDWithPreconditionningh)
{
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates2d>(
    "/scan2d.txt", transformation2d);
  testWithPreconditionning<romea::core::FindRigidTransformationByLeastSquares,
    romea::core::HomogeneousCoordinates3d>(
    "/scan3d.txt", transformation3d);
}

//-----------------------------------------------------------------------------
template<class PointType>
void testICP(
  const std::string & scanFileName,
  const Eigen::Transform<typename PointType::Scalar,
  romea::core::PointTraits<PointType>::DIM, Eigen::Affine> & transformation)
{
  typedef Eigen::Matrix<typename PointType::Scalar,
      romea::core::PointTraits<PointType>::DIM + 1,
      romea::core::PointTraits<PointType>::DIM + 1> GuessMatrix;

  romea::core::FindRigidTransformationByICP<PointType> icp(0.2);
  romea::core::PointSet<PointType> sourcePoints = loadScan<PointType>(scanFileName);
  romea::core::PointSet<PointType> targetPoints = projectScan(sourcePoints, transformation);

  auto method =
    romea::core::FindRigidTransformationByICP<PointType>::EstimationMethod::LEAST_SQUARES;
  bool found = icp.find(sourcePoints, targetPoints, GuessMatrix::Identity(), method);
  ASSERT_EQ(true, found);

  EXPECT_NEAR(
    romea::core::PointTraits<PointType>::DIM + 1,
    (transformation.matrix().inverse() * icp.getTransformation()).array().sum(), 0.01);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindByICPc)
{
  testICP<Eigen::Vector2d>("/scan2d.txt", transformation2d);
  //  testICP<Eigen::Vector3d>("/scan3d.txt",transformation3d);
}

//-----------------------------------------------------------------------------
TEST(TestTransform, FindByICPh)
{
  testICP<romea::core::HomogeneousCoordinates2d>("/scan2d.txt", transformation2d);
  //  testICP<romea::HomogeneousCoordinates3d>("/scan3d.txt",transformation3d);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
