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

#ifndef ROMEA_CORE_COMMON__SIGNAL__LOCALMAXSEARCH_HPP_
#define ROMEA_CORE_COMMON__SIGNAL__LOCALMAXSEARCH_HPP_

#include <Eigen/Core>
#include "romea_core_common/math/Interval.hpp"

namespace romea
{
namespace core
{

template<typename Container>
std::vector<Interval<double, 1>> getIntervalMaxs(const Container& input, double bump_height, double min_value);

template<typename Container>
int findBackwardBound(const Container& input, size_t i, double bump_percentage);

template<typename Container>
int findForwardBound(const Container& input, size_t i, double bump_percentage);


template<typename Container>
std::vector<Interval<double, 1>> getIntervalMaxs(const Container& input, double bump_height, double min_value)
{
  std::vector<Interval<double, 1>> max_bounds;

  size_t i = 0;
  if (input[i] >= min_value && input[i+1] < input[i])
  {
    int idx_min = 0;
    int idx_max = findForwardBound(input, i, bump_height);

    if (idx_max>-1)
    {
      max_bounds.push_back({idx_min, idx_max});
    }
  }

  // Cases i = 0 & i = input.size()-1 handled outside loop
  // If inside loop => input[i-1]<input[i] & input[i+1]<input[i] criteria need to be disjoint
  // causing more max detection : detection validated on rising edge OR falling edge.
  // If outside loop => detections validated on rising edge AND falling edge
  for (i=1; i<input.size()-1; i++)
  {
    if (input[i] >= min_value && input[i-1] < input[i] && input[i+1] < input[i])
    {
      // Case input[i] constant : "<" not "<="
      // Found local max. : if current data higher than previous & next one
      int idx_min = findBackwardBound(input, i, bump_height);
      int idx_max = findForwardBound(input, i, bump_height);

      // if left bound is found & right bound isn't found due to signal crop => we have found a max at the beginning of signal
      if (idx_min>-1 && idx_max==-1)
        idx_max = input.size()-1;
      // if right bound is found & left bound isn't found due to signal crop => we have found a max at the beginning of signal
      else if (idx_max>-1 && idx_min==-1)
        idx_min = 0;
      // if left or right bound are not found
      else if (idx_max<0 || idx_min<0)
        continue;
      max_bounds.push_back({idx_min, idx_max});
    }
  }

  i = input.size()-1;
  if (input[i] >= min_value && input[i-1] < input[i])
  {
    int idx_min = findBackwardBound(input, i, bump_height);
    int idx_max = input.size()-1;

    if (idx_min>-1)
    {
      max_bounds.push_back({idx_min, idx_max});
    }
  }

  return max_bounds;
}

template<typename Container>
int findBackwardBound(const Container& input, size_t i, double bump_height)
{
  double minimum_bump_size = bump_height;
  double maximum_bump_size = bump_height;

  for (size_t backward_idx=1; backward_idx<i+1; backward_idx++)
  {
    if (input[i-backward_idx] < input[i]-bump_height)
      return int(i-backward_idx);
    else if (input[i-backward_idx] > input[i]+bump_height)
      return -2; // Rising edge towards backward indexes
  }
  return -1; // Backward bound not found
}

template<typename Container>
int findForwardBound(const Container& input, size_t i, double bump_height)
{
  double minimum_bump_size = bump_height;
  double maximum_bump_size = bump_height;

  for (size_t forward_idx=1; forward_idx<input.size()-i; forward_idx++)
  {
    if (input[i+forward_idx] < input[i]-minimum_bump_size)
      return int(i+forward_idx);
    else if (input[i+forward_idx] > input[i]+maximum_bump_size)
      return -2; // Rising edge towards forward indexes
  }
  return -1; // Forward bound not found
}

}  // namespace core
}  // namespace romea

#endif