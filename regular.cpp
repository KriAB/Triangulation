#include "regular.h"
#include <algorithm>
Regular::Regular()
{

}
//En treig sorterings algoritme, må finne en bedre
void Regular::sort()
{
    minX = LASCoords.at(0).x;
    maxX = LASCoords.at(0).x;
    minY = LASCoords.at(0).y;
    maxY = LASCoords.at(0).y;
    minZ = LASCoords.at(0).z;
    maxZ = LASCoords.at(0).z;

    for( int startIndex = 0; startIndex < LASCoords.size(); ++ startIndex)
    {
        int smallestIndex = startIndex;
        for(int currIndex = startIndex + 1; currIndex < LASCoords.size(); ++currIndex)
        {
            if(compare(LASCoords[currIndex], LASCoords[smallestIndex]) == true)
            {
                smallestIndex = currIndex;
            }
        }
        std::swap(LASCoords[startIndex], LASCoords[smallestIndex]);

        //sette min og max verdier
        if(LASCoords.at(startIndex).x < minX)
            minX = LASCoords.at(startIndex).x;
        else
            maxX = LASCoords.at(startIndex).x;
        if(LASCoords.at(startIndex).y < minY)
            minY = LASCoords.at(startIndex).y;
        else
            maxY = LASCoords.at(startIndex).y;
        if(LASCoords.at(startIndex).z < minZ)
            minZ = LASCoords.at(startIndex).z;
        else
            maxZ = LASCoords.at(startIndex).z;
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
    makeTempPoints(numInterval);
    // - finne nærmeste punkt til hvert egensatte punkt og bruke høyden til dette punktet

    // - lage  triangler

    for(int i = 0; i<numInterval; i++)
    {

    }

}

void Regular::makeTempPoints(int numInterval)
{

    float tempIntX = minX; //  + intervalX
    float tempIntZ = minZ; // + intervalZ;
    for(int i = 0; i < numInterval; i++)
    {
        for(int j = 0; j < numInterval; j++)
        {

            tempIntZ = tempIntZ+intervalZ;
        }
        tempIntZ = minZ;
        tempIntX = tempIntX+intervalX;

    }

}

void Regular::calcLength()
{
    lengthX = maxX -minX;
    lengthY = maxY -minY; //trenger vel egentlig ikke denne?
    lengthZ = maxZ - minZ;
}
