#include <iostream>
#include <vector>
#include <algorithm>
#include "sweepline.h"
#include "delaunay.h"
using namespace std;


//convex
struct Point
{
    double x, y;
};

//sjekker om x på punkt a er mindre enn b, hvis de er like, sjekker den y også
bool compare(Point a,Point b)
{
    return a.x<b.x || (a.x==b.x && a.y<b.y);
}
//Returns positive value if B lies to the left of OA, negative if B lies to the right of OA, 0 if collinear
double cross(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
//Returns a list of points on the convex hull
vector<Point> convex_hull(vector<Point> P)
{
    int n = P.size(), k = 0;
    vector<Point> H(2*n);
    sort(P.begin(), P.end(),compare);
    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    // Build upper hull
    //i starts from n-2 because n-1 is the point which both hulls will have in common
    //t=k+1 so that the upper hull has atleast two points to begin with
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }
    //the last point of upper hull is same with the fist point of the lower hull
    H.resize(k-1);

    int a = 0;
    for( int i = n; i >= 0; i-- )
    {
        cout << "X: "<< P[a].x << " Y: " << P[a].y << endl;
        a++;
    }

    cout << endl<< "H ";
    int b = 0;
    for(int i = n; i >= 0; i-- )
    {
        cout << "X: "<< H[b].x << " Y: " << H[b].y << endl;
        b++;
    }
    return H;
}

#define MAX 1000
struct event
{
    int ind; // Index of rectangle in rects
    bool type; // Type of event: 0 = Lower-left ; 1 = Upper-right
    event() {};
    event(int ind, int type) : ind(ind), type(type) {};
};
struct point
{
    int x, y;
};
point rects [MAX][12]; // Each rectangle consists of 2 points: [0] = lower-left ; [1] = upper-right
bool compare_x(event a, event b)
{ return rects[a.ind][a.type].x<rects[b.ind][b.type].x; }

bool compare_y(event a, event b)
{ return rects[a.ind][a.type].y<rects[b.ind][b.type].y; }

int union_area(event events_v[],event events_h[],int n,int e)
{
    //n is the number of rectangles, e=2*n , e is the number of points (each rectangle has two points as described in declaration of rects)
    bool in_set[MAX]={0};int area=0;
    sort(events_v, events_v+e, compare_x);  //Pre-sort of vertical edges
    sort(events_h, events_h+e, compare_y); // Pre-sort set of horizontal edges
    in_set[events_v[0].ind] = 1;
    for (int i=1;i<e;++i)
    { // Vertical sweep line
        event c = events_v[i];
        int cnt = 0; // Counter to indicate how many rectangles are currently overlapping
        // Delta_x: Distance between current sweep line and previous sweep line
        int delta_x = rects[c.ind][c.type].x - rects[events_v[i-1].ind][events_v[i-1].type].x;
        int begin_y;
        if (delta_x==0){
            in_set[c.ind] = (c.type==0);
            continue;
        }
        for (int j=0;j<e;++j)
            if (in_set[events_h[j].ind]==1)                 //Horizontal sweep line for active rectangle
            {
                if (events_h[j].type==0)                //If it is a bottom edge of rectangle
                {
                    if (cnt==0) begin_y = rects[events_h[j].ind][0].y; // Block starts
                    ++cnt;                          //incrementing number of overlapping rectangles
                }
                else                                    //If it is a top edge
                {
                    --cnt;                          //the rectangle is no more overlapping, so remove it
                    if (cnt==0)                     //Block ends
                    {
                        int delta_y = (rects[events_h[j].ind][13].y-begin_y);//length of the vertical sweep line cut by rectangles
                        area+=delta_x * delta_y;
                    }
                }
            }
        in_set[c.ind] = (c.type==0);//If it is a left edge, the rectangle is in the active set else not
    }
    return area;
}

int main()
{
//    // Create
//    ////  vector< vector<int> > vec(4, vector<int>(4));
//    // Write
//    // vec[2][3] = 10;
//    // Read
//    //int a = vec[2][3];
//    Point pt1;
//    pt1.x = 0;
//    pt1.y = 1;

//    Point pt2;
//    pt2.x = 1;
//    pt2.y = 0;

//    Point pt3;
//    pt3.x= 2;
//    pt3.y = 0;

//    Point pt4;
//    pt4.x=3;
//    pt4.y=1;
//    Point pt5;
//    pt5.x =0;
//    pt5.y =0;
//    Point pt6;
//    pt6.x = 1;
//    pt6.y = 1;
//    Point pt7;
//    pt7.x = 4;
//    pt7.y = 5;
//    Point pt8;
//    pt8.x = 6;
//    pt8.y = 0;
//    Point pt9;
//    pt9.x = 2;
//    pt9.y = 2;

//    Point pt10;
//    pt10.x =3;
//    pt10.y=2;


//    vector<Point> ptvec;
//    ptvec.push_back(pt1);
//    ptvec.push_back(pt2);
//    ptvec.push_back(pt3);
//    ptvec.push_back(pt4);
//    ptvec.push_back(pt5);
//    ptvec.push_back(pt6);
//    ptvec.push_back(pt7);
//    ptvec.push_back(pt8);
//    ptvec.push_back(pt9);
//    ptvec.push_back(pt10);

//    convex_hull(ptvec);

////        int a = 0;
////        for(int i = 6; i >= 0; i-- )
////        {
////          cout << "X: "<< ptvec[a].x << " Y: " << ptvec[a].y << endl;
////          a++;
////        }
////    cout << "Hello World!" << endl;

//myPoint::SweepLine<double> One;

////One.insertPoint(1, 0, 0);
////One.insertPoint(0, 0, 0);
////One.insertPoint(2, 0, 0);
////One.insertPoint(1, 3, 0);
////One.insertPoint(1, 2, 0);
////One.insertPoint(0, 1, 0);
////One.insertPoint(0, 1, 5);
////One.insertPoint(0, 1, 4);
////One.insertPoint(0, 0, 5);
////One.print();

    std::vector<gsl::Vector3D> vertices{
        {0.f, 2.f, 1.f},
        {-3.f, 2.f, 3.f},
        {2.f, 2.f, 3.f},
        {2.f, 5.f, 0.f},
        {3.f, 0.f, -2.f},
        {2.f, -5.f, 2.f}
    };

    Delaunay::Triangulate(vertices);

    return 0;
}
