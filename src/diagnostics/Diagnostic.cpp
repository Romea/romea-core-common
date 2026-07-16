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
#include <cassert>
#include <string>
#include <list>

// romea
#include "romea_core_common/diagnostic/Diagnostic.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
Diagnostic::Diagnostic()
: status(DiagnosticStatus::STALE),
  message()
{
}

//-----------------------------------------------------------------------------
Diagnostic::Diagnostic(
  const DiagnosticStatus & status,
  const std::string & message)
: status(status),
  message(message)
{
}

//-----------------------------------------------------------------------------
DiagnosticStatus worseStatus(const std::list<Diagnostic> & diagnostics)
{
  assert(!diagnostics.empty());

  auto it = std::cbegin(diagnostics);
  DiagnosticStatus status = it->status;

  while (++it != std::cend(diagnostics)) {
    status = worse(status, it->status);
  }

  return status;
}

//-----------------------------------------------------------------------------
bool allOK(const std::list<Diagnostic> & diagnostics)
{
  assert(!diagnostics.empty());
  return worseStatus(diagnostics) == DiagnosticStatus::OK;
}

//-----------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Diagnostic & diagnostic)
{
  os << diagnostic.status << " : " << diagnostic.message;
  return os;
}

}  // namespace core
}  // namespace romea
