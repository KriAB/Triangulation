#ifndef REGULAR_H
#define REGULAR_H
#include <vector>
#include "vector3d.h"

//Sparer på det punktet som er nærmest det faste satte punktet.
class Regular
{
public:
    Regular();
    Regular(std::vector<gsl::Vector3D> LAS);
    void sort();
    //sjekker om x på punkt a er mindre enn b, hvis de er like, sjekker den y også
    bool compare(gsl::Vector3D a,gsl::Vector3D b);
    void makeTriangles(unsigned int numInterval);
    void makeTempPoints(unsigned int numInterval);
    void setHeight();

void calcLength();

std::vector<gsl::Vector3D> getFinalTriangles() const;

void setLASCoords( std::vector<gsl::Vector3D> &value);

private:
std::vector<gsl::Vector3D> LASCoords;
std::vector<gsl::Vector3D> finalTriangles;
    std::vector<gsl::Vector3D> tempCoords;

    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;

    float lengthX;
    float lengthY;
    float lengthZ;

    float intervalX;
    float intervalZ;









};

#endif // REGULAR_H
