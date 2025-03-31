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
#include <cstddef>
#include <stdexcept>

// romea
#include "romea_core_common/lexical/LexicalCast.hpp"

using namespace romea::core;

//-----------------------------------------------------------------------------
TEST(TestLexicalCast, bool_integer)
{
  std::size_t pos{};

  EXPECT_EQ(lexical_cast<bool>("1", pos), true);
  EXPECT_EQ(pos, 1);

  EXPECT_EQ(lexical_cast<bool>("0", pos), false);
  EXPECT_EQ(pos, 1);

  EXPECT_EQ(lexical_cast<bool>("0"), false);
  EXPECT_EQ(lexical_cast<bool>("1"), true);
}

TEST(TestLexicalCast, bool_alpha)
{
  std::size_t pos{};
  EXPECT_EQ(lexical_cast<bool>("true", pos), true);
  EXPECT_EQ(pos, 4);

  EXPECT_EQ(lexical_cast<bool>("false", pos), false);
  EXPECT_EQ(pos, 5);

  EXPECT_EQ(lexical_cast<bool>("true"), true);
  EXPECT_EQ(lexical_cast<bool>("false"), false);

  EXPECT_EQ(lexical_cast<bool>("True"), true);
  EXPECT_EQ(lexical_cast<bool>("False"), false);
  EXPECT_EQ(lexical_cast<bool>("TRUE"), true);
  EXPECT_EQ(lexical_cast<bool>("FALSE"), false);
}

TEST(TestLexicalCast, bool_alpha_error)
{
  std::size_t pos{};
  lexical_cast<bool>("cheval", pos);
  EXPECT_EQ(pos, 0);

  EXPECT_ANY_THROW(lexical_cast<bool>("cheval"));
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
