#ifndef REGULAR_H
#define REGULAR_H
#include <vector>
#include "vector3d.h"
#include <algorithm>

class Regular
{
public:
    Regular();
    void sort();
    //sjekker om x på punkt a er mindre enn b, hvis de er like, sjekker den y også
    bool compare(gsl::Vector3D a,gsl::Vector3D b);
    void makeTriangles();



private:
    std::vector<gsl::Vector3D> tempCoords;
    std::vector<gsl::Vector3D> finalCoords;

    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;








};

#endif // REGULAR_H
