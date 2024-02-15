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
#include "romea_core_common/math/Histogram.hpp"

//-----------------------------------------------------------------------------
TEST(TestHistogram, testHistogram)
{
  Eigen::VectorXd input(100);
  input << 2.80488696, 0.79862686, 2.88617381, 1.42979755, 4.66557267,
       3.74193266, 3.79799777, 2.91518339, 2.73819905, 4.96746683,
       3.0719172 , 0.84149947, 2.27081244, 4.98008889, 3.85965595,
       3.23746278, 3.07102383, 1.88663928, 2.37548671, 2.73347296,
       1.52827027, 2.42842879, 0.01812345, 1.70324113, 1.96498328,
       1.81318771, 0.3771223 , 3.01674183, 1.58709307, 2.8319989 ,
       1.59552103, 1.00134634, 2.89639195, 4.56591393, 0.33657798,
       0.53816702, 2.94792396, 1.81275622, 2.13918863, 1.55776737,
       3.98219598, 1.05752203, 0.22687698, 2.98038243, 4.02188255,
       4.01694706, 4.47375449, 2.97738917, 2.13381107, 3.57228696,
       1.02241579, 4.35846427, 2.65420524, 2.59743698, 3.4794253 ,
       1.75330099, 4.45991528, 2.99465168, 4.9476864 , 4.2975007 ,
       0.39188552, 0.64275777, 4.80085526, 2.99587368, 2.8775101 ,
       1.62659038, 0.97057192, 0.77548391, 2.00440373, 2.32492165,
       1.13470806, 4.65380675, 0.85983887, 0.96656646, 3.1253275 ,
       2.69822499, 2.97052381, 3.18894528, 2.07792698, 4.5458585 ,
       1.0446195 , 0.30321008, 0.26582821, 2.35610163, 2.76061547,
       4.99538082, 2.21685326, 0.52812752, 2.10581667, 1.41807702,
       4.58956266, 3.96145022, 2.44668966, 0.41677965, 3.18852588,
       1.97042367, 4.27075884, 3.39700243, 1.34291266, 3.23772712;
  Eigen::Matrix<double, -1, 2> bins_data;
  romea::core::computeHistogram(input, 6, -1, 5, bins_data);

  Eigen::Matrix<double, -1, 2> true_bins_data(6,2);
  true_bins_data << -0.5, 0,
                    0.5, 17,
                    1.5, 20,
                    2.5, 30,
                    3.5, 16,
                    4.5, 17;

  for (size_t i=0; i<bins_data.rows(); i++)
  {
    EXPECT_DOUBLE_EQ(bins_data(i,0), true_bins_data(i,0));
    EXPECT_DOUBLE_EQ(bins_data(i,1), true_bins_data(i,1));
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
