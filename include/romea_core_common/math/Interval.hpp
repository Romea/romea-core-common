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


#ifndef ROMEA_CORE_COMMON__MATH__INTERVAL_HPP_
#define ROMEA_CORE_COMMON__MATH__INTERVAL_HPP_

// Eigen
#include <Eigen/Core>

// std
#include <utility>
#include <limits>
#include <cassert>
#include <algorithm>


namespace romea
{
namespace core
{


template<typename Scalar, size_t DIM = 1>
class Interval
{
public:
  using T = Eigen::Matrix<Scalar, DIM, 1>;

public:
  Interval()
  : Interval(T::Constant(-std::numeric_limits<Scalar>::max()),
      T::Constant(std::numeric_limits<Scalar>::max()))
  {
  }

  Interval(const T & lower, const T & upper)
  : lower_(lower),
    upper_(upper)
  {
    if((lower_.array() <= upper_.array()).all())
    {
      lower_.array() = T::Constant(std::nan(""));
      upper_.array() = T::Constant(std::nan(""));
    };
  }

public:
  const T & lower() const
  {
    return lower_;
  }

  const T & upper() const
  {
    return upper_;
  }

  T width()const
  {
    return upper_ - lower_;
  }

  T center() const
  {
    return (upper_ + lower_) / 2.;
  }

  void include(const Interval<Scalar, DIM> & interval)
  {
    lower_.array() = lower_.array().min(interval.lower().array());
    upper_.array() = upper_.array().max(interval.upper().array());
  }

  Interval<Scalar, DIM> operator|(const Interval<Scalar, DIM> & interval)
  {
    return Interval<Scalar, DIM>({
      lower_.array().min(interval.lower().array()),
      upper_.array().max(interval.upper().array())
      });
  }

  Interval<Scalar, DIM> operator&(const Interval<Scalar, DIM> & interval)
  {
    return Interval<Scalar, DIM>({
      lower_.array().max(interval.upper().array()),
      upper_.array().min(interval.lower().array())
      });
  }

  bool inside(const T & val)const
  {
    return (val.array() >= lower_.array()).all() && (val.array() <= upper_.array()).all();
  }

  bool is_empty() const
  {
    return (lower_.array().isNaN()||upper_.array().isNaN()).any();
  }

private:
  T lower_;
  T upper_;
};

template<typename Scalar, size_t DIM = 1>
class IntervalComplement
{
  using T = Eigen::Matrix<Scalar, DIM, 1>;

public:
  IntervalComplement()
  : IntervalComplement(Interval<Scalar, DIM>(), Interval<Scalar, DIM>())
  {
  }

  IntervalComplement(
    const Interval<Scalar, DIM> & interval,
    const Interval<Scalar, DIM> & limits)
  : interval_(interval),
    limits_(limits)
  {
    assert(
      (interval_.lower().array() >= limits_.lower().array()).all() &&
      (interval_.upper().array() <= limits_.upper().array()).all());
  }

  explicit IntervalComplement(const Interval<Scalar, DIM> & interval)
  : IntervalComplement(interval, Interval<Scalar, DIM>())
  {
  }

  bool inside(const T & val)const
  {
    return !interval_.inside(val) && limits_.inside(val);
  }

  const Interval<Scalar, DIM> & interval()const
  {
    return interval_;
  }

  const Interval<Scalar, DIM> & limits()const
  {
    return limits_;
  }

private:
  Interval<Scalar, DIM> interval_;
  Interval<Scalar, DIM> limits_;
};


template<typename Scalar>
class Interval<Scalar, 1>
{
private:
  using T = Scalar;

public:
  Interval()
  : Interval(-std::numeric_limits<T>::max(),
      std::numeric_limits<T>::max())
  {
  }

  Interval(const T & lower, const T & upper)
  : lower_(lower),
    upper_(upper)
  {
    if (lower > upper)
    {
      lower_ = T(std::nan(""));
      upper_ = T(std::nan(""));
    }
  }

public:
  const T & lower() const
  {
    return lower_;
  }

  const T & upper() const
  {
    return upper_;
  }

  Interval<T, 1> operator+(const Interval<T, 1>& interval)
  {
    return Interval<T, 1>(lower_ + interval.lower(), upper_ + interval.upper());
  }

  Interval<T, 1> operator-(const Interval<T, 1>& interval)
  {
    return Interval<T, 1>(lower_ - interval.lower(), upper_ - interval.upper());
  }

  Interval<T, 1> operator*=(const Interval<T, 1>& interval)
  {
    auto list = {lower_*interval.lower(),
                 upper_*interval.upper(),
                 upper_*interval.lower(),
                 upper_*interval.upper()};

    return Interval<T, 1>(std::min(list), std::max(list));
  }

  Interval<T, 1> operator&(const Interval<T, 1>& interval)
  {
    return Interval<T, 1>(std::max(lower_, interval.lower()),
                          std::min(upper_, interval.upper()));
  }

  Interval<T, 1> operator|(const Interval<T, 1>& interval)
  {
    return Interval<T, 1>(std::min(lower_, interval.lower()),
                          std::max(upper_, interval.upper()));
  }

  T width()const
  {
    return upper_ - lower_;
  }

  T center() const
  {
    return (upper_ + lower_) / 2.;
  }
  void include(const Interval<T, 1> & interval)
  {
    lower_ = std::min(lower_, interval.lower());
    upper_ = std::max(upper_, interval.upper());
  }

  bool inside(const T & val)const
  {
    return val >= lower_ && val <= upper_;
  }

  template <size_t Index>
  auto get() const {
      if constexpr (Index == 0) return lower_;
      if constexpr (Index == 1) return upper_;
  }

private:
  T lower_;
  T upper_;
};

template<typename Scalar>
class IntervalComplement<Scalar, 1>
{
  using T = Scalar;

public:
  IntervalComplement(
    const Interval<Scalar, 1> & interval,
    const Interval<Scalar, 1> & limits)
  : interval_(interval),
    limits_(limits)
  {
    assert(
      interval_.lower() > limits.lower() &&
      interval.upper() < limits.upper());
  }

  explicit IntervalComplement(const Interval<Scalar, 1> & interval)
  : IntervalComplement(interval, Interval<Scalar, 1>())
  {
  }

  IntervalComplement()
  : IntervalComplement(Interval<T, 1>(0, 0), Interval<T, 1>())
  {
  }

  bool inside(const T & val)const
  {
    return !interval_.inside(val) && limits_.inside(val);
  }

  const Interval<Scalar, 1> & interval()const
  {
    return interval_;
  }

  const Interval<Scalar, 1> & limits()const
  {
    return limits_;
  }

private:
  Interval<T, 1> interval_;
  Interval<T, 1> limits_;
};


template<typename Scalar>
using Interval1D = Interval<Scalar, 1>;
template<typename Scalar>
using Interval2D = Interval<Scalar, 2>;
template<typename Scalar>
using Interval3D = Interval<Scalar, 3>;
template<typename Scalar>
using IntervalComplement1D = IntervalComplement<Scalar, 1>;
template<typename Scalar>
using IntervalComplement2D = IntervalComplement<Scalar, 2>;
template<typename Scalar>
using IntervalComplement3D = IntervalComplement<Scalar, 3>;

template<typename Scalar>
Interval2D<Scalar> to2D(const Interval3D<Scalar> & interval3D)
{
  return Interval2D<Scalar>(
    interval3D.lower().template segment<2>(0),
    interval3D.upper().template segment<2>(0));
}

}  // namespace core
}  // namespace romea

namespace std
{
  template<typename T>
  struct tuple_size<::romea::core::Interval<T, 1>>
      : integral_constant<size_t, 2> {};

  template<typename T>
  struct tuple_element<0, ::romea::core::Interval<T, 1>> {
      using type = T;
  };

  template<typename T>
  struct tuple_element<1, ::romea::core::Interval<T, 1>> {
      using type = T;
  };
}


#endif  // ROMEA_CORE_COMMON__MATH__INTERVAL_HPP_
