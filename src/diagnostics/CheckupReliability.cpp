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


// std
#include <string>

// local
#include "romea_core_common/diagnostic/CheckupReliability.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
CheckupReliability::CheckupReliability(
  const std::string & name,
  const double & low_reliability_threshold,
  const double & high_reliability_threshold)
: low_reliability_threshold_(low_reliability_threshold),
  high_reliability_threshold_(high_reliability_threshold),
  report_()
{
  report_.info[name] = "";
  report_.diagnostics.push_back(Diagnostic());
}

//-----------------------------------------------------------------------------
DiagnosticStatus CheckupReliability::evaluate(const double & reliability)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (reliability < low_reliability_threshold_) {
    setDiagnostic_(DiagnosticStatus::ERROR, " is too low.");
  } else if (reliability < high_reliability_threshold_) {
    setDiagnostic_(DiagnosticStatus::WARN, " is uncertain.");
  } else {
    setDiagnostic_(DiagnosticStatus::OK, " is high.");
  }

  setReliabilityValue_(reliability);
  return report_.diagnostics.front().status;
}

//-----------------------------------------------------------------------------
DiagnosticReport CheckupReliability::getReport()const
{
  std::lock_guard<std::mutex> lock(mutex_);
  return report_;
}

//-----------------------------------------------------------------------------
void CheckupReliability::setReliabilityValue_(const double & reliability)
{
  report_.info.begin()->second = toStringInfoValue(reliability);
}

//-----------------------------------------------------------------------------
void CheckupReliability::setDiagnostic_(
  const DiagnosticStatus & status,
  const std::string & messageEnd)
{
  Diagnostic & message = report_.diagnostics.front();
  message.message = report_.info.begin()->first + messageEnd;
  message.status = status;
}

}  // namespace core
}  // namespace romea
