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
PID::PID(const Parameters & parameters)
: PID(parameters.kp, parameters.ki, parameters.kd,
    parameters.imin, parameters.imax,
    parameters.error_epsilon)
{
}

//-----------------------------------------------------------------------------
PID::PID(
  const double & kp,
  const double & ki,
  const double & kd,
  const double & imin,
  const double & imax,
  const double & error_epsilon)
: dt_(0),
  kp_(kp),
  ki_(ki),
  kd_(kd),
  imax_(imax),
  imin_(imin),
  i_(0),
  d_(0),
  error_epsilon_(error_epsilon),
  previous_error_(std::numeric_limits<double>::quiet_NaN()),
  previous_error_stamp_()
{
}

//-----------------------------------------------------------------------------
const double & PID::kp() const
{
  return kp_;
}

//-----------------------------------------------------------------------------
const double & PID::ki() const
{
  return ki_;
}

//-----------------------------------------------------------------------------
const double & PID::kd() const
{
  return kd_;
}

//-----------------------------------------------------------------------------
double PID::compute(
  const Duration & stamp,
  const double & setpoint,
  const double & measurement)
{
  double output = 0;
  double error = setpoint - measurement;

  if (std::isfinite(previous_error_)) {
    computeDt_(stamp);
    updateIntegral_(error);
    computeDerivative_(error);
    // std::cout << "error" << error << " d " << d_ << " i " << i_ << std::endl;
    // std::cout << "gains " << kp_ << " " << ki_ << "  " << kd_ << std::endl;
    output = kp_ * setpoint + ki_ * i_ + kd_ * d_;
  }

  previous_error_ = error;
  previous_error_stamp_ = stamp;
  return output;
}

//-----------------------------------------------------------------------------
void PID::computeDt_(const Duration & stamp)
{
  dt_ = durationToSecond(stamp - previous_error_stamp_);
}
//-----------------------------------------------------------------------------
void PID::computeDerivative_(const double & error)
{
  d_ = (error - previous_error_) / dt_;
}

//-----------------------------------------------------------------------------
void PID::updateIntegral_(const double & error)
{
  if (std::abs(error) > error_epsilon_) {
    i_ = std::clamp(i_ + error * dt_, imin_, imax_);
  } else {
    i_ = 0;
  }
}

}  // namespace core
}  // namespace romea
