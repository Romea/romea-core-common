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
#include "romea_core_common/math/Algorithm.hpp"


//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testSign)
{
  EXPECT_DOUBLE_EQ(romea::core::sign(-2.3), -1);
  EXPECT_DOUBLE_EQ(romea::core::sign(14.7), 1);
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testClamp)
{
  EXPECT_DOUBLE_EQ(romea::core::clamp(0.5, -1., 1.), 0.5);
  EXPECT_DOUBLE_EQ(romea::core::clamp(1.5, -1., 1.), 1.);
  EXPECT_DOUBLE_EQ(romea::core::clamp(-1.5, -1., 1.), -1.);
  EXPECT_THROW(
    romea::core::clamp(std::numeric_limits<double>::quiet_NaN(), -1., 1.),
    std::runtime_error);
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testSymmetricClamp)
{
  EXPECT_DOUBLE_EQ(romea::core::symmetricClamp(0.5, 1.), 0.5);
  EXPECT_DOUBLE_EQ(romea::core::symmetricClamp(1.5, 1.), 1.);
  EXPECT_DOUBLE_EQ(romea::core::symmetricClamp(-1.5, 1.), -1.);
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, safe_divide)
{
  EXPECT_DOUBLE_EQ(romea::core::safeDivide(1., 2.).value(), 0.5);
  EXPECT_FALSE(romea::core::safeDivide(1., 0.).has_value());
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testSignedMin)
{
  EXPECT_DOUBLE_EQ(-10, romea::core::signedMin(-10., 10.));
  EXPECT_DOUBLE_EQ(-1, romea::core::signedMin(-1., -10.));
  EXPECT_DOUBLE_EQ(1, romea::core::signedMin(1., 10.));
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testSignedFloor)
{
  EXPECT_DOUBLE_EQ(10, romea::core::signedFloor(10.4));
  EXPECT_DOUBLE_EQ(10, romea::core::signedFloor(10.6));
  EXPECT_DOUBLE_EQ(-10, romea::core::signedFloor(-10.4));
  EXPECT_DOUBLE_EQ(-10, romea::core::signedFloor(-10.6));
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testIsApproximatelyEqual)
{
  EXPECT_EQ(true, romea::core::isApproximatelyEqual(1.0, 1.0, 1e-10));
  EXPECT_EQ(false, romea::core::isApproximatelyEqual(-1.0, 1.0));
  EXPECT_EQ(true, romea::core::isApproximatelyEqual(-1.0, -1.0));
  EXPECT_EQ(true, romea::core::isApproximatelyEqual(1.0, 2.0 / 2.0));
  EXPECT_EQ(true, romea::core::isApproximatelyEqual(1000.0, 999.5, 1e-3));
  EXPECT_EQ(false, romea::core::isApproximatelyEqual(1000.0, 999.0, 1e-3));
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testNearUsesGivenEpsilon)
{
  EXPECT_TRUE(romea::core::near(1.0, 1.0 + 1e-6));
  EXPECT_TRUE(romea::core::near(1.0, 1.0 + 1e-6, 1e-5));
  EXPECT_FALSE(romea::core::near(1.0, 1.0 + 1e-6, 1e-7));
}

//-----------------------------------------------------------------------------
TEST(TestAlgorithm, testIsApproximatelyZero)
{
  EXPECT_TRUE(romea::core::isApproximatelyZero(1e-8, 1e-7));
  EXPECT_FALSE(romea::core::isApproximatelyZero(1e-6, 1e-7));
}

// TEST(TestAlgorithm,testDefinitelyGreaterThan)
//{
//  EXPECT_EQ(true,romea::isDefinitelyGreaterThan(2.0, 1.0, 1e-10));
//  EXPECT_EQ(false,romea::isDefinitelyGreaterThan(1.0, 1.0));
//  EXPECT_EQ(false,romea::isDefinitelyGreaterThan(-2.0, 1.0));
//  EXPECT_EQ(true,romea::isDefinitelyGreaterThan(-0.5, -1.0));
//  EXPECT_EQ(true,romea::isDefinitelyGreaterThan(1000.0, 999.0, 1e-3));
//  EXPECT_EQ(true,romea::isDefinitelyGreaterThan(1000.0, 998.0, 1e-3));
//}

// TEST(TestAlgorithm, testIsDefinitelyLessThan)
//{
//  EXPECT_EQ(true,romea::isDefinitelyLessThan(1.0, 2.0, 1e-10));
//  EXPECT_EQ(false,romea::isDefinitelyLessThan(1.0, 1.0));
//  EXPECT_EQ(false,romea::isDefinitelyLessThan(1.0, -2.0));
//  EXPECT_EQ(true,romea::isDefinitelyLessThan(-1.0, -0.5));
//  EXPECT_EQ(true,romea::isDefinitelyLessThan(999.0,  1000.0, 1e-3));
//  EXPECT_EQ(true,romea::isDefinitelyLessThan(1000.0, 1000.0, 1e-3));
//}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
