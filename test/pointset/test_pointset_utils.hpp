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

#ifndef POINTSET__TEST_POINTSET_UTILS_HPP_
#define POINTSET__TEST_POINTSET_UTILS_HPP_

// std
#include <fstream>
#include <string>
#include <vector>

// romea
#include "romea_core_common/pointset/algorithms/Correspondence.hpp"
#include "romea_core_common/pointset/algorithms/NormalAndCurvatureEstimation.hpp"

//-----------------------------------------------------------------------------
template<class PointType>
romea::core::PointSet<PointType>
loadScan(const std::string & filename)
{
  std::string path = std::string(TEST_DIR);
  std::ifstream data(path + filename);
  //  ASSERT_FALSE(data.is_open());

  romea::core::PointSet<PointType> scan;
  if (romea::core::PointTraits<PointType>::DIM == 2) {
    PointType p;
    scan.reserve(1081);
    while (!data.eof()) {
      data >> p[0] >> p[1];
      scan.push_back(p);
    }
  } else {
    PointType p;
    scan.reserve(100000);
    while (!data.eof()) {
      data >> p[0] >> p[1] >> p[2];
      scan.push_back(p);
    }
  }

  return scan;
}

//-----------------------------------------------------------------------------
template<class PointType>
romea::core::PointSet<PointType>
projectScan(
  const romea::core::PointSet<PointType> & sourceScan,
  const Eigen::Transform<typename PointType::Scalar,
  romea::core::PointTraits<PointType>::DIM, Eigen::Affine> & tranformation)
{
  romea::core::PointSet<PointType> targetScan(sourceScan.size());
  for (size_t n = 0; n < sourceScan.size(); n++) {
    targetScan[n] = tranformation * sourceScan[n];
  }

  return targetScan;
}

//-----------------------------------------------------------------------------
template<class PointType>
romea::core::NormalSet<PointType>
computeNormals(const romea::core::PointSet<PointType> & scan)
{
  romea::core::NormalSet<PointType> normals(scan.size());
  romea::core::NormalAndCurvatureEstimation<PointType> normalEstimation(20);
  normalEstimation.compute(scan, normals);
  return normals;
}

//-----------------------------------------------------------------------------
std::vector<romea::core::Correspondence> fakeCorrespondences(size_t numberOfCorrespondences)
{
  std::vector<romea::core::Correspondence> correspondences(numberOfCorrespondences);

  for (size_t n = 0; n < numberOfCorrespondences; n++) {
    correspondences[n].sourcePointIndex = n;
    correspondences[n].targetPointIndex = n;
    correspondences[n].squareDistanceBetweenPoints = 0;
  }

  return correspondences;
}

#endif   // POINTSET__TEST_POINTSET_UTILS_HPP_
