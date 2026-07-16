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


#ifndef ROMEA_CORE_COMMON__GEODESY__ENUCONVERTER_HPP_
#define ROMEA_CORE_COMMON__GEODESY__ENUCONVERTER_HPP_

// Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>

// romea
#include "romea_core_common/geodesy/GeodeticCoordinates.hpp"
#include "romea_core_common/geodesy/ECEFConverter.hpp"


namespace romea
{
namespace core
{

class ENUConverter
{
public:
  ENUConverter();

  explicit ENUConverter(const GeodeticCoordinates & anchor);

  void setAnchor(const GeodeticCoordinates & anchor);

  const GeodeticCoordinates & getAnchor()const;

  bool isAnchored() const;

  void reset();

public:
  GeodeticCoordinates toWGS84(const Eigen::Vector3d & enuPosition)const;

  GeodeticCoordinates toWGS84(double xEast, double yNorth, double zUp) const;

  Eigen::Vector3d toECEF(const Eigen::Vector3d & enuPosition)const;

  Eigen::Vector3d toECEF(double xEast, double yNorth, double zUp) const;

  Eigen::Vector3d toENU(const GeodeticCoordinates & geodeticCoordinates);

  Eigen::Vector3d toENU(const WGS84Coordinates & wgs84Coordinates);

  Eigen::Vector3d toENU(const Eigen::Vector3d & ecefCoordinates)const;

  const Eigen::Isometry3d & getEnuToEcefIsometryTransform()const;

  [[deprecated("Use getEnuToEcefIsometryTransform() instead.")]]
  const Eigen::Affine3d & getEnuToEcefTransform()const;

private:
  ECEFConverter ecefConverter_;
  GeodeticCoordinates wgs84Anchor_;
  Eigen::Isometry3d enu2ecef_;
  Eigen::Affine3d enu2ecefAffine_;

  bool isAnchored_;
};

}  // namespace core
}  // namespace romea

#endif  // ROMEA_CORE_COMMON__GEODESY__ENUCONVERTER_HPP_
