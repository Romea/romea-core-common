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

#ifndef ROMEA_CORE_COMMON__MATH__HISTOGRAM_HPP_
#define ROMEA_CORE_COMMON__MATH__HISTOGRAM_HPP_

#include <Eigen/Core>

namespace romea
{
namespace core
{

template<typename DataContainer, typename Scalar = typename DataContainer::Scalar>
void computeHistogram(const DataContainer& input,
                      size_t nbins,
                      Scalar low_range,
                      Scalar high_range,
                      Eigen::Matrix<Scalar, -1, 2>& bins_data);

template<typename DataContainer, typename ScalarOut, typename Scalar, int SIZE>
void computeHistogram(const DataContainer& input,
                      size_t nbins,
                      Scalar low_range,
                      Scalar high_range,
                      Eigen::Matrix<ScalarOut, SIZE, 1>& bins,
                      Eigen::Matrix<ScalarOut, SIZE, 1>& bins_data);

template<typename DataContainer, typename Scalar>
auto computeHistogram(const DataContainer& input,
                      size_t nbins,
                      Scalar low_range,
                      Scalar high_range);


template<typename DataContainer, typename Scalar, typename ScalarOut>
void computeHistogram(const DataContainer& input,
                      size_t nbins,
                      Scalar low_range,
                      Scalar high_range,
                      Eigen::Matrix<ScalarOut, -1, 2>& bins_data)
{
  // Implementation taken from PCL Library
  //resizing the vector to nbins to store histogram;
  bins_data.resize(nbins,2);

  ScalarOut bin_size = (high_range - low_range) / (nbins);
  if (bin_size == 0) bin_size = 1.0;

  bins_data.col(0) = bins_data.col(0).LinSpaced(nbins, low_range, high_range-bin_size);
  bins_data.col(0).array() += bin_size/2.0;

  //fill the freq for each data
  for (size_t i=0; i<input.size(); i++)
  {
    if (std::isfinite (input[i]))
    {
      auto index = (size_t) (std::round ((input[i] - low_range) / bin_size)); // was initialy std::floor
      if (index == (size_t) nbins) index =  nbins - 1; //including right boundary
      //std::cout << bins_data(index,0) << " " << bins_data(index,1) <<std::endl;
      bins_data(index,1)++;
    }
  }
}

template<typename DataContainer, typename ScalarOut, typename Scalar, int SIZE>
void computeHistogram(const DataContainer& input,
                      size_t nbins,
                      Scalar low_range,
                      Scalar high_range,
                      Eigen::Matrix<ScalarOut, SIZE, 1>& bins,
                      Eigen::Matrix<ScalarOut, SIZE, 1>& bins_data)
{
    Eigen::Matrix<Scalar, -1, 2> hist;
    computeHistogram(input, nbins, low_range, high_range, hist);
    bins = hist.col(0);
    bins_data = hist.col(1);
}

template<typename DataContainer, typename Scalar>
auto computeHistogram(const DataContainer& input,
                      size_t nbins,
                      Scalar low_range,
                      Scalar high_range)
{
    Eigen::Matrix<Scalar, -1, 2> bins_data;
    computeHistogram<DataContainer, Scalar, Scalar>(input, nbins, low_range, high_range, bins_data);
    return bins_data;
}

}  // namespace core
}  // namespace romea

#endif