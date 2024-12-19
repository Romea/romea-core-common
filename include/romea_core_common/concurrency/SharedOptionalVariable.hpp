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


#ifndef ROMEA_CORE_COMMON__CONCURRENCY__SHAREDOPTIONALVARIABLE_HPP_
#define ROMEA_CORE_COMMON__CONCURRENCY__SHAREDOPTIONALVARIABLE_HPP_

// std
#include <optional>
#include <mutex>

namespace romea
{
namespace core
{

template<typename T>
class SharedOptionalVariable
{
public:
  SharedOptionalVariable();
  explicit SharedOptionalVariable(const T & value);

  SharedOptionalVariable & operator=(const T & value) = delete;
  SharedOptionalVariable(const SharedOptionalVariable &) = delete;
  SharedOptionalVariable & operator=(const SharedOptionalVariable &) = delete;

public:
  void store(const T & value);
  std::optional<T> consume();

private:
  std::optional<T> value_;
  mutable std::mutex mutex_;
};

//-----------------------------------------------------------------------------
template<typename T>
SharedOptionalVariable<T>::SharedOptionalVariable()
: value_(),
  mutex_()
{
}

//-----------------------------------------------------------------------------
template<typename T>
SharedOptionalVariable<T>::SharedOptionalVariable(const T & value)
: value_(value),
  mutex_()
{
}

// //-----------------------------------------------------------------------------
// template <typename T>
// ProducerConsumerVariable<T> & ProducerConsumerVariable<T>::operator=(const T & value)
// {
//  store(value);
//  return *this;
// }

//-----------------------------------------------------------------------------
template<typename T>
void SharedOptionalVariable<T>::store(const T & value)
{
  std::lock_guard<std::mutex> lock(mutex_);
  value_ = value;
}

//-----------------------------------------------------------------------------
template<typename T>
std::optional<T> SharedOptionalVariable<T>::consume()
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto value = value_;
  value_.reset();
  return value;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__CONCURRENCY__SHAREDOPTIONALVARIABLE_HPP_
