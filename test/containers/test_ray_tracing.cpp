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

// std
#include <iostream>
#include <chrono>


// romea
#include "romea_core_common/containers/grid/GridIndexMapping.hpp"
#include "romea_core_common/containers/grid/WrappableGrid.hpp"
#include "romea_core_common/containers/grid/RayTracing.hpp"


//-----------------------------------------------------------------------------
TEST(TestContainers, testRayCasting2d)
{
  typedef typename romea::core::GridIndexMapping2d::PointType PointType;
  typedef typename romea::core::GridIndexMapping2d::CellIndexes CellIndexes;

  romea::core::GridIndexMapping2d gridIndexMapping(10., 0.1);
  romea::core::RayCasting2d rayCasting(&gridIndexMapping);

  EXPECT_EQ(gridIndexMapping.getNumberOfCellsAlongAxes().x(), 201);
  EXPECT_EQ(gridIndexMapping.getNumberOfCellsAlongAxes().y(), 201);

  {
    rayCasting.setOriginPoint(PointType(0, 0));
    rayCasting.setEndPoint(PointType(5, 0));

    CellIndexes originPointIndexes = rayCasting.getOriginPointIndexes();
    CellIndexes endPointIndexes = rayCasting.getEndPointIndexes();

    EXPECT_EQ(originPointIndexes.x(), 100);
    EXPECT_EQ(originPointIndexes.y(), 100);
    EXPECT_EQ(endPointIndexes.x(), 150);
    EXPECT_EQ(endPointIndexes.y(), 100);

    {
      romea::core::VectorOfEigenVector<CellIndexes> ray = rayCasting.cast();

      EXPECT_EQ(originPointIndexes.x(), ray.front().x());
      EXPECT_EQ(originPointIndexes.y(), ray.front().y());
      EXPECT_EQ(endPointIndexes.x(), ray.back().x());
      EXPECT_EQ(endPointIndexes.y(), ray.back().y());
    }

    {
      romea::core::VectorOfEigenVector<CellIndexes> ray =
        rayCasting.cast(PointType(0, 5));

      EXPECT_EQ(ray.front().x(), 100);
      EXPECT_EQ(ray.front().y(), 100);
      EXPECT_EQ(ray.back().x(), 100);
      EXPECT_EQ(ray.back().y(), 150);
    }

    {
      romea::core::VectorOfEigenVector<CellIndexes> ray =
        rayCasting.cast(PointType(-6.3, 7), PointType(5.1, 8.2));

      EXPECT_EQ(ray.front().x(), 37);
      EXPECT_EQ(ray.front().y(), 170);
      EXPECT_EQ(ray.back().x(), 151);
      EXPECT_EQ(ray.back().y(), 182);
    }
  }
}

//-----------------------------------------------------------------------------
TEST(TestContainers, testRayIterativeCasting2d)
{
  typedef typename romea::core::GridIndexMapping2d::PointType PointType;
  typedef typename romea::core::GridIndexMapping2d::CellIndexes CellIndexes;

  romea::core::GridIndexMapping2d gridIndexMapping(10., 0.1);
  romea::core::RayCasting2d rayCasting(&gridIndexMapping);

  rayCasting.setOriginPoint(PointType(4.08, -3.33));
  rayCasting.setEndPoint(PointType(-7.0123, 5.59));

  CellIndexes originPointIndexes = rayCasting.getOriginPointIndexes();
  CellIndexes endPointIndexes = rayCasting.getEndPointIndexes();

  size_t rayNumberOfCells = rayCasting.computeRayNumberOfCells();
  romea::core::VectorOfEigenVector<CellIndexes> ray(rayNumberOfCells);
  CellIndexes cellIndexes = originPointIndexes;

  ray[0] = originPointIndexes;
  for (size_t n = 1; n < rayNumberOfCells; n++) {
    rayCasting.next(cellIndexes);
    ray[n] = cellIndexes;
  }

  EXPECT_EQ(gridIndexMapping.getNumberOfCellsAlongAxes().x(), 201);
  EXPECT_EQ(gridIndexMapping.getNumberOfCellsAlongAxes().y(), 201);
  EXPECT_EQ(originPointIndexes.x(), ray.front().x());
  EXPECT_EQ(originPointIndexes.y(), ray.front().y());
  EXPECT_EQ(endPointIndexes.x(), ray.back().x());
  EXPECT_EQ(endPointIndexes.y(), ray.back().y());
}

//-----------------------------------------------------------------------------
TEST(TestContainers, testRayCasting3d)
{
  typedef typename romea::core::GridIndexMapping3d::PointType PointType;
  typedef typename romea::core::GridIndexMapping3d::CellIndexes CellIndexes;

  romea::core::GridIndexMapping3d gridIndexMapping(10., 0.1);
  romea::core::RayCasting3d rayCasting(&gridIndexMapping);

  rayCasting.setOriginPoint(PointType(4.08, -3.33, 2.34));
  rayCasting.setEndPoint(PointType(-7.0123, 5.59, 8.98));

  CellIndexes originPointIndexes = rayCasting.getOriginPointIndexes();
  CellIndexes endPointIndexes = rayCasting.getEndPointIndexes();

  romea::core::VectorOfEigenVector<CellIndexes> ray = rayCasting.cast();

  EXPECT_EQ(originPointIndexes.x(), ray.front().x());
  EXPECT_EQ(originPointIndexes.y(), ray.front().y());
  EXPECT_EQ(originPointIndexes.z(), ray.front().z());
  EXPECT_EQ(endPointIndexes.x(), ray.back().x());
  EXPECT_EQ(endPointIndexes.y(), ray.back().y());
  EXPECT_EQ(endPointIndexes.z(), ray.back().z());
}


////-----------------------------------------------------------------------------
// TEST(TestContainers,testRayCasting2dOutRange)
//{

//  typedef typename romea::GridIndexMapping2d::PointType PointType;
//  typedef typename romea::GridIndexMapping2d::CellIndexes CellIndexes;

//  romea::GridIndexMapping2d gridIndexMapping(10.,0.1);
//  romea::RayCasting2d rayCasting(&gridIndexMapping);
//  romea::Grid<float,2> grid(gridIndexMapping.getNumberOfCellsAlongAxes());


//  rayCasting.setOriginPoint(PointType(0,0));
//  rayCasting.setEndPoint(PointType(4.75192,0.415739));

////  CellIndexes originPointIndexes = rayCasting.getOriginPointIndexes();
////  CellIndexes endPointIndexes = rayCasting.getEndPointIndexes();


//  CellIndexes nextPointIndexes=rayCasting.getOriginPointIndexes();
//  rayCasting.next(nextPointIndexes);

//}


////-----------------------------------------------------------------------------
// TEST(TestContainers,testRadarCast)
//{
//  const size_t numberOfRay =401;
//  const size_t numberOfDataByRay =512;
//  const size_t numberOfParticles =100;
//  const float resolution =0.2;

//  romea::GridIndexMapping2f gridIndexMapping(numberOfDataByRay*resolution,resolution);
//  romea::Grid<int,2> grid(gridIndexMapping.getNumberOfCellsAlongAxes());
//  Eigen::ArrayXf x = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXf y = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXf cosyaw = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXf sinyaw = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXf w = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXf xi = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXf yi = Eigen::VectorXf::Zero(numberOfParticles);
//  Eigen::ArrayXi amplitudes = Eigen::VectorXi::Zero(numberOfDataByRay);
//  Eigen::ArrayXi map_amplitudes = Eigen::VectorXi::Zero(numberOfDataByRay);

//  auto start = std::chrono::system_clock::now();
//  romea::GridIndexMapping2d::CellIndexes pi;
//  float theta = 0;

//  for(size_t i=0; i < numberOfRay; ++i)
//  {
//      float costheta = std::cos(theta);
//      float sintheta = std::sin(theta);
//      for( size_t j=0; j <numberOfDataByRay ;j++)
//      {
//        float d = j*0.2;
//        xi = (x+ (cosyaw*costheta - sinyaw*sintheta)*d);
//        yi = (y+ (cosyaw*sintheta + sinyaw*costheta)*d);


//        for( size_t n=0; n <numberOfParticles ;n++)
//        {
//          pi = gridIndexMapping.computeCellIndexes(Eigen::Vector2f(xi(n),yi(n)));
//          map_amplitudes(n)=grid(pi);
//        }

//        w *= (map_amplitudes-amplitudes(j)).cast<float>()/20.;

//      }
//      theta+=2*M_PI/numberOfRay;
//  }
//  auto end = std::chrono::system_clock::now();
//  std::chrono::duration<double> diff = end-start;
//  std::cout << "time ellapsed " <<diff.count() << std::endl;
//}


//-----------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
