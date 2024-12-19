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


#ifndef ROMEA_CORE_COMMON__MONITORING__RATEMONITORING_HPP_
#define ROMEA_CORE_COMMON__MONITORING__RATEMONITORING_HPP_

// std
#include <queue>
#include <atomic>

// romea
#include "romea_core_common/time/Time.hpp"
#include "romea_core_common/concurrency/SharedVariable.hpp"

namespace romea
{
namespace core
{

class RateMonitoring
{
public:
  RateMonitoring();

  explicit RateMonitoring(const double & expectedRate);

  RateMonitoring(const RateMonitoring & RateMonitoring);

  void initialize(const double & expectedRate);

  double update(const Duration & duration);

  double getRate()const;

  bool timeout(const Duration & duration);

private:
  size_t windowSize_;

  Duration lastPeriod_;
  SharedVariable<Duration> lastDuration_;
  std::queue<int64_t> periods_;
  int64_t periodsSum_;
  std::atomic<double> rate_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__MONITORING__RATEMONITORING_HPP_
