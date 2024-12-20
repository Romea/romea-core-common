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
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>

// romea
#include "romea_core_common/control/PIDFeedForwad.hpp"


namespace romea
{
namespace core
{


//-----------------------------------------------------------------------------
PIDFeedForward::PIDFeedForward(const Parameters & parameters, const double & kff)
: PID(parameters), kff_(kff)
{
}

//-----------------------------------------------------------------------------
double PIDFeedForward::evaluate_()
{
  computeDt_();
  return computeProportionalTerm_() + computeIntegralTerm_() +
         computeDerivativeTerm_() + computeFeedForwardTerm_();
}

//-----------------------------------------------------------------------------
double PIDFeedForward::computeDerivativeTerm_()
{
  return -parameters_.kd * (currentInputs_.measurement - previousInputs_.measurement) / dt_;
}

//-----------------------------------------------------------------------------
double PIDFeedForward::computeFeedForwardTerm_()
{
  return kff_ * currentInputs_.setpoint;
}


}  // namespace core
}  // namespace romea
