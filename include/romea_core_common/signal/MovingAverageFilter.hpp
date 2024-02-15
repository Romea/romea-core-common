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

#ifndef ROMEA_CORE_COMMON__SIGNAL__MOVINGAVERAGEFILTER_HPP_
#define ROMEA_CORE_COMMON__SIGNAL__MOVINGAVERAGEFILTER_HPP_

#include <Eigen/Core>

namespace romea
{
namespace core
{

template<typename ContainerIn, typename ContainerOut>
void movingAverageFilter(const ContainerIn& input,
                         ContainerOut& output,
                         size_t smooth_length);

template<typename Container>
auto movingAverageFilter(const Container& input,
                         size_t smooth_length);


/***********************************************************/
template<typename ContainerIn, typename ContainerOut>
void movingAverageFilter(const ContainerIn& input_,
                         ContainerOut& output,
                         size_t smooth_length)
{
  static_assert(std::is_same<typename ContainerIn::Scalar, typename ContainerOut::Scalar>::value,
                "Input and output containers must have the same scalar type.");
  static_assert(ContainerIn::IsVectorAtCompileTime && ContainerOut::IsVectorAtCompileTime,
                "Input and output containers must be 1D vectors.");
  using Scalar = typename ContainerIn::Scalar;
  auto input = input_;

  assert(input.size() >= smooth_length + 1);
  assert(output.size() == input.size());

  output(0) = input.head(smooth_length + 1).sum();

  size_t end = input.size()-smooth_length-1;
  for (size_t i=1; i<input.size(); i++)
  {
    Scalar prev = (i > smooth_length) ? input(i - smooth_length - 1) : 0.0;
    Scalar next = (i <= input.size() - smooth_length - 1) ? input(i + smooth_length) : 0.0;
    output(i) = output(i - 1) - prev + next;
  }

  for (size_t i=0; i<output.size(); i++)
  {
    size_t normalization_factor = std::min({2 * smooth_length, smooth_length + i + 1, input.size() - i + smooth_length});
    output(i) = output(i) / normalization_factor;
  }
}

template<typename Container>
auto movingAverageFilter(const Container& input,
                         size_t smooth_length)
{
  Eigen::Matrix<typename Container::Scalar, -1, 1> output(input.size());
  movingAverageFilter(input, output, smooth_length);
  return output;
}

} // namespace core
} // namespace romea

#endif // ROMEA_CORE_COMMON__SIGNAL__MOVINGAVERAGEFILTER_HPP_