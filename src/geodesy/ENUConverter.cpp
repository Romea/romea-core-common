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


// romea
#include "romea_core_common/geodesy/ENUConverter.hpp"

// std
#include <cassert>
#include <cmath>

namespace romea
{
namespace core
{

//--------------------------------------------------------------------------
ENUConverter::ENUConverter()
: ecefConverter_(),
  wgs84Anchor_(),
  enu2ecef_(Eigen::Isometry3d::Identity()),
  enu2ecefAffine_(Eigen::Affine3d::Identity()),
  isAnchored_(false)
{
}

//--------------------------------------------------------------------------
ENUConverter::ENUConverter(const GeodeticCoordinates & anchor)
: ENUConverter()
{
  setAnchor(anchor);
}

//--------------------------------------------------------------------------
const GeodeticCoordinates & ENUConverter::getAnchor()const
{
  return wgs84Anchor_;
}

//--------------------------------------------------------------------------
bool ENUConverter::isAnchored() const
{
  return isAnchored_;
}

//--------------------------------------------------------------------------
void ENUConverter::setAnchor(const GeodeticCoordinates & anchor)
{
  // Set anchor
  wgs84Anchor_ = anchor;

  enu2ecef_.translation() = ecefConverter_.toECEF(wgs84Anchor_);

  // Rotation matrix giving attitude of tangent plane
  // Plane axis are East North Up
  double latitude = anchor.latitude;
  double longitude = anchor.longitude;

  enu2ecef_.linear().col(0) << -std::sin(longitude), std::cos(longitude), 0.0;
  enu2ecef_.linear().col(1) << -std::sin(latitude) * std::cos(longitude), -std::sin(latitude) * sin(
    longitude), cos(latitude);
  enu2ecef_.linear().col(2) << std::cos(latitude) * std::cos(longitude), std::cos(latitude) * sin(
    longitude), sin(latitude);

  enu2ecefAffine_ = enu2ecef_;
  isAnchored_ = true;
}

//--------------------------------------------------------------------------
Eigen::Vector3d ENUConverter::toECEF(const Eigen::Vector3d & enuPosition)const
{
  assert(isAnchored_);

  return enu2ecef_ * enuPosition;
}


//--------------------------------------------------------------------------
Eigen::Vector3d ENUConverter::toECEF(double xEast, double yNorth, double zUp) const
{
  return toECEF((Eigen::Vector3d() << xEast, yNorth, zUp).finished());
}


//--------------------------------------------------------------------------
GeodeticCoordinates ENUConverter::toWGS84(const Eigen::Vector3d & enuPosition)const
{
  return ecefConverter_.toWGS84(toECEF(enuPosition));
}

//--------------------------------------------------------------------------
GeodeticCoordinates ENUConverter::toWGS84(double xEast, double yNorth, double zUp)const
{
  return toWGS84((Eigen::Vector3d() << xEast, yNorth, zUp).finished());
}

//--------------------------------------------------------------------------
Eigen::Vector3d ENUConverter::toENU(const Eigen::Vector3d & ecefCoordinates)const
{
  assert(isAnchored_);

  return enu2ecef_.inverse() * ecefCoordinates;
}

//--------------------------------------------------------------------------
Eigen::Vector3d ENUConverter::toENU(const GeodeticCoordinates & geodeticCoordinates)
{
  if (!isAnchored()) {
    setAnchor(geodeticCoordinates);
  }

  return toENU(ecefConverter_.toECEF(geodeticCoordinates));
}

//--------------------------------------------------------------------------
Eigen::Vector3d ENUConverter::toENU(const WGS84Coordinates & wgs84Coordinates)
{
  return toENU(makeGeodeticCoordinates(wgs84Coordinates, wgs84Anchor_.altitude));
}

//--------------------------------------------------------------------------
const Eigen::Isometry3d & ENUConverter::getEnuToEcefIsometryTransform() const
{
  return enu2ecef_;
}

//--------------------------------------------------------------------------
const Eigen::Affine3d & ENUConverter::getEnuToEcefTransform() const
{
  return enu2ecefAffine_;
}

//--------------------------------------------------------------------------
void ENUConverter::reset()
{
  enu2ecef_ = Eigen::Isometry3d::Identity();
  enu2ecefAffine_ = Eigen::Affine3d::Identity();
  isAnchored_ = false;
}

}  // namespace core
}  // namespace romea
