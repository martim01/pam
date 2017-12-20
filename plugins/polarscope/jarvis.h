#pragma once
#include <wx/gdicmn.h>
#include <vector>

int orientation(wxPoint p, wxPoint q, wxPoint r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Prints convex hull of a set of n points.
std::vector<wxPoint> convexHull(wxPoint points[], int n)
{
    // There must be at least 3 points
    if (n < 3)
        return;

    // Initialize Result
    int next[n];
    for (int i = 0; i < n; i++)
        next[i] = -1;

    // Find the leftmost point
    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again
    int p = l, q;
    do
    {
        // Search for a point 'q' such that orientation(p, i, q) is
        // counterclockwise for all points 'i'
        q = (p + 1) % n;
        for (int i = 0; i < n; i++)
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;

        next[p] = q; // Add q to result as a next point of p
        p = q; // Set p as q for next iteration
    }
    while (p != l);

    // Print Result
    std::vector<wxPoint> vPnt;
    for (int i = 0; i < n; i++)
    {
        if (next[i] != -1)
        {
           // wxLogDebug(wxT("%d: %d,%d"),i,points[i].x,points[i].y);
            vPnt.push_back(points[i]);
        }
    }
    return vPnt;
}
