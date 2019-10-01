#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <array>
#include <vector>
#include "vector3d.h"

struct Triangle
{
    Triangle(std::array<unsigned int, 3> indices, std::array<int, 3> neighbours = {-1, -1, -1})
        : index{indices.at(0), indices.at(1), indices.at(2)},
          neighbour{neighbours.at(0), neighbours.at(1), neighbours.at(2)}
    {

    }

    unsigned int index[3];
    int neighbour[3];
};

class Delaunay
{
public:
    Delaunay();

    //sjekker om x på punkt a er mindre enn b, hvis de er like, sjekker den y også
    static bool compare(gsl::Vector3D a, gsl::Vector3D b);

    static std::vector<Triangle> Triangulate(std::vector<gsl::Vector3D> vertices);

private:
    static std::vector<Triangle> makeTriangles(std::vector<gsl::Vector3D> vertices);
    static std::vector<unsigned int> convex_hull(std::vector<std::pair<gsl::Vector3D, unsigned int>> P);
    static float cross(const gsl::Vector3D &O, const gsl::Vector3D &A, const gsl::Vector3D &B);
};

#endif // DELAUNAY_H
