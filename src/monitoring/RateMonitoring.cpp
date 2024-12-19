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

// local
#include "romea_core_common/monitoring/RateMonitoring.hpp"

namespace
{
const size_t MINIMAL_WINDOW_SIZE = 4;
const size_t MAXIMAL_WINDOW_SIZE = 64;
}

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
RateMonitoring::RateMonitoring()
: windowSize_(0),
  lastPeriod_(),
  lastDuration_(Duration::zero()),
  periods_(),
  periodsSum_(0),
  rate_(0)
{
}

//-----------------------------------------------------------------------------
RateMonitoring::RateMonitoring(const double & expectedRate)
: RateMonitoring::RateMonitoring()
{
  initialize(expectedRate);
}

//-----------------------------------------------------------------------------
RateMonitoring::RateMonitoring(const RateMonitoring & rateMonitoring)
: windowSize_(rateMonitoring.windowSize_),
  lastPeriod_(rateMonitoring.lastPeriod_),
  lastDuration_(rateMonitoring.lastDuration_.load()),
  periods_(rateMonitoring.periods_),
  periodsSum_(rateMonitoring.periodsSum_),
  rate_(rateMonitoring.rate_.load())
{
}


//-----------------------------------------------------------------------------
void RateMonitoring::initialize(const double & expectedRate)
{
  windowSize_ = static_cast<size_t>(2 * expectedRate);
  windowSize_ = std::max(windowSize_, MINIMAL_WINDOW_SIZE);
  windowSize_ = std::min(windowSize_, MAXIMAL_WINDOW_SIZE);
}

//-----------------------------------------------------------------------------
double RateMonitoring::update(const Duration & duration)
{
  assert(windowSize_ != 0);

  lastPeriod_ = duration - lastDuration_.load();
  int64_t lastPeriodInNanoSecond = durationToNanoSecond(lastPeriod_);

  periods_.push(lastPeriodInNanoSecond);
  periodsSum_ += lastPeriodInNanoSecond;

  if (periods_.size() == windowSize_ + 1) {
    periodsSum_ -= periods_.front();
    periods_.pop();

    rate_.store(1000000000. / (periodsSum_ / static_cast<double>(windowSize_)));
  }

  lastDuration_.store(duration);
  return rate_.load();
}

//-----------------------------------------------------------------------------
double RateMonitoring::getRate()const
{
  return rate_.load();
}

//-----------------------------------------------------------------------------
bool RateMonitoring::timeout(const Duration & duration)
{
  if (!periods_.empty() &&
    durationToSecond(duration - lastDuration_.load()) > 0.5)
  {
    rate_.store(0.);
    return true;
  }
  return false;
}

}   // namespace core
}   // namespace romea
