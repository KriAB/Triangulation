#include "delaunay.h"
#include <limits>
#include <algorithm>

Delaunay::Delaunay()
{

}

bool Delaunay::compare(gsl::Vector3D a, gsl::Vector3D b)
{
    return a.x < b.x || (std::abs(a.x - b.x) < std::numeric_limits<float>::epsilon() && a.y < b.y);
}

std::vector<Triangle> Delaunay::Triangulate(std::vector<gsl::Vector3D> vertices)
{
    if (vertices.size() < 3)
        return {};

    std::vector<std::pair<gsl::Vector3D, unsigned int>> vertexAndIndex{vertices.size()};
    for (unsigned int i{0}; i < vertices.size(); ++i) { vertexAndIndex[i] = {vertices[i], i}; }

    // Sorting using a lambda
    std::sort(vertexAndIndex.begin(), vertexAndIndex.end(),
              [](const std::pair<gsl::Vector3D, unsigned int> &a, const std::pair<gsl::Vector3D, unsigned int> &b){
        return a.first.x < b.first.x || (std::abs(a.first.x - b.first.x) < std::numeric_limits<float>::epsilon() && a.first.y < b.first.y);
    });

    auto outline = convex_hull(vertexAndIndex);

//    auto A = makeTriangles(outline);
//    auto B = makeTriangles(outline);
//    std::vector<Triangle> AB;
//    AB.reserve(A.size() + B.size());
//    AB.insert(AB.end(), A.begin(), A.end());
//    AB.insert(AB.end(), B.begin(), B.end());
//    return AB;

    return {};
}

std::vector<Triangle> Delaunay::makeTriangles(std::vector<gsl::Vector3D> vertices)
{

}

//Returns positive value if B lies to the left of OA, negative if B lies to the right of OA, 0 if collinear
float Delaunay::cross(const gsl::Vector3D &O, const gsl::Vector3D &A, const gsl::Vector3D &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}
//Returns a list of indexes for the points on the convex hull
std::vector<unsigned int> Delaunay::convex_hull(std::vector<std::pair<gsl::Vector3D, unsigned int>> P)
{
    int n = P.size(), k = 0;
    std::vector<std::pair<gsl::Vector3D, unsigned int>> H(2*n);
    // sort(P.begin(), P.end(), compare);
    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2].first, H[k-1].first, P[i].first) <= 0) k--;
        H[k++] = P[i];
    }

    // Build upper hull
    //i starts from n-2 because n-1 is the point which both hulls will have in common
    //t=k+1 so that the upper hull has atleast two points to begin with
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(H[k-2].first, H[k-1].first, P[i].first) <= 0) k--;
        H[k++] = P[i];
    }
    //the last point of upper hull is same with the fist point of the lower hull
    H.resize(k-1);

    // Convert the pairs to a list of indexes to the output
    std::vector<unsigned int> outputs;
    outputs.resize(H.size());
    std::transform(H.begin(), H.end(), outputs.begin(),
                   [](const std::pair<gsl::Vector3D, unsigned int> pair){ return pair.second; });

    return outputs;
}
