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


#ifndef ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPEQUALTO_HPP_
#define ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPEQUALTO_HPP_

// std
#include <string>

// romea
#include "romea_core_common/diagnostic/Checkup.hpp"

namespace romea
{
namespace core
{


template<typename T>
class CheckupEqualTo : public Checkup<T>
{
public:
  CheckupEqualTo(
    const std::string & name,
    const T & desired_value,
    const T & epsilon,
    const Diagnostic & initialDiagnostic = Diagnostic());

  DiagnosticStatus evaluate(const T & value) override;
};

//-----------------------------------------------------------------------------
template<typename T>
CheckupEqualTo<T>::CheckupEqualTo(
  const std::string & name,
  const T & desired_value,
  const T & epsilon,
  const Diagnostic & initialDiagnostic)
: Checkup<T>(name, desired_value, epsilon, initialDiagnostic)
{
}

//-----------------------------------------------------------------------------
template<typename T>
DiagnosticStatus CheckupEqualTo<T>::evaluate(const T & value)
{
  std::lock_guard<std::mutex> lock(this->mutex_);
  if (value < this->value_to_compare_with_ - this->epsilon_) {
    this->setDiagnostic_(DiagnosticStatus::ERROR, " is too low.");
  } else if (value > this->value_to_compare_with_ + this->epsilon_) {
    this->setDiagnostic_(DiagnosticStatus::ERROR, " is too high.");
  } else {
    this->setDiagnostic_(DiagnosticStatus::OK, " is OK.");
  }

  this->setValue_(value);
  return this->getStatus_();
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUPEQUALTO_HPP_
