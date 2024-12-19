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


#ifndef ROMEA_CORE_COMMON__MONITORING__ONLINEAVERAGE_HPP_
#define ROMEA_CORE_COMMON__MONITORING__ONLINEAVERAGE_HPP_

// std
#include <queue>
#include <atomic>
#include <cstdlib>
#include <mutex>
#include <vector>

namespace romea
{
namespace core
{


class OnlineAverage
{
public:
  explicit OnlineAverage(const double & averagePrecision);

  OnlineAverage(const double & averagePrecision, size_t windowSize);

  OnlineAverage(const OnlineAverage & onlineAverage);

  virtual ~OnlineAverage() = default;

public:
  virtual void setWindowSize(const size_t & windowSize);

  const size_t & getWindowSize() const;

  bool isAvailable()const;


  virtual void update(const double & value);

  double getAverage()const;

  virtual void reset();

protected:
  size_t index_;
  size_t windowSize_;
  int multiplier_;

  mutable std::mutex mutex_;
  std::vector<int64_t> data_;
  int64_t sumOfData_;
  double average_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__MONITORING__ONLINEAVERAGE_HPP_
