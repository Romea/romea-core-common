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


#ifndef ROMEA_CORE_COMMON__CONCURRENCY__SHAREDVARIABLE_HPP_
#define ROMEA_CORE_COMMON__CONCURRENCY__SHAREDVARIABLE_HPP_

#include <mutex>

namespace romea
{
namespace core
{

template<typename T>
class SharedVariable
{
public:
  SharedVariable();
  explicit SharedVariable(const T & value);

  SharedVariable & operator=(const T & value);
  SharedVariable(const SharedVariable &) = delete;
  SharedVariable & operator=(const SharedVariable &) = delete;

public:
  operator T() const;
  void store(const T & value);
  T load() const;

private:
  T value_;
  mutable std::mutex mutex_;
};

template<typename T>
using ThreadSafeVariable = SharedVariable<T>;

//-----------------------------------------------------------------------------
template<typename T>
SharedVariable<T>::SharedVariable()
: value_(T()), mutex_()
{
}

//-----------------------------------------------------------------------------
template<typename T>
SharedVariable<T>::SharedVariable(const T & value)
: value_(value), mutex_()
{
}

//-----------------------------------------------------------------------------
template<typename T>
SharedVariable<T> & SharedVariable<T>::operator=(const T & value)
{
  store(value);
  return *this;
}

//-----------------------------------------------------------------------------
template<typename T>
SharedVariable<T>::operator T() const
{
  return load();
}

//-----------------------------------------------------------------------------
template<typename T>
void SharedVariable<T>::store(const T & value)
{
  std::lock_guard<std::mutex> lock(mutex_);
  value_ = value;
}

//-----------------------------------------------------------------------------
template<typename T>
T SharedVariable<T>::load() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  return value_;
}

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__CONCURRENCY__SHAREDVARIABLE_HPP_
