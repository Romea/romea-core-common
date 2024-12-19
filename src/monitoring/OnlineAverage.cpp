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
#include <limits>

// romea
#include "romea_core_common/monitoring/OnlineAverage.hpp"

namespace romea
{
namespace core
{

//-----------------------------------------------------------------------------
OnlineAverage::OnlineAverage(const double & averagePrecision, size_t windowSize)
: index_(0),
  windowSize_(windowSize),
  multiplier_(static_cast<int>(1 / averagePrecision)),
  data_(),
  sumOfData_(0.0),
  average_(std::numeric_limits<double>::quiet_NaN())
{
}

//-----------------------------------------------------------------------------
OnlineAverage::OnlineAverage(const double & averagePrecision)
: OnlineAverage::OnlineAverage(averagePrecision, 0)
{
}

//-----------------------------------------------------------------------------
OnlineAverage::OnlineAverage(const OnlineAverage & onlineAverage)
: index_(onlineAverage.index_),
  windowSize_(onlineAverage.windowSize_),
  multiplier_(onlineAverage.multiplier_),
  data_(onlineAverage.data_),
  sumOfData_(onlineAverage.sumOfData_),
  average_(onlineAverage.average_)
{
}

//-----------------------------------------------------------------------------
void OnlineAverage::setWindowSize(const size_t & windowSize)
{
  windowSize_ = windowSize;
  data_.reserve(windowSize_);
}


//-----------------------------------------------------------------------------
const size_t & OnlineAverage::getWindowSize() const
{
  return windowSize_;
}


//-----------------------------------------------------------------------------
double OnlineAverage::getAverage()const
{
  std::lock_guard<std::mutex> lock(mutex_);
  return average_;
}


//-----------------------------------------------------------------------------
void OnlineAverage::update(const double & value)
{
  std::lock_guard<std::mutex> lock(mutex_);
  int64_t integerValue = static_cast<int64_t>(value * multiplier_);

  sumOfData_ += integerValue;
  if (data_.size() != windowSize_) {
    data_.push_back(integerValue);
  } else {
    sumOfData_ -= data_[index_];
    data_[index_] = integerValue;
  }
  average_ = sumOfData_ / (static_cast<double>(multiplier_) * data_.size());
  index_ = (index_ + 1) % windowSize_;
}


//-----------------------------------------------------------------------------
bool OnlineAverage::isAvailable()const
{
  return data_.size() == windowSize_;
}

//-----------------------------------------------------------------------------
void OnlineAverage::reset()
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_.clear();
  sumOfData_ = 0;
  average_ = std::numeric_limits<double>::quiet_NaN();
}

}  // namespace core
}  // namespace romea
