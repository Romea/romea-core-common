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


#ifndef ROMEA_CORE_COMMON__DIAGNOSTIC__DIAGNOSTICREPORT_HPP_
#define ROMEA_CORE_COMMON__DIAGNOSTIC__DIAGNOSTICREPORT_HPP_

// std
#include <optional>
#include <sstream>
#include <string>
#include <list>
#include <map>

// romea
#include "romea_core_common/diagnostic/Diagnostic.hpp"

namespace romea
{
namespace core
{

struct DiagnosticReport
{
  DiagnosticReport();
  std::list<Diagnostic> diagnostics;
  std::map<std::string, std::string> info;
};

std::ostream & operator<<(std::ostream & os, const DiagnosticReport & report);

DiagnosticReport & operator+=(DiagnosticReport & report1, const DiagnosticReport & report2);

//-----------------------------------------------------------------------------
template<typename T>
std::string toStringInfoValue(const T & infoValue)
{
  std::ostringstream os;
  if constexpr (std::is_same_v<T, bool>) {
    os << std::boolalpha;
  }
  os << infoValue;
  return os.str();
}

//-----------------------------------------------------------------------------
template<typename T>
void setReportInfo(
  DiagnosticReport & report,
  const std::string & infoName,
  const T & infoValue)
{
  report.info[infoName] = toStringInfoValue(infoValue);
}

//-----------------------------------------------------------------------------
template<typename T>
void setReportInfo(
  DiagnosticReport & report,
  const std::string & infoName,
  const std::optional<T> & infoValue)
{
  if (infoValue.has_value()) {
    setReportInfo(report, infoName, *infoValue);
  } else {
    report.info[infoName] = "";
  }
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__DIAGNOSTIC__DIAGNOSTICREPORT_HPP_
