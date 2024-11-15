#pragma once
#include <wx/gdicmn.h>
#include <stack>
#include <vector>
#include "log.h"
#include <algorithm>

// A globle point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
wxPoint p0;

// A utility function to find next to top in a stack
wxPoint nextToTop(std::stack<wxPoint> &S)
{
    wxPoint p = S.top();
    S.pop();
    wxPoint res = S.top();
    S.push(p);
    return res;
}

// A utility function to swap two points
int swap(wxPoint &p1, wxPoint &p2)
{
    wxPoint temp = p1;
    p1 = p2;
    p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(wxPoint p1, wxPoint p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(wxPoint p, wxPoint q, wxPoint r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
   wxPoint *p1 = (wxPoint *)vp1;
   wxPoint *p2 = (wxPoint *)vp2;

   // Find orientation
   int o = orientation(p0, *p1, *p2);
   if (o == 0)
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

   return (o == 2)? -1: 1;
}

// Prints convex hull of a set of n points.
std::vector<wxPoint> convexHull(std::vector<wxPoint>& vPnts)
{
    std::vector<wxPoint> vPoints;

   // Find the bottommost point
   int ymin = vPnts[0].y;
   int min = 0;
   for (auto i = 1; i < vPnts.size(); i++)
   {
     int y = vPnts[i].y;

     // Pick the bottom-most or chose the left
     // most point in case of tie
     if ((y < ymin) || (ymin == y && vPnts[i].x < vPnts[min].x))
     {
        ymin = vPnts[i].y; 
        min = i;
     }
   }


   // Place the bottom-most point at first position
   std::swap(vPnts[0], vPnts[min]);

   // Sort n-1 points with respect to the first point.
   // A point p1 comes before p2 in sorted ouput if p2
   // has larger polar angle (in counterclockwise
   // direction) than p1
   p0 = vPnts[0];
   qsort(&vPnts[1], vPnts.size()-1, sizeof(wxPoint), compare);

   // If two or more points make same angle with p0,
   // Remove all but the one that is farthest from p0
   // Remember that, in above sorting, our criteria was
   // to keep the farthest point at the end when more than
   // one points have same angle.
   int m = 1; // Initialize size of modified array
   for (int i=1; i<vPnts.size(); i++)
   {
       // Keep removing i while angle of i and i+1 is same
       // with respect to p0
       while (i < vPnts.size()-1 && orientation(p0, vPnts[i],
                                    vPnts[i+1]) == 0)
          i++;


       vPnts[m] = vPnts[i];
       m++;  // Update size of modified array
   }

   // If modified array of points has less than 3 points,
   // convex hull is not possible
   if (m < 3)
   {
       return vPoints;
   }


   // Create an empty std::stack and push first three points
   // to it.
   std::stack<wxPoint> S;
   S.push(vPnts[0]);
   S.push(vPnts[1]);
   S.push(vPnts[2]);

   // Process remaining n-3 points
   for (int i = 3; i < m; i++)
   {
      // Keep removing top while the angle formed by
      // points next-to-top, top, and points[i] makes
      // a non-left turn
      while (orientation(nextToTop(S), S.top(), vPnts[i]) != 2)
         S.pop();
      S.push(vPnts[i]);
   }

   vPoints.reserve(S.size());

   // Now stack has the output points, print contents of stack
   while (!S.empty())
   {
       vPoints.push_back(S.top());
       S.pop();
   }
   return vPoints;
}
