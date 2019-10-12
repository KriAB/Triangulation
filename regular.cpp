#include "regular.h"
#include <algorithm>
Regular::Regular()
{

}

Regular::Regular(std::vector<gsl::Vector3D> LAS)
{
   LASCoords = LAS;
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

void Regular::makeTriangles(unsigned int numInterval)
{
    if(numInterval < 2)
    {
        std::cout << "Number of Interval is to small for the triangulation!"<< std::endl;
    }
    if(LASCoords.size() == 0)
    {
        std::cout << "No LAS coordinates!" << std::endl;
    }
    else
    {


    //- Skal lage et rutenett basert på min og max verdier og delt opp i x interval.
    //- Først sortere punktene.
    sort();

    //- Dele opp i x antall deler (interval) mellom hvert punkt
    calcLength();
    intervalX = lengthX/numInterval;
    intervalZ = lengthZ/numInterval;
    // - lage standard punkter, som ikke er i noen triangler enda.
    makeTempPoints(numInterval);
    // - finne nærmeste punkt fra LAS til hvert egensatte punkt og bruke høyden til LAS punktet
    //Jo større interval, jo likere LAS planet. Men det vil ta veldig lang tid.
    setHeight();
    // - lage  triangler, kunne brukt indices?

    //man vet antal punkt på hver linje, for det er numInterval
    //så finne en sammenheng
    std::vector <gsl::Vector3D> list1;
    std::vector <gsl::Vector3D> list2;
    unsigned int numTemp =0;
    //Sette liste 1 og 2 først
    //liste 1 putter du første linje med punktene
    //liste 2 putter du andre linje med punktene
    for(unsigned int j = numTemp; j < numInterval; j++)
    {
        list1.push_back(tempCoords.at(j));
        list2.push_back(tempCoords.at(j+numInterval));
    }
    int n = 0;
    numTemp = numTemp + (2*numInterval);
    for(unsigned int i = 0; i < numInterval-1; i++)
    {
        //Triangulerer så de to listene med hverandre.
        //T0: L10 L20 L21 T1: L10 L21 L11,
        for(unsigned int j = 0; j <numInterval-1; j++)
        {
            std::cout << "T" << n << " | " << list1.at(j).x <<" " << list1.at(j).z <<" | " << list2.at(j).x << "," << list2.at(j).z << " | " << list2.at(j+1).x << "," << list2.at(j+1).z << std::endl;
            finalTriangles.push_back(list1.at(j));
            finalTriangles.push_back(list2.at(j));
            finalTriangles.push_back(list2.at(j+1));
            n++;
              std::cout << "T" << n << " | "<< list1.at(j).x <<" " << list1.at(j).z <<" | " << list2.at(j+1).x << "," << list2.at(j+1).z << " | " << list1.at(j+1).x <<  "," << list1.at(j+1).z << std::endl;
            finalTriangles.push_back(list1.at(j));
            finalTriangles.push_back(list2.at(j+1));
            finalTriangles.push_back(list1.at(j+1));

            n++;
        }
        //TODO: finne feilen
        //så setter man liste 1 = liste 2 og tømmer liste 2.
        list1.empty();
        list1 = list2;
        list2.empty();
        //så setter man liste 2 til linje 3 osv.
        for(unsigned int j = 0; j < numInterval; j++)
        {
          list2.push_back(tempCoords.at(j+numTemp));
        }
        if(list2.size() != numInterval)
        {
            std::cout << "Something wrong with Triangulation" << std::endl;
            break;
        }
        numTemp = numTemp+numInterval;
    }
 }

}
void Regular::makeTempPoints(unsigned int numInterval)
{
    //lage punktene bortover z aksen, så øke x, så lage de neste punktene
    float tempIntX = minX; //  + intervalX
    float tempIntZ = minZ; // + intervalZ;
    for(unsigned int i = 0; i < numInterval; i++)
    {
        for(unsigned int j = 0; j < numInterval; j++)
        {
            tempCoords.push_back(gsl::Vector3D(tempIntX, 0, tempIntZ));
            tempIntZ = tempIntZ+intervalZ;
        }
        tempIntZ = minZ;
        tempIntX = tempIntX+intervalX;
    }

}

void Regular::setHeight()
{
    gsl::Vector3D newCoord = LASCoords.at(0);
    float lengthBetABx = fabs(tempCoords.at(0).x - LASCoords.at(0).x);
    float lengthBetABz = fabs(tempCoords.at(0).z - LASCoords.at(0).z);
    float lengthBetABTempX;
    float lengthBetABTempZ;
    //Finne punktene som er nærmest de faste punktene
    int atNum = 0;
    int tempNum = 0;
    for(int i = 0; i < tempCoords.size(); i++)
    {
        // dyrt å gå igjennom hele listen hver gang
        //finne en bedre løsning
        for(int j = atNum; j < LASCoords.size(); j++)
        {
            lengthBetABTempX = fabs(tempCoords.at(i).x - LASCoords.at(j).x);
            lengthBetABTempZ = fabs(tempCoords.at(i).z - LASCoords.at(j).z);
            //hvis det er lengre lengde mellom neste enn forige så har man funnet punktet(siden de er sortert)?
            if(lengthBetABx > lengthBetABTempX && lengthBetABz > lengthBetABTempZ )
            {
                newCoord = LASCoords.at(j);
                lengthBetABx = lengthBetABTempX;
                lengthBetABz = lengthBetABTempZ;

            }
            else
            {
                tempCoords.at(i).y = LASCoords.at(j).y;
                break;
            }
            tempNum = j;
        }

       atNum = tempNum; //For å slippe å itterere igjennom heler hver gang.
    }

}

void Regular::calcLength()
{
    lengthX = maxX -minX;
    lengthY = maxY -minY; //trenger vel egentlig ikke denne?
    lengthZ = maxZ - minZ;
}

std::vector<gsl::Vector3D> Regular::getFinalTriangles() const
{
    return finalTriangles;
}

void Regular::setLASCoords(std::vector<gsl::Vector3D> &value)
{
    LASCoords = value;
}
