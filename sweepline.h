#ifndef SWEEPLINE_H
#define SWEEPLINE_H
#include <algorithm>
#include <vector>
#include <iostream>
namespace myPoint
{
template<class U>
class SweepLine
{
    struct Point
    {
        U x;
        U y;
        U z;
    };

std::vector<Point> PtArray; // burde vel ha en pointer her
Point Pt;
//sjekker om x på punkt a er mindre enn b, hvis de er like, sjekker den y også, hvis de også er like, sjekker den z
    bool compare(Point a,Point b)
    {
        return a.x<b.x || (a.x==b.x && a.y<b.y) || (a.x==b.x && a.y == b.y && a.z<b.z);
    }

    //Returns positive value if B lies to the left of OA, negative if B lies to the right of OA, 0 if collinear
    //sjekker at de tre punktene i trekanten ikke ligger på linje, og at det ikke ligger til høyre for linjen OA
    //Må også sjekke Z at de ligger i samme dimensjon, at Z er lik
    U cross(const Point &O, const Point &A, const Point &B)
    {
        return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
    }
    //sjekker at alle punktene er i samme dimensjon, lik x, y eller z.
    bool dimension(const Point &O, const Point &A, const Point &B)
    {
        if((O.x == A.x && O.x == B.x) || (O.y == A.y && O.y == B.y) ||(O.z == A.z && O.z==B.z))
        {
            return(true);
        }
        else
            return false;
    }
    std::vector<Point> triangulation(std::vector<Point> P)
    {
        sort(P);

    //   std::sort(P.begin(), P.end(), compare);
    }
//burde nok endre sorteringsfunksjonen til en raskere
    //sortere arrayen med punkter
void sort(std::vector<Point>& P)
{
   for( int startIndex = 0; startIndex < P.size(); ++ startIndex)
   {
       int smallestIndex = startIndex;
       for(int currIndex = startIndex + 1; currIndex < P.size(); ++currIndex)
       {
           if(compare(P[currIndex], P[smallestIndex]) == true)
           {
               smallestIndex = currIndex;
           }



       }
       std::swap(P[startIndex], P[smallestIndex]);
   }
}
//burde lage en array med Points som har samme x kordinater, og de som har samme y, og de som har samme z
//kan kanskje bruke en stack eller noe
public:
    void insertPoint(U x, U y, U z)
    {
        Pt.x = x;
        Pt.y = y;
        Pt.z = z;
        PtArray.push_back(Pt);
    }

    void print()
    {
        int n = PtArray.size();
       // triangulation(PtArray);
        int i = 0;
       //std::sort(PtArray.begin(), PtArray.end(), compare);


        while (i < n)
        {

            std::cout << "x: " << PtArray[i].x << " y: " << PtArray[i].y << " z: " << PtArray[i].z << std::endl;
           i++;
        }
        std::cout << std::endl;

        sort(PtArray);
        int v = 0;
        while (v < n)
        {

            std::cout << "x: " << PtArray[v].x << " y: " << PtArray[v].y << " z: " << PtArray[v].z << std::endl;
           v++;
        }
    }
};};


#endif // SWEEPLINE_H
