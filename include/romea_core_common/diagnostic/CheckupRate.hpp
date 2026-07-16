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


#ifndef ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPRATE_HPP_
#define ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPRATE_HPP_

// std
#include <string>

// romea
#include "romea_core_common/diagnostic/CheckupEqualTo.hpp"
#include "romea_core_common/diagnostic/CheckupGreaterThan.hpp"
#include "romea_core_common/monitoring/RateMonitoring.hpp"

namespace romea
{
namespace core
{

template<typename CheckupType>
class CheckupRate
{
public:
  CheckupRate(
    const std::string & name,
    const double & rate,
    const double & epsilon);

  DiagnosticStatus evaluate(const Duration & stamp);

  bool heartBeatCallback(const Duration & stamp);

  DiagnosticReport getReport()const;

private:
  RateMonitoring rateMonitoring_;
  CheckupType checkup_;
};


using CheckupEqualToRate = CheckupRate<CheckupEqualTo<double>>;
using CheckupGreaterThanRate = CheckupRate<CheckupGreaterThan<double>>;

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPRATE_HPP_
