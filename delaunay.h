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

    static std::vector<Triangle> Triangulate(const std::vector<gsl::Vector3D>& vertices);
};

#endif // DELAUNAY_H
