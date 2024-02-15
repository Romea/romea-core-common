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


// gtest
#include <gtest/gtest.h>

// romea
#include "romea_core_common/math/EulerAngles.hpp"
#include "romea_core_common/math/Matrix.hpp"
#include "romea_core_common/math/Interval.hpp"

//-----------------------------------------------------------------------------
TEST(TestInterval, testInterval1Di)
{
  int lower1 = 0;
  int upper1 = 10;
  romea::core::Interval1D<int> interval1(lower1, upper1);
  EXPECT_EQ(interval1.lower(), lower1);
  EXPECT_EQ(interval1.upper(), upper1);
  EXPECT_EQ(interval1.center(), 5);
  EXPECT_EQ(interval1.width(), 10);

  int lower2 = -3;
  int upper2 = 10;
  romea::core::Interval1D<int> interval2(lower2, upper2);
  EXPECT_EQ(interval2.lower(), lower2);
  EXPECT_EQ(interval2.upper(), upper2);
  EXPECT_EQ(interval2.center(), 3);
  EXPECT_EQ(interval2.width(), 13);
}


//-----------------------------------------------------------------------------
TEST(TestInterval, testInterval1Df)
{
  double lower1 = 0.;
  double upper1 = 10.;
  romea::core::Interval1D<double> interval1(lower1, upper1);
  EXPECT_DOUBLE_EQ(interval1.lower(), lower1);
  EXPECT_DOUBLE_EQ(interval1.upper(), upper1);
  EXPECT_DOUBLE_EQ(interval1.center(), 5.);
  EXPECT_DOUBLE_EQ(interval1.width(), 10.);

  double lower2 = -3.;
  double upper2 = 10.;
  romea::core::Interval1D<double> interval2(lower2, upper2);
  EXPECT_DOUBLE_EQ(interval2.lower(), lower2);
  EXPECT_DOUBLE_EQ(interval2.upper(), upper2);
  EXPECT_DOUBLE_EQ(interval2.center(), 3.5);
  EXPECT_DOUBLE_EQ(interval2.width(), 13.);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testInsideInterval1Di)
{
  int lower = 0;
  int upper = 10;
  romea::core::Interval1D<int> interval(lower, upper);
  EXPECT_TRUE(interval.inside(lower));
  EXPECT_TRUE(interval.inside(upper));
  EXPECT_TRUE(interval.inside(interval.center()));
  EXPECT_FALSE(interval.inside(lower - 1));
  EXPECT_FALSE(interval.inside(upper + 1));
}


//-----------------------------------------------------------------------------
TEST(TestInterval, testInsideInterval1Df)
{
  double lower = 0.3;
  double upper = 10.2;
  romea::core::Interval1D<double> interval(lower, upper);
  EXPECT_TRUE(interval.inside(lower));
  EXPECT_TRUE(interval.inside(upper));
  EXPECT_TRUE(interval.inside(interval.center()));
  EXPECT_FALSE(interval.inside(lower - 0.1));
  EXPECT_FALSE(interval.inside(upper + 0.1));
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testInsideInterval2Di)
{
  Eigen::Vector2i lower{0, -5};
  Eigen::Vector2i upper{10, 20};
  romea::core::Interval2D<int> interval(lower, upper);
  EXPECT_EQ(interval.lower().x(), lower.x());
  EXPECT_EQ(interval.lower().y(), lower.y());
  EXPECT_EQ(interval.upper().x(), upper.x());
  EXPECT_EQ(interval.upper().y(), upper.y());
  EXPECT_TRUE(interval.inside(lower));
  EXPECT_TRUE(interval.inside(upper));
//  EXPECT_TRUE(interval.inside(interval.center()));
  EXPECT_FALSE(interval.inside(lower.array() - 1));
  EXPECT_FALSE(interval.inside(upper.array() + 1));
  EXPECT_FALSE(interval.inside(Eigen::Vector2i{-1, 10}));
  EXPECT_FALSE(interval.inside(Eigen::Vector2i{3, 21}));
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testInsideInterval2Df)
{
  Eigen::Vector2d lower{0.4, -5.2};
  Eigen::Vector2d upper{10.2, 20.5};
  romea::core::Interval2D<double> interval(lower, upper);
  EXPECT_DOUBLE_EQ(interval.lower().x(), lower.x());
  EXPECT_DOUBLE_EQ(interval.lower().y(), lower.y());
  EXPECT_DOUBLE_EQ(interval.upper().x(), upper.x());
  EXPECT_DOUBLE_EQ(interval.upper().y(), upper.y());
  EXPECT_TRUE(interval.inside(lower));
  EXPECT_TRUE(interval.inside(upper));
  EXPECT_TRUE(interval.inside(interval.center()));
  EXPECT_FALSE(interval.inside(lower.array() - 0.1));
  EXPECT_FALSE(interval.inside(upper.array() + 0.1));
  EXPECT_FALSE(interval.inside(Eigen::Vector2d{0.3, 10}));
  EXPECT_FALSE(interval.inside(Eigen::Vector2d{4.1, 20.6}));
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testInterval1DiInclude)
{
  int lower1 = 0;
  int upper1 = 10;
  int lower2 = 5;
  int upper2 = 15;
  int lower3 = -5;
  int upper3 = 20;
  int lower4 = -2;
  int upper4 = 8;

  romea::core::Interval1D<int> interval(lower1, upper1);

  interval.include(romea::core::Interval1D<int>(lower2, upper2));
  EXPECT_EQ(interval.lower(), lower1);
  EXPECT_EQ(interval.upper(), upper2);

  interval.include(romea::core::Interval1D<int>(lower3, upper3));
  EXPECT_EQ(interval.lower(), lower3);
  EXPECT_EQ(interval.upper(), upper3);

  interval.include(romea::core::Interval1D<int>(lower4, upper4));
  EXPECT_EQ(interval.lower(), lower3);
  EXPECT_EQ(interval.upper(), upper3);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testInterval2DiInclude)
{
  Eigen::Vector2i lower1(0, 10);
  Eigen::Vector2i upper1(5, 20);
  Eigen::Vector2i lower2(-2, 11);
  Eigen::Vector2i upper2(3, 25);
  Eigen::Vector2i lower3(10, 30);
  Eigen::Vector2i upper3(20, 40);
  Eigen::Vector2i lower4(5, 15);
  Eigen::Vector2i upper4(8, 20);

  romea::core::Interval2D<int> interval(lower1, upper1);

  interval.include(romea::core::Interval2D<int>(lower2, upper2));
  EXPECT_EQ(interval.lower().x(), lower2.x());
  EXPECT_EQ(interval.lower().y(), lower1.y());
  EXPECT_EQ(interval.upper().x(), upper1.x());
  EXPECT_EQ(interval.upper().y(), upper2.y());

  interval.include(romea::core::Interval2D<int>(lower3, upper3));
  EXPECT_EQ(interval.lower().x(), lower2.x());
  EXPECT_EQ(interval.lower().y(), lower1.y());
  EXPECT_EQ(interval.upper().x(), upper3.x());
  EXPECT_EQ(interval.upper().y(), upper3.y());

  interval.include(romea::core::Interval2D<int>(lower4, upper4));
  EXPECT_EQ(interval.lower().x(), lower2.x());
  EXPECT_EQ(interval.lower().y(), lower1.y());
  EXPECT_EQ(interval.upper().x(), upper3.x());
  EXPECT_EQ(interval.upper().y(), upper3.y());
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testIntervalAddOperator1D)
{
  double lower1 = 3.2;
  double upper1 = 8.5;
  double lower2 = -7.3;
  double upper2 = -2.4;

  romea::core::Interval1D<double> interval(lower1, upper1);
  romea::core::Interval1D<double> interval2(lower2, upper2);
  auto ret = interval + interval2;

  EXPECT_DOUBLE_EQ(ret.lower(), -4.1);
  EXPECT_DOUBLE_EQ(ret.upper(), 6.1);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testIntervalMinusOperator1D)
{
  double lower1 = 3.2;
  double upper1 = 8.5;
  double lower2 = -7.3;
  double upper2 = -2.4;

  romea::core::Interval1D<double> interval(lower1, upper1);
  romea::core::Interval1D<double> interval2(lower2, upper2);
  auto ret = interval - interval2;

  EXPECT_DOUBLE_EQ(ret.lower(), 10.5);
  EXPECT_DOUBLE_EQ(ret.upper(), 10.9);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testIntervalTimesEqualOperator1D)
{
  double lower1 = 3.2;
  double upper1 = 8.5;
  double lower2 = -7.3;
  double upper2 = -2.4;

  romea::core::Interval1D<double> interval(lower1, upper1);
  romea::core::Interval1D<double> interval2(lower2, upper2);
  interval *= interval2;

  EXPECT_DOUBLE_EQ(interval.lower(), -62.05);
  EXPECT_DOUBLE_EQ(interval.upper(), -7.68);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testIntervalIntersectOperator1D)
{
  romea::core::Interval1D<double> interval(3.2, 8.5);
  romea::core::Interval1D<double> interval2(-7.3, -2.4);
  auto ret = interval & interval2;

  EXPECT_TRUE(std::isnan(ret.lower()));
  EXPECT_TRUE(std::isnan(ret.upper()));

  romea::core::Interval1D<double> interval3(1.2, 12.4);
  romea::core::Interval1D<double> interval4(4.7, 78.1);
  auto ret2 = interval3 & interval4;
  EXPECT_DOUBLE_EQ(ret2.lower(), 4.7);
  EXPECT_DOUBLE_EQ(ret2.upper(), 12.4);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testIntervalUnionOperator1D)
{
  romea::core::Interval1D<double> interval(3.2, 8.5);
  romea::core::Interval1D<double> interval2(-7.3, -2.4);
  auto ret = interval | interval2;

  EXPECT_DOUBLE_EQ(ret.lower(), -7.3);
  EXPECT_DOUBLE_EQ(ret.upper(), 8.5);
}

//-----------------------------------------------------------------------------
TEST(TestInterval, testIntervalStructuredBinding1D)
{
  romea::core::Interval1D<double> interval(3.2, 8.5);
  auto [a,b] = interval;

  EXPECT_DOUBLE_EQ(a, 3.2);
  EXPECT_DOUBLE_EQ(b, 8.5);
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
