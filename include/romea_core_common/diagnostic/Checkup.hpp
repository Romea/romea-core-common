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


#ifndef ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUP_HPP_
#define ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUP_HPP_

// std
#include <limits>
#include <sstream>
#include <string>
#include <mutex>

// romea
#include "romea_core_common/diagnostic/DiagnosticReport.hpp"

namespace romea
{
namespace core
{


template<typename T>
class Checkup
{
public:
  Checkup(
    const std::string & name,
    const T & value_to_compare_with_,
    const T & epsilon,
    const Diagnostic & diagnostic = Diagnostic());

  virtual ~Checkup() = default;

  virtual DiagnosticStatus evaluate(const T & value) = 0;

  DiagnosticReport getReport() const;

  void timeout();

protected:
  void setValue_(const T & value);

  void setDiagnostic_(
    const DiagnosticStatus & status,
    const std::string & messageEnd);

  const DiagnosticStatus & getStatus_() const;

protected:
  T value_to_compare_with_;
  T epsilon_;

  mutable std::mutex mutex_;
  DiagnosticReport report_;
};

//-----------------------------------------------------------------------------
template<typename T>
Checkup<T>::Checkup(
  const std::string & name,
  const T & value_to_compare_with,
  const T & epsilon,
  const Diagnostic & diagnostic)
: value_to_compare_with_(value_to_compare_with),
  epsilon_(epsilon),
  mutex_(),
  report_()
{
  report_.diagnostics.push_back(diagnostic);
  report_.info[name] = "";
}

//-----------------------------------------------------------------------------
template<typename T>
DiagnosticReport Checkup<T>::getReport() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  return report_;
}

//-----------------------------------------------------------------------------
template<typename T>
void Checkup<T>::timeout()
{
  std::lock_guard<std::mutex> lock(mutex_);
  setDiagnostic_(DiagnosticStatus::STALE, " timeout.");
  report_.info.begin()->second = "";
}


//-----------------------------------------------------------------------------
template<typename T>
void Checkup<T>::setValue_(const T & value)
{
  report_.info.begin()->second = toStringInfoValue(value);
}

//-----------------------------------------------------------------------------
template<typename T>
void Checkup<T>::setDiagnostic_(
  const DiagnosticStatus & status,
  const std::string & messageEnd)
{
  Diagnostic & diagnostic = report_.diagnostics.front();
  diagnostic.message = report_.info.begin()->first + messageEnd;
  diagnostic.status = status;
}

//-----------------------------------------------------------------------------
template<typename T>
const DiagnosticStatus & Checkup<T>::getStatus_() const
{
  return this->report_.diagnostics.front().status;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__DIAGNOSTIC__CHECKUP_HPP_
