#ifndef romea_Correspondence_hpp
#define romea_Correspondence_hpp

//std
#include <cstddef>

namespace romea{

struct Correspondence{

  Correspondence();

  Correspondence(const size_t &sourcePointIndex,
                 const size_t &targetPointIndex);

  Correspondence(const size_t &sourcePointIndex,
                 const size_t &targetPointIndex,
                 const double & squareDistanceBetweenPoints);

  Correspondence(const size_t &sourcePointIndex,
                 const size_t &targetPointIndex,
                 const double & squareDistanceBetweenPoints,
                 const double & weight);

  size_t sourcePointIndex;

  size_t targetPointIndex;

  double squareDistanceBetweenPoints;

  double weight;
};


//-----------------------------------------------------------------------------
inline bool sortByTargetIndexAndDistancePredicate(const Correspondence & c1,const Correspondence & c2)
{
  if (c1.targetPointIndex < c2.targetPointIndex)
    return (true);
  else if ((c1.targetPointIndex == c2.targetPointIndex) &&
           (c1.squareDistanceBetweenPoints < c2.squareDistanceBetweenPoints))
    return (true);
  return (false);
}

//-----------------------------------------------------------------------------
inline bool sortBySourceIndexAndDistancePredicate(const Correspondence & c1,const Correspondence & c2)
{
  if (c1.sourcePointIndex < c2.sourcePointIndex)
    return (true);
  else if ((c1.sourcePointIndex == c2.sourcePointIndex) &&
           (c1.squareDistanceBetweenPoints < c2.squareDistanceBetweenPoints))
    return (true);
  return (false);
}

//-----------------------------------------------------------------------------
inline bool equalTargetIndexesPredicate(const Correspondence & c1,const Correspondence & c2)
{
  return c1.targetPointIndex == c2.targetPointIndex;
}

//-----------------------------------------------------------------------------
inline bool equalSourceIndexesPredicate(const Correspondence & c1,const Correspondence & c2)
{
  return c1.sourcePointIndex == c2.sourcePointIndex;
}

}
#endif