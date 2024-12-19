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


#ifndef ROMEA_CORE_COMMON__MONITORING__ONLINEVARIANCE_HPP_
#define ROMEA_CORE_COMMON__MONITORING__ONLINEVARIANCE_HPP_

// std
#include <vector>

// local
#include "romea_core_common/monitoring/OnlineAverage.hpp"

namespace romea
{
namespace core
{

class OnlineVariance : public OnlineAverage
{
public:
  explicit OnlineVariance(const double & averagePrecision);

  OnlineVariance(const double & averagePrecision, size_t windowSize);

  OnlineVariance(const OnlineVariance & onlineVariance);

  virtual ~OnlineVariance() = default;

public:
  void setWindowSize(const size_t & windowSize) override;

  void update(const double & value) override;

  void reset() override;

  double getVariance()const;

private:
  size_t windowSizeMinusOne_;
  int squaredMultiplier_;

  std::vector<int64_t> squaredData_;
  int64_t sumOfSquaredData_;
  double variance_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__MONITORING__ONLINEVARIANCE_HPP_
