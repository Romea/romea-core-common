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


#ifndef ROMEA_CORE_COMMON__POINTSET__ALGORITHMS__NORMALANDCURVATUREESTIMATION_HPP_
#define ROMEA_CORE_COMMON__POINTSET__ALGORITHMS__NORMALANDCURVATUREESTIMATION_HPP_

// Eigen
#include <Eigen/Eigenvalues>

// std
#include <vector>

// romea
#include "romea_core_common/pointset/PointSet.hpp"
#include "romea_core_common/pointset/NormalSet.hpp"
#include "romea_core_common/pointset/KdTree.hpp"


namespace romea
{
namespace core
{

template<class PointType>
class NormalAndCurvatureEstimation
{
public:
  using Scalar = typename PointType::Scalar;
  static constexpr size_t CARTESIAN_DIM = PointTraits<PointType>::DIM;
  static constexpr size_t POINT_SIZE = PointTraits<PointType>::SIZE;

  using KdTreeType = KdTree<PointType>;
  using VectorType = std::vector<Scalar>;
  using PointSetType = PointSet<PointType>;
  using NormalSetType = NormalSet<PointType>;

  using EigenValuesType = Eigen::Matrix<Scalar, CARTESIAN_DIM, 1>;
  using EigenVectorsType = Eigen::Matrix<Scalar, CARTESIAN_DIM, CARTESIAN_DIM>;

  enum class EstimationMethod
  {
    PCA = 0,
    ROBUST_PCA,  // not yed implemented
    MCMD  // not yed implemented
  };

public:
  explicit NormalAndCurvatureEstimation(const size_t & numberOfNeighborPoints);

  void compute(
    const PointSetType & points,
    NormalSetType & normals);

  void compute(
    const PointSetType & points,
    const KdTreeType & pointsKdTree,
    NormalSetType & normals);

  void compute(
    const PointSetType & points,
    NormalSetType & normals,
    VectorType & curvatures);

  void compute(
    const PointSetType & points,
    const KdTreeType & pointsKdTree,
    NormalSetType & normals,
    VectorType & curvatures);

  void compute(
    const PointSetType & points,
    NormalSetType & normals,
    VectorType & curvatures,
    VectorType & normalsReliability);

  void compute(
    const PointSetType & points,
    const KdTreeType & pointsKdTree,
    NormalSetType & normals,
    VectorType & curvatures,
    VectorType & normalsReliability);

private:
  void planeEstimation_(
    const PointSetType & points,
    const KdTreeType & pointsKdTree,
    const size_t & pointIndex);

  Scalar computeNormalReliability();

private:
  size_t numberOfNeighborPoints_;
  std::vector<size_t> neighborIndexes_;
  std::vector<Scalar> neighborSquareDistances_;
  Eigen::SelfAdjointEigenSolver<EigenVectorsType> eigenSolver_;
  EigenValuesType eigenValues_;
  EigenVectorsType eigenVectors_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(Scalar, CARTESIAN_DIM)
};

}  // namespace core
}  // namespace romea


#endif  // ROMEA_CORE_COMMON__POINTSET__ALGORITHMS__NORMALANDCURVATUREESTIMATION_HPP_
