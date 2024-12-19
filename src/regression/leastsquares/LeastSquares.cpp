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


// Eigen
#include <Eigen/LU>
#include <Eigen/SVD>

// std
#include <limits>

// romea
#include "romea_core_common/regression/leastsquares/LeastSquares.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<typename RealType>
LeastSquares<RealType>::LeastSquares()
: dataSize_(0),
  estimateSize_(),
  Ac_(),
  Bc_(),
  J_(),
  Y_(),
  W_(),
  JtJ_(),
  inverseJtJ_(),
  JtY_()
{
}

//-----------------------------------------------------------------------------
template<typename RealType>
LeastSquares<RealType>::LeastSquares(const size_t & estimateSize)
: dataSize_(0),
  estimateSize_(static_cast<int>(estimateSize)),
  Ac_(Matrix::Identity(estimateSize_, estimateSize_)),
  Bc_(Vector::Zero(estimateSize_)),
  J_(),
  Y_(),
  W_(),
  JtJ_(Matrix::Zero(estimateSize_, estimateSize_)),
  inverseJtJ_(Matrix::Zero(estimateSize_, estimateSize_)),
  JtY_(Vector::Zero(estimateSize_))
{
}

//-----------------------------------------------------------------------------
template<typename RealType>
LeastSquares<RealType>::LeastSquares(const size_t & estimateSize, const size_t & dataSize)
: dataSize_(static_cast<int>(dataSize)),
  estimateSize_(static_cast<int>(estimateSize)),
  Ac_(Matrix::Identity(estimateSize_, estimateSize_)),
  Bc_(Vector::Zero(estimateSize_)),
  J_(Matrix::Zero(dataSize_, estimateSize_)),
  Y_(Vector::Zero(dataSize_)),
  W_(Vector::Ones(dataSize_)),
  JtJ_(Matrix::Zero(estimateSize_, estimateSize_)),
  inverseJtJ_(Matrix::Zero(estimateSize_, estimateSize_)),
  JtY_(Vector::Zero(estimateSize_))
{
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Matrix &
LeastSquares<RealType>::getJ()
{
  return J_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
const typename LeastSquares<RealType>::Matrix &
LeastSquares<RealType>::getJ() const
{
  return J_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Vector &
LeastSquares<RealType>::getY()
{
  return Y_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
const typename LeastSquares<RealType>::Vector &
LeastSquares<RealType>::getY() const
{
  return Y_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Vector &
LeastSquares<RealType>::getW()
{
  return W_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
const typename LeastSquares<RealType>::Vector &
LeastSquares<RealType>::getW() const
{
  return W_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
void
LeastSquares<RealType>::setEstimateSize(const size_t & estimateSize)
{
  estimateSize_ = static_cast<int>(estimateSize);
  Ac_ = Matrix::Identity(estimateSize_, estimateSize_);
  Bc_ = Vector::Zero(estimateSize_);
  JtJ_ = Matrix::Zero(estimateSize_, estimateSize_);
  inverseJtJ_ = Matrix::Zero(estimateSize_, estimateSize_);
  JtY_ = Vector::Zero(estimateSize_);
}

//-----------------------------------------------------------------------------
template<typename RealType>
bool
LeastSquares<RealType>::setDataSize(const size_t & dataSize)
{
  assert(estimateSize_ != 0);
  dataSize_ = static_cast<int>(dataSize);

  if (Y_.rows() < static_cast<int>(dataSize)) {
    J_.resize(static_cast<int>(dataSize), estimateSize_);
    Y_.resize(static_cast<int>(dataSize));
    W_.resize(static_cast<int>(dataSize));
    W_.setConstant(1.0);
    return true;
  }

  return false;
}

//-----------------------------------------------------------------------------
template<typename RealType>
void LeastSquares<RealType>::setPreconditionner(
  const typename LeastSquares<RealType>::Matrix & Ac,
  const typename LeastSquares<RealType>::Vector & Bc)
{
  Ac_ = Ac;
  Bc_ = Bc;
}

//-----------------------------------------------------------------------------
template<typename RealType>
void LeastSquares<RealType>::setPreconditionner(const Matrix & Ac)
{
  setPreconditionner(Ac, Vector::Zero(estimateSize_));
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Vector LeastSquares<RealType>::estimateUsingCholeskyDecomposition()
{
  computeJTJ_();
  computeJTY_();
  inverseJtJ_ = JtJ_.ldlt().solve(Matrix::Identity(estimateSize_, estimateSize_));
  return Ac_ * inverseJtJ_ * JtY_ + Bc_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Vector LeastSquares<RealType>::estimateUsingSVD()
{
  computeJTJ_();
  computeJTY_();

  Eigen::JacobiSVD<Matrix> svd(JtJ_, Eigen::ComputeThinU | Eigen::ComputeThinV);
  inverseJtJ_ = svd.singularValues().asDiagonal();
  for (int n = 0; n < estimateSize_; n++) {
    if (inverseJtJ_(n, n) > std::numeric_limits<RealType>::epsilon()) {
      inverseJtJ_(n, n) = 1 / inverseJtJ_(n, n);
    }
  }

  inverseJtJ_ = svd.matrixV() * inverseJtJ_ * svd.matrixU().transpose();

  return Ac_ * inverseJtJ_ * JtY_ + Bc_;
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Vector LeastSquares<RealType>::weightedEstimate()
{
  weightJAndY_();
  return estimateUsingCholeskyDecomposition();
}

//-----------------------------------------------------------------------------
template<typename RealType>
typename LeastSquares<RealType>::Matrix LeastSquares<RealType>::computeEstimateCovariance(
  const RealType & dataVariance)
{
  return Ac_.transpose() * inverseJtJ_ * Ac_ * dataVariance;
}

// TODO(Jean) paralléliser les trois fonctions ci dessous
//-----------------------------------------------------------------------------
template<typename RealType>
void LeastSquares<RealType>::weightJAndY_()
{
  Y_.head(dataSize_).array() *= W_.head(dataSize_).array();

  for (int i = 0; i < estimateSize_; ++i) {
    J_.col(i).head(dataSize_).array() *= W_.head(dataSize_).array();
  }
}

//-----------------------------------------------------------------------------
template<typename RealType>
void LeastSquares<RealType>::computeJTJ_()
{
  for (int i = 0; i < estimateSize_; ++i) {
    for (int j = i; j < estimateSize_; ++j) {
      JtJ_(i, j) = JtJ_(j, i) = J_.col(i).head(dataSize_).
        dot(J_.col(j).head(dataSize_));
    }
  }
}

//-----------------------------------------------------------------------------
template<typename RealType>
void LeastSquares<RealType>::computeJTY_()
{
  for (int i = 0; i < estimateSize_; ++i) {
    JtY_(i) = J_.col(i).head(dataSize_).dot(Y_.head(dataSize_));
  }
}

//-----------------------------------------------------------------------------
template class LeastSquares<float>;
template class LeastSquares<double>;

}  // namespace core
}  // namespace romea
