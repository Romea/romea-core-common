#ifndef romea_CartesianCoordinates_hpp
#define romea_CartesianCoordinates_hpp

//Eigen
#include <Eigen/Core>

namespace romea {

template < typename Scalar> using CartesianCoordinates2 = Eigen::Matrix<Scalar,2,1> ;
template < typename Scalar> using CartesianCoordinates3 = Eigen::Matrix<Scalar,3,1> ;

using CartesianCoordinates2f=CartesianCoordinates2<float>;
using CartesianCoordinates2d=CartesianCoordinates2<double>;
using CartesianPoint2f=CartesianCoordinates2<float>;
using CartesianPoint2d=CartesianCoordinates2<double>;

using CartesianCoordinates3f=CartesianCoordinates3<float> ;
using CartesianCoordinates3d=CartesianCoordinates3<double> ;
using CartesianPoint3f=CartesianCoordinates3<float> ;
using CartesianPoint3d=CartesianCoordinates3<double>;

}

#endif