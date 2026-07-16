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


// romea
#include "romea_core_common/diagnostic/CheckupRate.hpp"


namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
template<typename CheckupType>
CheckupRate<CheckupType>::CheckupRate(
  const std::string & name,
  const double & rate,
  const double & epsilon)
: rateMonitoring_(rate),
  checkup_(name + "_rate",
    rate,
    epsilon,
    Diagnostic(DiagnosticStatus::ERROR, "no data received from " + name))
{
}


//-----------------------------------------------------------------------------
template<typename CheckupType>
DiagnosticStatus CheckupRate<CheckupType>::evaluate(const Duration & stamp)
{
  double rate = rateMonitoring_.update(stamp);
  return checkup_.evaluate(rate);
}

//-----------------------------------------------------------------------------
template<typename CheckupType>
DiagnosticReport CheckupRate<CheckupType>::getReport() const
{
  return checkup_.getReport();
}

//-----------------------------------------------------------------------------
template<typename CheckupType>
bool CheckupRate<CheckupType>::heartBeatCallback(const Duration & stamp)
{
  if (rateMonitoring_.timeout(stamp)) {
    checkup_.timeout();
    return false;
  }
  return true;
}

template class CheckupRate<CheckupGreaterThan<double>>;
template class CheckupRate<CheckupEqualTo<double>>;

}  // namespace core
}  // namespace romea
