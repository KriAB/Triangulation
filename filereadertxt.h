#ifndef FILEREADERTXT_H
#define FILEREADERTXT_H

#include <vector>
#include <string>
#include "vector3d.h"

class FileReaderTXT
{
public:
    FileReaderTXT();
    FileReaderTXT(std::string filename);
    //friend std::istream& operator>> (std::istream& is, gsl::Vector3D& v);
    friend std::ostream& operator<< (std::ostream&, const gsl::Vector3D&);
    void PrintCoordinates();

    std::vector<gsl::Vector3D> points;

};

#endif // FILEREADERTXT_H
