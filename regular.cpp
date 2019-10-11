#include "regular.h"
#include <algorithm>
Regular::Regular()
{

}
//En treig sorterings algoritme
void Regular::sort()
{
    minX = tempCoords.at(0).x;
    maxX = tempCoords.at(0).x;
    minY = tempCoords.at(0).y;
    maxY = tempCoords.at(0).y;
    minZ = tempCoords.at(0).z;
    maxZ = tempCoords.at(0).z;

    for( int startIndex = 0; startIndex < tempCoords.size(); ++ startIndex)
    {
        int smallestIndex = startIndex;
        for(int currIndex = startIndex + 1; currIndex < tempCoords.size(); ++currIndex)
        {
            if(compare(tempCoords[currIndex], tempCoords[smallestIndex]) == true)
            {
                smallestIndex = currIndex;
            }
        }
        std::swap(tempCoords[startIndex], tempCoords[smallestIndex]);

        if(tempCoords.at(startIndex).x < minX)
            minX = tempCoords.at(startIndex).x;
        else
            maxX = tempCoords.at(startIndex).x;
        if(tempCoords.at(startIndex).y < minY)
            minY = tempCoords.at(startIndex).y;
        else
            maxY = tempCoords.at(startIndex).y;
        if(tempCoords.at(startIndex).z < minZ)
            minZ = tempCoords.at(startIndex).z;
        else
            maxZ = tempCoords.at(startIndex).z;
    }
}

bool Regular::compare(gsl::Vector3D a, gsl::Vector3D b)
{
    double epsilon = 0.00001;
    //sjekke om ax er mindre en bx                                              //Prosjektet har y opp, så bruker z her
    if(a.x < b.x ||(fabs (static_cast<double>(a.x-b.x)) < static_cast<double>(a.x)* epsilon && a.z <b.z))
        return true;

    return false;
}

void Regular::makeTriangles(int numInterval)
{
    //- Skal lage et rutenett basert på min og max verdier og delt opp i x interval.
    //- Først sortere punktene.
    sort();

    //- Dele opp i x antall deler (interval) mellom hvert punkt
    calcLength();
    intervalX = lengthX/numInterval;
    intervalZ = lengthZ/numInterval;
    // - lage standard punkter, som ikke er i noen triangler enda. Trianguleringen skjer etter dette
    makeTempPoints();
    // - finne nærmeste punkt til hvert egensatte punkt og bruke høyden til dette punktet
    // - lage  triangler





}

void Regular::makeTempPoints()
{

}

void Regular::calcLength()
{
    lengthX = maxX -minX;
    lengthY = maxY -minY; //trenger vel egentlig ikke denne?
    lengthZ = maxZ - minZ;
}
