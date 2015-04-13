#include <cmath>
#include <cv.h>
#include "cvblob.h"

namespace cvb {

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// http://www.topcoder.com/tc?module=Static&d1=tutorials&d2=geometry1

double cvDotProductPoints(CvPoint const &a, CvPoint const &b, CvPoint const &c)
{
  double abx = b.x-a.x;
  double aby = b.y-a.y;
  double bcx = c.x-b.x;
  double bcy = c.y-b.y;

  return abx*bcx + aby*bcy;
}

double cvCrossProductPoints(CvPoint const &a, CvPoint const &b, CvPoint const &c)
{
  double abx = b.x-a.x;
  double aby = b.y-a.y;
  double acx = c.x-a.x;
  double acy = c.y-a.y;

  return abx*acy - aby*acx;
}

double cvDistancePointPoint(CvPoint const &a, CvPoint const &b)
{
  double abx = a.x-b.x;
  double aby = a.y-b.y;

  return sqrt(abx*abx + aby*aby);
}

double cvDistanceLinePoint(CvPoint const &a, CvPoint const &b, CvPoint const &c, bool isSegment)
{
  if (isSegment)
  {
    double dot1 = cvDotProductPoints(a, b, c);
    if (dot1>0) return cvDistancePointPoint(b, c);

    double dot2 = cvDotProductPoints(b, a, c);
    if(dot2>0) return cvDistancePointPoint(a, c);
  }

  return fabs(cvCrossProductPoints(a,b,c)/cvDistancePointPoint(a,b));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

