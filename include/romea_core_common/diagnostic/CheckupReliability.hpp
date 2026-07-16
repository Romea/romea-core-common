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


#ifndef ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPRELIABILITY_HPP_
#define ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPRELIABILITY_HPP_

// std
#include <string>
#include <mutex>

// romea
#include "romea_core_common/diagnostic/DiagnosticReport.hpp"

namespace romea
{
namespace core
{

class CheckupReliability
{
public:
  CheckupReliability(
    const std::string & name,
    const double & low_reliability_threshold,
    const double & high_reliability_threshold);

  DiagnosticStatus evaluate(const double & reliability);

  DiagnosticReport getReport()const;

private:
  void setReliabilityValue_(const double & reliability);

  void setDiagnostic_(
    const DiagnosticStatus & status,
    const std::string & messageEnd);

private:
  double low_reliability_threshold_;
  double high_reliability_threshold_;

  mutable std::mutex mutex_;
  DiagnosticReport report_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPRELIABILITY_HPP_
