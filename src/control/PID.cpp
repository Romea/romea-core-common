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
#include "romea_core_common/control/PID.hpp"

// std
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
double PID::Inputs::error() const
{
  return setpoint - measurement;
}

//-----------------------------------------------------------------------------
PID::PID(const Parameters & parameters)
: dt_(0),
  currentInputs_(),
  previousInputs_(),
  parameters_(parameters)
{
  currentInputs_.setpoint = std::numeric_limits<double>::quiet_NaN();
}

// //-----------------------------------------------------------------------------
// const double & PID::kp() const
// {
//   return kp_;
// }

// //-----------------------------------------------------------------------------
// const double & PID::ki() const
// {
//   return ki_;
// }

// //-----------------------------------------------------------------------------
// const double & PID::kd() const
// {
//   return kd_;
// }

//-----------------------------------------------------------------------------
double PID::compute(const Inputs & inputs)
{
  previousInputs_ = currentInputs_;
  currentInputs_ = inputs;

  if (std::isfinite(previousInputs_.setpoint)) {
    return evaluate_();
  } else {
    return 0.;
  }
}

//-----------------------------------------------------------------------------
double PID::evaluate_()
{
  std::cout << "current inputs" << currentInputs_.setpoint << " " <<
    currentInputs_.measurement << " " << currentInputs_.error() << std::endl;
  std::cout << "previous inputs" << previousInputs_.setpoint << " " <<
    previousInputs_.measurement << " " << previousInputs_.error() << std::endl;
  std::cout << " gains " << parameters_.kp << " " << parameters_.ki << " " << parameters_.kd <<
    std::endl;
  computeDt_();
  return computeProportionalTerm_() + computeIntegralTerm_() + computeDerivativeTerm_();
}

//-----------------------------------------------------------------------------
void PID::computeDt_()
{
  dt_ = durationToSecond(currentInputs_.stamp - previousInputs_.stamp);
  std::cout << " dt_ " << dt_ << std::endl;
}

//-----------------------------------------------------------------------------
double PID::computeProportionalTerm_()
{
  return parameters_.kp * currentInputs_.error();
}

//-----------------------------------------------------------------------------
double PID::computeDerivativeTerm_()
{
  return parameters_.kd * (currentInputs_.error() - previousInputs_.error()) / dt_;
}

//-----------------------------------------------------------------------------
double PID::computeIntegralTerm_()
{
  if (std::abs(currentInputs_.error()) > parameters_.errorEpsilon * dt_) {
    i_ = std::clamp(i_ + currentInputs_.error() * dt_, parameters_.imin, parameters_.imax);
  } else {
    // std::cout << " reset pid " << std::endl;
    i_ = 0;
  }
  std::cout << " i_ " << i_ << std::endl;
  return parameters_.ki * i_;
}

//-----------------------------------------------------------------------------
void PID::reset()
{
  i_ = 0;
}

}  // namespace core
}  // namespace romea
