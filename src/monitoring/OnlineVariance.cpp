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
#include "romea_core_common/monitoring/OnlineVariance.hpp"

// std
#include <cassert>
#include <limits>

namespace romea
{
namespace core
{


//-----------------------------------------------------------------------------
OnlineVariance::OnlineVariance(const double & averagePrecision, size_t windowSize)
: OnlineAverage(averagePrecision, windowSize),
  windowSizeMinusOne_(windowSize - 1),
  squaredMultiplier_(multiplier_ * multiplier_),
  squaredData_(),
  sumOfSquaredData_(0),
  variance_(std::numeric_limits<double>::quiet_NaN())
{
}

//-----------------------------------------------------------------------------
OnlineVariance::OnlineVariance(const double & averagePrecision)
: OnlineVariance(averagePrecision, 0)
{
}

//-----------------------------------------------------------------------------
OnlineVariance::OnlineVariance(const OnlineVariance & onlineVariance)
: OnlineAverage(onlineVariance),
  windowSizeMinusOne_(onlineVariance.windowSizeMinusOne_),
  squaredMultiplier_(onlineVariance.squaredMultiplier_),
  squaredData_(onlineVariance.squaredData_),
  sumOfSquaredData_(onlineVariance.sumOfSquaredData_),
  variance_(onlineVariance.variance_)
{
}

//-----------------------------------------------------------------------------
void OnlineVariance::setWindowSize(const size_t & windowSize)
{
  windowSize_ = windowSize;
  windowSizeMinusOne_ = windowSize - 1;
  data_.reserve(windowSize_);
  squaredData_.reserve(windowSize_);
}

//-----------------------------------------------------------------------------
double OnlineVariance::getVariance()const
{
  std::lock_guard<std::mutex> lock(mutex_);
  return variance_;
}

//-----------------------------------------------------------------------------
void OnlineVariance::update(const double & value)
{
  std::lock_guard<std::mutex> lock(mutex_);
  int64_t integerValue = static_cast<int64_t>(value * multiplier_);
  int64_t squaredIntegerValue = integerValue * integerValue;

  sumOfData_ += integerValue;
  sumOfSquaredData_ += squaredIntegerValue;

  if (data_.size() != windowSize_) {
    data_.push_back(integerValue);
    squaredData_.push_back(squaredIntegerValue);
  } else {
    sumOfData_ -= data_[index_];
    sumOfSquaredData_ -= squaredData_[index_];
    data_[index_] = integerValue;
    squaredData_[index_] = squaredIntegerValue;
  }

  double average = sumOfData_ / (static_cast<double>(multiplier_) * data_.size());
  double squaredAverage = (sumOfSquaredData_) / static_cast<double>(squaredMultiplier_);
  double variance = (squaredAverage - data_.size() * average * average) / (windowSizeMinusOne_);
  average_ = average;
  variance_ = variance;

  index_ = (index_ + 1) % windowSize_;
}

//-----------------------------------------------------------------------------
void OnlineVariance::reset()
{
  std::lock_guard<std::mutex> lock(mutex_);

  data_.clear();
  squaredData_.clear();

  sumOfData_ = 0;
  sumOfSquaredData_ = 0;
  average_ = std::numeric_limits<double>::quiet_NaN();
  variance_ = std::numeric_limits<double>::quiet_NaN();
}

}   // namespace core
}   // namespace romea
