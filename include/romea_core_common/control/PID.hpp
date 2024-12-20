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


#ifndef ROMEA_CORE_COMMON__CONTROL__PID_HPP_
#define ROMEA_CORE_COMMON__CONTROL__PID_HPP_

#include "romea_core_common/time/Time.hpp"

namespace romea
{
namespace core
{

class PID
{
public:
  struct Parameters
  {
    double kp;
    double ki;
    double kd;
    double imin;
    double imax;
    double errorEpsilon;
  };

  struct Inputs
  {
    Duration stamp;
    double setpoint;
    double measurement;
    double error() const;
  };

public:
  explicit PID(const Parameters & parameters);

  double compute(const Inputs & inputs);

  void reset();

protected:
  void computeDt_();

  virtual double evaluate_();

  virtual double computeProportionalTerm_();

  virtual double computeDerivativeTerm_();

  virtual double computeIntegralTerm_();

protected:
  double dt_;
  double i_;
  Inputs currentInputs_;
  Inputs previousInputs_;
  Parameters parameters_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__CONTROL__PID_HPP_
